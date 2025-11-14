#include <bits/stdc++.h>
using namespace std;

struct SCCResult {
    int compCount;
    vector<int> compId;               // size N, component id of each node
    vector<vector<int>> components;   // nodes in each component
};

SCCResult computeSCC(int N, const vector<array<int,2>>& dest) {
    vector<vector<int>> g(N), gr(N);
    for (int u = 0; u < N; ++u) {
        int v1 = dest[u][0];
        int v2 = dest[u][1];
        g[u].push_back(v1);
        g[u].push_back(v2);
        gr[v1].push_back(u);
        gr[v2].push_back(u);
    }
    vector<int> order;
    vector<char> used(N, 0);
    function<void(int)> dfs1 = [&](int u) {
        used[u] = 1;
        for (int v : g[u]) if (!used[v]) dfs1(v);
        order.push_back(u);
    };
    for (int i = 0; i < N; ++i) if (!used[i]) dfs1(i);

    vector<int> compId(N, -1);
    int compCnt = 0;
    function<void(int)> dfs2 = [&](int u) {
        compId[u] = compCnt;
        for (int v : gr[u]) if (compId[v] == -1) dfs2(v);
    };
    for (int i = N-1; i >= 0; --i) {
        int v = order[i];
        if (compId[v] == -1) {
            dfs2(v);
            compCnt++;
        }
    }
    vector<vector<int>> comps(compCnt);
    for (int i = 0; i < N; ++i) comps[compId[i]].push_back(i);
    return {compCnt, compId, comps};
}

