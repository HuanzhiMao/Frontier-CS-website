#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t x;
    RNG(uint64_t s = 88172645463393265ull) { if (s == 0) s = 88172645463393265ull; x = s; }
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int randint(int n) { return int(next() % n); }
    inline uint64_t rand64() { return next(); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    int r0, c0;
    cin >> r0 >> c0;
    --r0; --c0;

    int64_t NN = 1LL * N * N;
    int target = (N % 2 == 0 ? (int)NN : (((r0 + c0) & 1) == 0 ? (int)NN : (int)NN - 1));

    // Precompute adjacency
    const int dr[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    const int dc[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int totalV = N * N;
    vector<vector<int>> adj(totalV);
    adj.reserve(totalV);
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int u = r * N + c;
            auto &vec = adj[u];
            vec.reserve(8);
            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    vec.push_back(nr * N + nc);
                }
            }
        }
    }
    vector<int> degInit(totalV);
    for (int i = 0; i < totalV; ++i) degInit[i] = (int)adj[i].size();

    RNG rng(chrono::high_resolution_clock::now().time_since_epoch().count());

    auto attempt = [&](vector<int> &outPath)->int {
        outPath.clear();
        outPath.reserve(totalV);
        vector<char> vis(totalV, 0);
        vector<int> deg = degInit;

        int u = r0 * N + c0;
        vis[u] = 1;
        outPath.push_back(u);
        for (int w : adj[u]) if (!vis[w]) --deg[w];

        while ((int)outPath.size() < target) {
            int best = -1;
            int bestDeg = INT_MAX;
            int bestDeg2 = INT_MAX;
            uint64_t bestRand = 0;

            // Collect candidates
            const auto &cand = adj[u];
            for (int v : cand) {
                if (vis[v]) continue;
                int d = deg[v];
                if (d < bestDeg) {
                    // compute secondary
                    int d2 = INT_MAX;
                    for (int t : adj[v]) if (!vis[t]) d2 = min(d2, deg[t]);
                    bestDeg = d;
                    bestDeg2 = d2 == INT_MAX ? 0 : d2;
                    best = v;
                    bestRand = rng.rand64();
                } else if (d == bestDeg) {
                    int d2 = INT_MAX;
                    for (int t : adj[v]) if (!vis[t]) d2 = min(d2, deg[t]);
                    int dd2 = d2 == INT_MAX ? 0 : d2;
                    if (dd2 < bestDeg2) {
                        bestDeg2 = dd2;
                        best = v;
                        bestRand = rng.rand64();
                    } else if (dd2 == bestDeg2) {
                        uint64_t rkey = rng.rand64();
                        if (rkey < bestRand) { // random tie-break
                            best = v;
                            bestRand = rkey;
                        }
                    }
                }
            }
            if (best == -1) break; // no move
            u = best;
            vis[u] = 1;
            outPath.push_back(u);
            for (int w : adj[u]) if (!vis[w]) --deg[w];
        }

        // If target is full board and we haven't reached it, try to continue greedily anyway
        // (for odd wrong parity, target=N^2-1, loop ends once we hit target)
        while ((int)outPath.size() < totalV) {
            int best = -1;
            int bestDeg = INT_MAX;
            int bestDeg2 = INT_MAX;
            uint64_t bestRand = 0;

            const auto &cand = adj[u];
            for (int v : cand) {
                if (vis[v]) continue;
                int d = deg[v];
                if (d < bestDeg) {
                    int d2 = INT_MAX;
                    for (int t : adj[v]) if (!vis[t]) d2 = min(d2, deg[t]);
                    bestDeg = d;
                    bestDeg2 = d2 == INT_MAX ? 0 : d2;
                    best = v;
                    bestRand = rng.rand64();
                } else if (d == bestDeg) {
                    int d2 = INT_MAX;
                    for (int t : adj[v]) if (!vis[t]) d2 = min(d2, deg[t]);
                    int dd2 = d2 == INT_MAX ? 0 : d2;
                    if (dd2 < bestDeg2) {
                        bestDeg2 = dd2;
                        best = v;
                        bestRand = rng.rand64();
                    } else if (dd2 == bestDeg2) {
                        uint64_t rkey = rng.rand64();
                        if (rkey < bestRand) {
                            best = v;
                            bestRand = rkey;
                        }
                    }
                }
            }
            if (best == -1) break;
            u = best;
            vis[u] = 1;
            outPath.push_back(u);
            for (int w : adj[u]) if (!vis[w]) --deg[w];
        }

        return (int)outPath.size();
    };

    vector<int> bestPath;
    bestPath.reserve(totalV);
    int bestLen = 0;

    auto startTime = chrono::high_resolution_clock::now();
    const double timeLimit = 0.95; // seconds
    int maxTries = 2000; // plenty; time-limited will stop
    for (int tries = 0; tries < maxTries; ++tries) {
        vector<int> curPath;
        int len = attempt(curPath);
        if (len > bestLen) {
            bestLen = len;
            bestPath.swap(curPath);
            if (bestLen >= target) break;
        }
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(now - startTime).count();
        if (elapsed > timeLimit) break;
    }

    // Output
    cout << bestLen << '\n';
    for (int i = 0; i < bestLen; ++i) {
        int id = bestPath[i];
        int rr = id / N;
        int cc = id % N;
        cout << (rr + 1) << ' ' << (cc + 1);
        if (i + 1 < bestLen) cout << '\n';
    }
    return 0;
}