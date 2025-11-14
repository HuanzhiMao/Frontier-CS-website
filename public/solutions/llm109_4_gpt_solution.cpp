#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t x;
    RNG(uint64_t seed=88172645463393265ull) : x(seed) {}
    inline uint32_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return (uint32_t)x;
    }
    inline uint32_t next(uint32_t mod) {
        return next() % mod;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    int r0, c0;
    cin >> r0 >> c0;
    r0--; c0--; // zero-index
    
    int64_t total = 1LL * N * N;
    bool odd = (N & 1);
    bool startMinority = false;
    if (odd) {
        // For 1-indexed, even (r+c) has one extra on odd boards
        startMinority = (( ( (r0+1) + (c0+1) ) & 1) == 1);
    }
    int64_t targetLen = total - (startMinority ? 1 : 0);
    
    const int dr[8] = {2,2,1,1,-1,-1,-2,-2};
    const int dc[8] = {1,-1,2,-2,2,-2,1,-1};
    
    int tot = N*N;
    // Precompute neighbors
    vector<array<int,8>> neigh(tot);
    vector<uint8_t> ndeg0(tot, 0);
    for (int r = 0; r < N; ++r) {
        for(int c = 0; c < N; ++c) {
            int id = r * N + c;
            uint8_t cnt = 0;
            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    neigh[id][cnt++] = nr * N + nc;
                }
            }
            ndeg0[id] = cnt;
        }
    }
    
    vector<uint8_t> deg(tot);
    vector<uint8_t> visited(tot);
    vector<int> path;
    path.reserve(tot);
    vector<int> bestPath;
    int bestLen = 0;
    
    // RNG seed deterministic based on N, r0, c0
    uint64_t seed = (uint64_t)N * 1469598103934665603ull ^ ((uint64_t)(r0+1) << 32) ^ (uint64_t)(c0+1);
    seed ^= 0x9E3779B97F4A7C15ull;
    RNG rng(seed);
    
    auto attempt = [&](int maxSteps, bool shuffleMoves) {
        // reset
        memset(visited.data(), 0, visited.size()*sizeof(uint8_t));
        memcpy(deg.data(), ndeg0.data(), deg.size()*sizeof(uint8_t));
        path.clear();
        
        int cur = r0 * N + c0;
        path.push_back(cur);
        visited[cur] = 1;
        // Update degrees of neighbors of starting node
        for (int i = 0, cnt = ndeg0[cur]; i < cnt; ++i) {
            int w = neigh[cur][i];
            if (!visited[w]) deg[w]--;
        }
        
        for (int step = 1; step < maxSteps; ++step) {
            int cand = -1;
            int bestKey1 = INT_MAX;
            int bestKey2 = INT_MAX;
            int bestKey3 = INT_MAX;
            // gather candidates
            int candList[8];
            int candCnt = 0;
            for (int i = 0, cnt = ndeg0[cur]; i < cnt; ++i) {
                int v = neigh[cur][i];
                if (!visited[v]) candList[candCnt++] = v;
            }
            if (candCnt == 0) break;
            if (shuffleMoves && candCnt > 1) {
                // simple shuffle
                for (int i = candCnt - 1; i > 0; --i) {
                    int j = rng.next(i+1);
                    swap(candList[i], candList[j]);
                }
            }
            for (int idx = 0; idx < candCnt; ++idx) {
                int v = candList[idx];
                int k1 = deg[v]; // primary: Warnsdorff
                // secondary: sum of degrees of neighbors (or min degree of neighbors)
                int minN = 9;
                int sumN = 0;
                int cntN = 0;
                for (int j = 0, cc = ndeg0[v]; j < cc; ++j) {
                    int w = neigh[v][j];
                    if (!visited[w]) {
                        ++cntN;
                        int dv = deg[w];
                        if (dv < minN) minN = dv;
                        sumN += dv;
                    }
                }
                if (cntN == 0) { // Terminal after moving to v
                    minN = 9;
                    sumN = 0;
                }
                int k2 = minN; // lower is better
                int k3 = sumN; // lower is better
                // slight random tie-break
                int rnd = (int)(rng.next() & 0x1FF); // small
                // Prefer not to move to dead-end too early
                // if k1 <= 1 and step < maxSteps-1, add mild penalty
                if (k1 <= 1 && step < maxSteps - 1) {
                    // penalty reduces preference
                    k1 += 10;
                }
                // combine tie-breaks
                bool better = false;
                if (k1 < bestKey1) better = true;
                else if (k1 == bestKey1) {
                    if (k2 < bestKey2) better = true;
                    else if (k2 == bestKey2) {
                        if (k3 + rnd < bestKey3) better = true;
                    }
                }
                if (better) {
                    bestKey1 = k1;
                    bestKey2 = k2;
                    bestKey3 = k3 + rnd;
                    cand = v;
                }
            }
            if (cand == -1) break;
            // move
            cur = cand;
            path.push_back(cur);
            visited[cur] = 1;
            for (int i = 0, cnt = ndeg0[cur]; i < cnt; ++i) {
                int w = neigh[cur][i];
                if (!visited[w]) deg[w]--;
            }
        }
        return (int)path.size();
    };
    
    // attempts with varying strategies
    const int64_t target = targetLen;
    int maxAttempts = 2000; // will break on time
    auto beginTime = chrono::steady_clock::now();
    for (int attemptId = 0; attemptId < maxAttempts; ++attemptId) {
        bool shuffle = (attemptId % 3 != 0);
        int got = attempt((int)target, shuffle);
        if (got > bestLen) {
            bestLen = got;
            bestPath = path;
            if (bestLen == target) break;
        }
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - beginTime).count();
        if (elapsed > 0.95) break;
    }
    // If even N, try to push further by letting target be total if we didn't already reach
    if (!odd && bestLen < total) {
        // try a few more attempts aimed at full tour
        for (int attemptId = 0; attemptId < 800; ++attemptId) {
            int got = attempt((int)total, true);
            if (got > bestLen) {
                bestLen = got;
                bestPath = path;
                if (bestLen == total) break;
            }
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(now - beginTime).count();
            if (elapsed > 0.98) break;
        }
    }
    
    // Output best found
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