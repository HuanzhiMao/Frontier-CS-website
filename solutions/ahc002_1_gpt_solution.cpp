#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int H = 50, W = 50, N = H * W;
    int si, sj;
    if (!(cin >> si >> sj)) return 0;
    vector<int> tile(N), pval(N);
    int mx = -1;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int t; cin >> t;
            tile[i*W + j] = t;
            if (t > mx) mx = t;
        }
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int v; cin >> v;
            pval[i*W + j] = v;
        }
    }
    int M = mx + 1;
    vector<char> used(M, 0);
    int cur = si * W + sj;
    used[tile[cur]] = 1;

    // Directions
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    const char mvch[4] = {'U', 'D', 'L', 'R'};

    string ans;
    ans.reserve(3000);

    // BFS arrays
    vector<int> dist(N, -1), par(N, -1);
    vector<int> q(N);
    auto inb = [&](int r, int c){ return (0 <= r && r < H && 0 <= c && c < W); };

    // Degree function for a cell after stepping into it (tile becomes used)
    auto degree_after_step = [&](int id)->int{
        int r = id / W, c = id % W;
        int tid = tile[id];
        int cnt = 0;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!inb(nr, nc)) continue;
            int nid = nr * W + nc;
            int t2 = tile[nid];
            if (used[t2]) continue;
            if (t2 == tid) continue; // cannot step within same tile after stepping in
            cnt++;
        }
        return cnt;
    };

    // Random engine for tie-breaking
    unsigned long long seed = 88172645463393265ull ^ (unsigned long long)chrono::high_resolution_clock::now().time_since_epoch().count();
    auto rng = [&]()->unsigned{
        seed ^= seed << 7;
        seed ^= seed >> 9;
        return (unsigned)seed;
    };

    // Main loop
    while (true) {
        int r = cur / W, c = cur % W;
        // Check available immediate moves
        vector<int> neigh;
        vector<int> ndir;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!inb(nr, nc)) continue;
            int nid = nr * W + nc;
            if (!used[tile[nid]]) {
                neigh.push_back(nid);
                ndir.push_back(d);
            }
        }
        if (neigh.empty()) break;

        // BFS from current position on grid with used tiles blocked
        fill(dist.begin(), dist.end(), -1);
        fill(par.begin(), par.end(), -1);
        int qh = 0, qt = 0;
        dist[cur] = 0;
        q[qt++] = cur;
        while (qh < qt) {
            int u = q[qh++];
            int ur = u / W, uc = u % W;
            for (int d = 0; d < 4; d++) {
                int vr = ur + dr[d], vc = uc + dc[d];
                if (!inb(vr, vc)) continue;
                int v = vr * W + vc;
                if (dist[v] != -1) continue;
                if (used[tile[v]]) continue;
                dist[v] = dist[u] + 1;
                par[v] = u;
                q[qt++] = v;
            }
        }

        // If BFS cannot reach anything other than current, fallback to any neighbor (shouldn't usually happen if neigh non-empty)
        double bestScore = -1e100;
        int bestTarget = -1;
        // Scoring parameters
        const double PWEIGHT = 1.0;
        const double DWEIGHT = 2.2;
        // Choose best target by score = p - DWEIGHT*dist, dist>0
        for (int i = 0; i < N; i++) {
            if (dist[i] <= 0) continue; // skip self/unreachable
            double s = PWEIGHT * pval[i] - DWEIGHT * dist[i];
            // small random noise for tie-breaking
            s += (rng() & 1023) * 1e-6;
            if (s > bestScore) {
                bestScore = s;
                bestTarget = i;
            }
        }

        int nextId = -1;
        int nextDir = -1;

        if (bestTarget != -1) {
            // Reconstruct first step towards bestTarget
            int x = bestTarget;
            while (par[x] != -1 && par[x] != cur) x = par[x];
            if (par[x] == -1) {
                // Should not happen, but fallback to greedy neighbor
                int bestIdx = -1;
                int bestP = -1;
                int bestDeg = -1;
                for (int k = 0; k < (int)neigh.size(); k++) {
                    int nid = neigh[k];
                    int pv = pval[nid];
                    int deg = degree_after_step(nid);
                    // Prefer higher p, then higher degree
                    if (pv > bestP || (pv == bestP && deg > bestDeg)) {
                        bestP = pv; bestDeg = deg; bestIdx = k;
                    }
                }
                nextId = neigh[bestIdx];
                nextDir = ndir[bestIdx];
            } else {
                nextId = x;
                // Determine direction
                int nr = nextId / W, nc = nextId % W;
                if (nr == r - 1 && nc == c) nextDir = 0;
                else if (nr == r + 1 && nc == c) nextDir = 1;
                else if (nr == r && nc == c - 1) nextDir = 2;
                else if (nr == r && nc == c + 1) nextDir = 3;
                else {
                    // Should not happen; fallback
                    int bestIdx = -1;
                    int bestP = -1;
                    int bestDeg = -1;
                    for (int k = 0; k < (int)neigh.size(); k++) {
                        int nid = neigh[k];
                        int pv = pval[nid];
                        int deg = degree_after_step(nid);
                        if (pv > bestP || (pv == bestP && deg > bestDeg)) {
                            bestP = pv; bestDeg = deg; bestIdx = k;
                        }
                    }
                    nextId = neigh[bestIdx];
                    nextDir = ndir[bestIdx];
                }
            }
        } else {
            // Fallback: choose immediate neighbor with best local heuristic: p + alpha*deg
            int bestIdx = -1;
            double bestLocal = -1e100;
            for (int k = 0; k < (int)neigh.size(); k++) {
                int nid = neigh[k];
                int pv = pval[nid];
                int deg = degree_after_step(nid);
                double s = pv + 2.5 * deg + (rng() & 1023) * 1e-6;
                if (s > bestLocal) {
                    bestLocal = s;
                    bestIdx = k;
                }
            }
            nextId = neigh[bestIdx];
            nextDir = ndir[bestIdx];
        }

        if (nextId == -1) break; // no move
        ans.push_back(mvch[nextDir]);
        cur = nextId;
        used[tile[cur]] = 1;
    }

    cout << ans << "\n";
    return 0;
}