long long deltaChange(long long a_old, long long b_old, long long w) {
    // a_old corresponds to r2[oldDest], b_old to r2[newDest]
    long long before = llabs(a_old) + llabs(b_old);
    long long after = llabs(a_old + w) + llabs(b_old - w);
    return after - before; // negative is improvement
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, L;
    if (!(cin >> N >> L)) return 0;
    vector<int> T(N);
    for (int i = 0; i < N; ++i) cin >> T[i];

    // Initialize residuals scaled by 2: r2[i] = 2*T[i]
    vector<long long> r2(N);
    for (int i = 0; i < N; ++i) r2[i] = 2LL * T[i];

    // Prepare list of 2N halves: each with weight T[j], belonging to source j and id 0/1
    struct Half { int w, j, id; };
    vector<Half> halves;
    halves.reserve(2*N);
    for (int j = 0; j < N; ++j) {
        halves.push_back({T[j], j, 0});
        halves.push_back({T[j], j, 1});
    }

    // Shuffle, then sort by descending weight (stable-ish)
    std::mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
    shuffle(halves.begin(), halves.end(), rng);
    sort(halves.begin(), halves.end(), [](const Half& a, const Half& b){
        if (a.w != b.w) return a.w > b.w;
        if (a.j != b.j) return a.j < b.j;
        return a.id < b.id;
    });

    // dest[j][0/1] = assigned destination
    vector<array<int,2>> dest(N);
    for (int j = 0; j < N; ++j) dest[j][0] = dest[j][1] = -1;

    // Greedy assignment: for each half, assign to bin with max residual
    for (const auto& h : halves) {
        // find index with maximum r2
        long long bestVal = LLONG_MIN;
        int bestIdx = 0;
        for (int i = 0; i < N; ++i) {
            if (r2[i] > bestVal) {
                bestVal = r2[i];
                bestIdx = i;
            }
        }
        dest[h.j][h.id] = bestIdx;
        r2[bestIdx] -= h.w;
    }

    // Local improvement: move edges from overfilled bins to deficit bins
    for (int iter = 0; iter < 300; ++iter) {
        vector<int> posIdx;
        posIdx.reserve(N);
        for (int i = 0; i < N; ++i) if (r2[i] > 0) posIdx.push_back(i);
        if (posIdx.empty()) break;
        bool improved = false;
        // try to move one edge that reduces L1
        long long bestDelta = 0;
        int bestJ = -1, bestId = -1, bestNew = -1;
        for (int j = 0; j < N; ++j) {
            if (T[j] == 0) continue; // moving zero-weight has no effect
            for (int id = 0; id < 2; ++id) {
                int old = dest[j][id];
                if (r2[old] < 0) {
                    long long a_old = r2[old];
                    long long w = T[j];
                    // choose best new destination among positive residuals
                    for (int newd : posIdx) {
                        if (newd == old) continue; // no change
                        long long b_old = r2[newd];
                        long long d = deltaChange(a_old, b_old, w);
                        if (d < bestDelta) {
                            bestDelta = d;
                            bestJ = j; bestId = id; bestNew = newd;
                        }
                    }
                }
            }
        }
        if (bestJ != -1) {
            int old = dest[bestJ][bestId];
            long long w = T[bestJ];
            r2[old] += w;
            r2[bestNew] -= w;
            dest[bestJ][bestId] = bestNew;
            improved = true;
        }
        if (!improved) break;
    }

    // Ensure strong connectivity
    auto ensure_strongly_connected = [&](vector<array<int,2>>& destRef, vector<long long>& r2Ref) {
        int attempts = 0;
        while (attempts < 3) {
            SCCResult scc = computeSCC(N, destRef);
            if (scc.compCount == 1) return; // already strongly connected

            // Connect components in a cycle by rewiring one edge from each component to next component
            int K = scc.compCount;
            vector<int> compOrder(K);
            // scc.components order is topological order in Kosaraju's typical construction; we'll use as is
            iota(compOrder.begin(), compOrder.end(), 0);

            for (int idx = 0; idx < K; ++idx) {
                int fromComp = compOrder[idx];
                int toComp = compOrder[(idx + 1) % K];

                // Choose u in fromComp with minimal T[u] to minimize damage
                int u = -1;
                int minT = INT_MAX;
                for (int node : scc.components[fromComp]) {
                    if (T[node] < minT) { minT = T[node]; u = node; }
                }
                if (u == -1) continue;

                // Choose best edge (0 or 1) and destination v in toComp that minimizes L1 change
                long long bestDelta = LLONG_MAX;
                int bestEdge = 0;
                int bestV = scc.components[toComp][0];

                for (int edgeId = 0; edgeId < 2; ++edgeId) {
                    int oldDest = destRef[u][edgeId];
                    long long a_old = r2Ref[oldDest];
                    long long w = T[u];
                    // Search best v in toComp
                    for (int v : scc.components[toComp]) {
                        long long b_old = r2Ref[v];
                        long long d = deltaChange(a_old, b_old, w);
                        if (d < bestDelta) {
                            bestDelta = d;
                            bestEdge = edgeId;
                            bestV = v;
                        }
                    }
                }

                int oldDest = destRef[u][bestEdge];
                long long w = T[u];
                r2Ref[oldDest] += w;
                r2Ref[bestV] -= w;
                destRef[u][bestEdge] = bestV;
            }
            attempts++;
        }

        // Fallback: enforce a ring by rewiring one edge per node to i+1 mod N
        for (int i = 0; i < N; ++i) {
            int newDest = (i + 1) % N;
            long long w = T[i];
            int e0 = destRef[i][0], e1 = destRef[i][1];
            long long d0 = deltaChange(r2Ref[e0], r2Ref[newDest], w);
            long long d1 = deltaChange(r2Ref[e1], r2Ref[newDest], w);
            if (d0 <= d1) {
                r2Ref[e0] += w;
                r2Ref[newDest] -= w;
                destRef[i][0] = newDest;
            } else {
                r2Ref[e1] += w;
                r2Ref[newDest] -= w;
                destRef[i][1] = newDest;
            }
        }
    };

    ensure_strongly_connected(dest, r2);

    // Output a_i and b_i
    for (int i = 0; i < N; ++i) {
        int a_i = dest[i][0];
        int b_i = dest[i][1];
        if (a_i < 0) a_i = 0;
        if (b_i < 0) b_i = 0;
        cout << a_i << " " << b_i << "\n";
    }
    return 0;
}