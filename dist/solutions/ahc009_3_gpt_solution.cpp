#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int si, sj, ti, tj;
    double p;
    if (!(cin >> si >> sj >> ti >> tj >> p)) {
        return 0;
    }
    vector<string> h(20);
    for (int i = 0; i < 20; ++i) cin >> h[i];
    vector<string> v(19);
    for (int i = 0; i < 19; ++i) cin >> v[i];

    const int H = 20, W = 20, N = H * W;
    auto id = [&](int r, int c) { return r * W + c; };
    auto rc = [&](int idx) { return pair<int,int>(idx / W, idx % W); };

    // Precompute neighbors: 0:U 1:D 2:L 3:R
    int neigh[N][4];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int idx = id(i, j);
            // Up
            if (i == 0) neigh[idx][0] = -1;
            else neigh[idx][0] = (v[i-1][j] == '0') ? id(i-1, j) : -1;
            // Down
            if (i == H-1) neigh[idx][1] = -1;
            else neigh[idx][1] = (v[i][j] == '0') ? id(i+1, j) : -1;
            // Left
            if (j == 0) neigh[idx][2] = -1;
            else neigh[idx][2] = (h[i][j-1] == '0') ? id(i, j-1) : -1;
            // Right
            if (j == W-1) neigh[idx][3] = -1;
            else neigh[idx][3] = (h[i][j] == '0') ? id(i, j+1) : -1;
        }
    }

    int s = id(si, sj);
    int t = id(ti, tj);

    // BFS distance to target
    const int INF = 1e9;
    vector<int> dist_to_t(N, INF);
    queue<int> q;
    dist_to_t[t] = 0;
    q.push(t);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int du = dist_to_t[u];
        // iterate neighbors (undirected edges)
        for (int d = 0; d < 4; ++d) {
            int vtx = neigh[u][d];
            if (vtx >= 0 && dist_to_t[vtx] == INF) {
                dist_to_t[vtx] = du + 1;
                q.push(vtx);
            }
        }
        // Also need reverse neighbors: if there is edge from x to u
        // But since edges are undirected, neigh[x][dir]==u implies neigh[u][opposite]==x,
        // and iteration above already pushes such vtx from u to neighbor. That's enough to cover all.
        // No need to explicitly add reverse edges.
    }

    // safe: all squares are reachable from the home per problem statement.

    // Helper function to compute one-step transition: cur -> nxt with direction d
    auto update_step = [&](const double* cur, double* nxt, int d, double &absorb, double &expDistAfter, const vector<int>& dist_to_t_local) {
        absorb = 0.0;
        expDistAfter = 0.0;
        // zero nxt
        for (int i = 0; i < N; ++i) nxt[i] = 0.0;
        for (int x = 0; x < N; ++x) {
            if (x == t) continue; // mass should never be at t in cur, but skip just in case
            double m = cur[x];
            if (m == 0.0) continue;
            int y = neigh[x][d];
            if (y < 0) {
                // blocked, stay with prob 1
                nxt[x] += m;
                expDistAfter += m * dist_to_t_local[x];
            } else if (y == t) {
                // can reach goal
                double to_goal = (1.0 - p) * m;
                absorb += to_goal;
                double stay = p * m;
                nxt[x] += stay;
                expDistAfter += stay * dist_to_t_local[x];
            } else {
                double move = (1.0 - p) * m;
                double stay = p * m;
                nxt[y] += move;
                nxt[x] += stay;
                expDistAfter += move * dist_to_t_local[y] + stay * dist_to_t_local[x];
            }
        }
    };

    // If already at goal
    if (s == t) {
        cout << "\n";
        return 0;
    }

    // Greedy construction with lookahead
    const int MAXL = 200;
    const int DEPTH = 3; // lookahead depth
    const double wDist = 5.0; // weight for expected distance reduction

    vector<double> dist(N, 0.0), tmp(N, 0.0);
    dist[s] = 1.0;

    string res;
    res.reserve(MAXL);
    const char DIRCH[4] = {'U','D','L','R'};

    // pre-allocated buffers for caching per depth
    static double cur1[4][N];
    static double cur2[4][4][N];

    for (int step = 1; step <= MAXL; ++step) {
        // total remaining mass
        double remaining = 0.0;
        for (int i = 0; i < N; ++i) remaining += dist[i];
        if (remaining < 1e-15) break;

        int rem = MAXL - (int)res.size();
        int K = min(DEPTH, rem);

        // expected distance before
        double eDistStart = 0.0;
        for (int i = 0; i < N; ++i) eDistStart += dist[i] * dist_to_t[i];

        int bestFirst = 0;
        double bestScore = -1e100;

        if (K == 1) {
            for (int d1 = 0; d1 < 4; ++d1) {
                double absorb1 = 0.0, eDist1 = 0.0;
                update_step(dist.data(), tmp.data(), d1, absorb1, eDist1, dist_to_t);
                double score = absorb1 * (401 - step) + wDist * (eDistStart - eDist1);
                if (score > bestScore) {
                    bestScore = score;
                    bestFirst = d1;
                }
            }
        } else if (K == 2) {
            double s1val[4];
            for (int d1 = 0; d1 < 4; ++d1) {
                double absorb1 = 0.0, eDist1 = 0.0;
                update_step(dist.data(), cur1[d1], d1, absorb1, eDist1, dist_to_t);
                s1val[d1] = absorb1 * (401 - step);
            }
            for (int d1 = 0; d1 < 4; ++d1) {
                for (int d2 = 0; d2 < 4; ++d2) {
                    double absorb2 = 0.0, eDist2 = 0.0;
                    update_step(cur1[d1], tmp.data(), d2, absorb2, eDist2, dist_to_t);
                    double score = s1val[d1] + absorb2 * (401 - (step + 1)) + wDist * (eDistStart - eDist2);
                    if (score > bestScore) {
                        bestScore = score;
                        bestFirst = d1;
                    }
                }
            }
        } else { // K == 3
            double s1val[4];
            double s2val[4][4];
            double eDist2_cache[4][4];
            for (int d1 = 0; d1 < 4; ++d1) {
                double absorb1 = 0.0, eDist1 = 0.0;
                update_step(dist.data(), cur1[d1], d1, absorb1, eDist1, dist_to_t);
                s1val[d1] = absorb1 * (401 - step);
            }
            for (int d1 = 0; d1 < 4; ++d1) {
                for (int d2 = 0; d2 < 4; ++d2) {
                    double absorb2 = 0.0, eDist2 = 0.0;
                    update_step(cur1[d1], cur2[d1][d2], d2, absorb2, eDist2, dist_to_t);
                    s2val[d1][d2] = s1val[d1] + absorb2 * (401 - (step + 1));
                    eDist2_cache[d1][d2] = eDist2;
                }
            }
            for (int d1 = 0; d1 < 4; ++d1) {
                for (int d2 = 0; d2 < 4; ++d2) {
                    for (int d3 = 0; d3 < 4; ++d3) {
                        double absorb3 = 0.0, eDist3 = 0.0;
                        update_step(cur2[d1][d2], tmp.data(), d3, absorb3, eDist3, dist_to_t);
                        double score = s2val[d1][d2] + absorb3 * (401 - (step + 2)) + wDist * (eDistStart - eDist3);
                        if (score > bestScore) {
                            bestScore = score;
                            bestFirst = d1;
                        }
                    }
                }
            }
        }

        // Apply the chosen first step to dist
        double absorb_apply = 0.0, eDist_after_apply = 0.0;
        update_step(dist.data(), tmp.data(), bestFirst, absorb_apply, eDist_after_apply, dist_to_t);
        // update dist
        dist.swap(tmp);
        res.push_back(DIRCH[bestFirst]);
    }

    cout << res << "\n";
    return 0;
}