#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    int r0, c0;
    cin >> r0 >> c0;
    --r0; --c0;

    int64_t nn = 1LL * N * N;
    vector<array<int, 8>> neigh; neigh.resize(nn);
    vector<uint8_t> deg0(nn, 0);
    auto id = [&](int r, int c) { return r * N + c; };
    auto rc = [&](int idx) { return pair<int,int>(idx / N, idx % N); };

    static const int dr[8] = {-2,-1,1,2,2,1,-1,-2};
    static const int dc[8] = {1,2,2,1,-1,-2,-2,-1};

    // Build adjacency
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int u = id(r,c);
            uint8_t cnt = 0;
            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    neigh[u][cnt++] = id(nr, nc);
                }
            }
            deg0[u] = cnt;
        }
    }

    vector<uint8_t> deg(nn);
    vector<int> path; path.reserve(nn);
    vector<int> best_path; best_path.reserve(nn);
    vector<int> cand(8), tie1(8), tie2(8);
    vector<int> visit(nn, 0);
    int stamp = 0;
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    auto attempt = [&](int start)->int {
        ++stamp;
        // reset deg
        for (int i = 0; i < nn; ++i) deg[i] = deg0[i];
        path.clear();
        int cur = start;
        path.push_back(cur);
        visit[cur] = stamp;
        for (int j = 0; j < deg0[cur]; ++j) {
            int v = neigh[cur][j];
            if (visit[v] != stamp) deg[v]--;
        }
        while ((int)path.size() < nn) {
            int csz = 0;
            int mind = 255;
            // collect unvisited neighbors and find minimal onward degree
            uint8_t ncnt = deg0[cur];
            for (int i = 0; i < ncnt; ++i) {
                int u = neigh[cur][i];
                if (visit[u] == stamp) continue;
                int d = deg[u];
                if (d < mind) {
                    mind = d;
                    cand[0] = u; csz = 1;
                } else if (d == mind) {
                    cand[csz++] = u;
                }
            }
            if (csz == 0) break;

            int sel = -1;
            if (csz == 1) {
                sel = cand[0];
            } else {
                // second-level tie-break: minimal min neighbor degree
                int bcnt = 0;
                int bestSecond = 255;
                for (int i = 0; i < csz; ++i) {
                    int u = cand[i];
                    int m2 = 255;
                    uint8_t n2 = deg0[u];
                    for (int j = 0; j < n2; ++j) {
                        int w = neigh[u][j];
                        if (visit[w] == stamp) continue;
                        int dv = deg[w];
                        if (dv < m2) m2 = dv;
                    }
                    if (m2 < bestSecond) {
                        bestSecond = m2;
                        tie1[0] = u; bcnt = 1;
                    } else if (m2 == bestSecond) {
                        tie1[bcnt++] = u;
                    }
                }
                if (bcnt == 1) {
                    sel = tie1[0];
                } else {
                    // third-level: minimal third-level sum (optional small lookahead)
                    // We'll compute sum of degrees of neighbors unvisited
                    int cbcnt = 0;
                    int bestThird = INT_MAX;
                    for (int i = 0; i < bcnt; ++i) {
                        int u = tie1[i];
                        int sum3 = 0;
                        uint8_t n2 = deg0[u];
                        for (int j = 0; j < n2; ++j) {
                            int w = neigh[u][j];
                            if (visit[w] == stamp) continue;
                            sum3 += deg[w];
                        }
                        if (sum3 < bestThird) {
                            bestThird = sum3;
                            tie2[0] = u; cbcnt = 1;
                        } else if (sum3 == bestThird) {
                            tie2[cbcnt++] = u;
                        }
                    }
                    // random among remaining ties
                    uniform_int_distribution<int> dist(0, cbcnt-1);
                    sel = tie2[dist(rng)];
                }
            }
            cur = sel;
            path.push_back(cur);
            visit[cur] = stamp;
            uint8_t ncnt2 = deg0[cur];
            for (int j = 0; j < ncnt2; ++j) {
                int v = neigh[cur][j];
                if (visit[v] != stamp) deg[v]--;
            }
        }
        return (int)path.size();
    };

    int startId = id(r0, c0);

    // Time management
    auto t0 = chrono::steady_clock::now();
    const double TIME_LIMIT_SEC = 0.90; // stay under 1s
    int best_len = 0;
    int attempts = 0;

    // Adaptive number of attempts depending on N
    int maxAttempts;
    if (N <= 8) maxAttempts = 50000;
    else if (N <= 12) maxAttempts = 20000;
    else if (N <= 20) maxAttempts = 10000;
    else if (N <= 50) maxAttempts = 3000;
    else if (N <= 150) maxAttempts = 1000;
    else if (N <= 300) maxAttempts = 400;
    else maxAttempts = 200;

    // First try deterministic move-order
    int len = attempt(startId);
    attempts++;
    if (len == nn) {
        best_len = len;
        best_path = path;
    } else {
        best_len = len;
        best_path = path;
    }

    while (best_len < nn && attempts < maxAttempts) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - t0).count();
        if (elapsed > TIME_LIMIT_SEC) break;

        len = attempt(startId);
        attempts++;
        if (len > best_len) {
            best_len = len;
            best_path = path;
            if (best_len == nn) break;
        }
    }

    cout << best_len << '\n';
    for (int i = 0; i < best_len; ++i) {
        auto p = rc(best_path[i]);
        cout << (p.first + 1) << ' ' << (p.second + 1);
        if (i + 1 < best_len) cout << '\n';
    }
    return 0;
}