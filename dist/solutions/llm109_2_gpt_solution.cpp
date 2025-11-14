#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t s;
    RNG() {
        uint64_t x = chrono::high_resolution_clock::now().time_since_epoch().count();
        s = x ^ (x << 13) ^ (x >> 7) ^ (x << 17);
        if (s == 0) s = 88172645463393265ull;
    }
    inline uint64_t next() {
        s ^= s << 7;
        s ^= s >> 9;
        s ^= s << 8;
        return s;
    }
    inline size_t next_size_t(size_t n) { // 0..n-1
        return (size_t)(next() % (n ? n : 1));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    int r0, c0;
    cin >> r0 >> c0;
    --r0; --c0;
    int V = N * N;

    // Precompute neighbors for each cell as static arrays for memory efficiency.
    const int drs[8] = {2,1,-1,-2,-2,-1,1,2};
    const int dcs[8] = {1,2,2,1,-1,-2,-2,-1};
    vector<array<int, 8>> nbr(V);
    vector<uint8_t> nbrCount(V, 0);

    auto inb = [&](int r, int c) { return r >= 0 && r < N && c >= 0 && c < N; };

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int id = r * N + c;
            uint8_t cnt = 0;
            for (int k = 0; k < 8; ++k) {
                int nr = r + drs[k];
                int nc = c + dcs[k];
                if (inb(nr, nc)) {
                    nbr[id][cnt++] = nr * N + nc;
                }
            }
            nbrCount[id] = cnt;
        }
    }

    RNG rng;

    auto unvisited_degree = [&](int v, const vector<uint8_t>& vis) {
        int cnt = 0;
        uint8_t nc = nbrCount[v];
        for (uint8_t i = 0; i < nc; ++i) {
            int u = nbr[v][i];
            if (!vis[u]) ++cnt;
        }
        return cnt;
    };

    auto attempt = [&](int start, vector<int>& outPath) -> int {
        vector<uint8_t> visited(V, 0);
        outPath.clear();
        outPath.reserve(V);
        int cur = start;
        visited[cur] = 1;
        outPath.push_back(cur);

        for (int step = 1; step < V; ++step) {
            int bestDeg = 10;
            static int candidates[8];
            int candCnt = 0;

            uint8_t nc = nbrCount[cur];
            for (uint8_t i = 0; i < nc; ++i) {
                int u = nbr[cur][i];
                if (visited[u]) continue;
                int deg = unvisited_degree(u, visited);
                if (deg < bestDeg) {
                    bestDeg = deg;
                    candCnt = 0;
                    candidates[candCnt++] = u;
                } else if (deg == bestDeg) {
                    candidates[candCnt++] = u;
                }
            }

            if (candCnt == 0) {
                // dead end
                break;
            }

            int chosen = -1;
            if (candCnt == 1) {
                chosen = candidates[0];
            } else {
                // Tie-break by second-level minimal degree among next moves
                int bestSec = 10;
                static int secCandidates[8];
                int secCnt = 0;
                for (int i = 0; i < candCnt; ++i) {
                    int u = candidates[i];
                    int secMin = 10;
                    uint8_t ncu = nbrCount[u];
                    for (uint8_t j = 0; j < ncu; ++j) {
                        int w = nbr[u][j];
                        if (visited[w]) continue;
                        int dg = unvisited_degree(w, visited);
                        if (dg < secMin) secMin = dg;
                    }
                    if (secMin < bestSec) {
                        bestSec = secMin;
                        secCnt = 0;
                        secCandidates[secCnt++] = u;
                    } else if (secMin == bestSec) {
                        secCandidates[secCnt++] = u;
                    }
                }
                if (secCnt == 1) {
                    chosen = secCandidates[0];
                } else {
                    // Further tie-break by distance from center (prefer far to avoid early traps)
                    double cr = (double)N / 2.0 - 0.5;
                    double cc = (double)N / 2.0 - 0.5;
                    auto dist2 = [&](int id) {
                        int r = id / N, c = id % N;
                        double dr = r - cr, dc = c - cc;
                        return dr*dr + dc*dc;
                    };
                    double bestScore = -1.0;
                    static int finCandidates[8];
                    int finCnt = 0;
                    for (int i = 0; i < secCnt; ++i) {
                        int u = secCandidates[i];
                        double sc = dist2(u);
                        if (sc > bestScore + 1e-12) {
                            bestScore = sc;
                            finCnt = 0;
                            finCandidates[finCnt++] = u;
                        } else if (abs(sc - bestScore) <= 1e-12) {
                            finCandidates[finCnt++] = u;
                        }
                    }
                    if (finCnt == 1) chosen = finCandidates[0];
                    else chosen = finCandidates[rng.next_size_t((size_t)finCnt)];
                }
            }

            cur = chosen;
            visited[cur] = 1;
            outPath.push_back(cur);
        }

        return (int)outPath.size();
    };

    vector<int> bestPath, curPath;
    int bestLen = 0;

    // Time-bound multiple attempts
    auto t_start = chrono::high_resolution_clock::now();
    const double TIME_LIMIT = 0.95; // seconds
    int maxAttempts = 1000000000; // effectively limited by time
    for (int it = 0; it < maxAttempts; ++it) {
        int len = attempt(r0 * N + c0, curPath);
        if (len > bestLen) {
            bestLen = len;
            bestPath = curPath;
            if (bestLen == V) break;
        }
        auto t_now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(t_now - t_start).count();
        if (elapsed > TIME_LIMIT) break;
    }

    cout << bestLen << '\n';
    for (int i = 0; i < bestLen; ++i) {
        int id = bestPath[i];
        int r = id / N;
        int c = id % N;
        cout << (r + 1) << ' ' << (c + 1);
        if (i + 1 < bestLen) cout << '\n';
    }
    return 0;
}