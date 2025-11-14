#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t x;
    RNG(uint64_t seed=88172645463393265ull) : x(seed) {}
    inline uint64_t next() { x ^= x << 7; x ^= x >> 9; return x; }
    inline int nextInt(int l, int r) { return l + (int)(next() % (uint64_t)(r - l + 1)); }
    inline double nextDouble() { return (next() & ((1ULL<<53)-1)) / double(1ULL<<53); }
};

struct Timer {
    chrono::high_resolution_clock::time_point st;
    Timer() { reset(); }
    void reset() { st = chrono::high_resolution_clock::now(); }
    double elapsed() const {
        auto ed = chrono::high_resolution_clock::now();
        return chrono::duration<double>(ed - st).count();
    }
};

pair<long long,int> computeDistancesAndScore(const vector<vector<int>>& G, const vector<int>& roots, const vector<int>& A, vector<int>& dist) {
    int N = (int)G.size();
    const int INF = 1e9;
    dist.assign(N, INF);
    queue<int> q;
    if (!roots.empty()) {
        for (int r : roots) {
            dist[r] = 0;
            q.push(r);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            int du = dist[u] + 1;
            for (int v : G[u]) {
                if (dist[v] > du) {
                    dist[v] = du;
                    q.push(v);
                }
            }
        }
    }
    long long score = 0;
    int maxd = 0;
    for (int i = 0; i < N; ++i) {
        if (dist[i] >= INF/2) {
            // disconnected from any root (shouldn't happen for connected graph and non-empty roots)
            // set to large to ensure invalid
            return {-(1LL<<60), INF};
        }
        score += 1LL * A[i] * dist[i];
        if (dist[i] > maxd) maxd = dist[i];
    }
    return {score, maxd};
}

vector<int> computeParents(const vector<vector<int>>& G, const vector<int>& roots, vector<int>& dist) {
    int N = (int)G.size();
    const int INF = 1e9;
    dist.assign(N, INF);
    vector<int> parent(N, -1);
    queue<int> q;
    for (int r : roots) {
        dist[r] = 0;
        parent[r] = -1;
        q.push(r);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int du = dist[u] + 1;
        for (int v : G[u]) {
            if (dist[v] > du) {
                dist[v] = du;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    // For safety: if some nodes are not reached (should not happen), set them as roots.
    for (int i = 0; i < N; ++i) if (dist[i] >= INF/2) parent[i] = -1;
    return parent;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, H;
    if (!(cin >> N >> M >> H)) {
        return 0;
    }
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    vector<vector<int>> G(N);
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    vector<pair<int,int>> XY(N);
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;
        XY[i] = {x, y};
    }

    RNG rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    Timer timer;
    const double TIME_LIMIT = 1.85;

    // Initial root selection: farthest-first with tie-breaking on smallest A
    int minAidx = 0;
    for (int i = 1; i < N; ++i) if (A[i] < A[minAidx]) minAidx = i;
    vector<int> roots;
    roots.push_back(minAidx);

    vector<int> dist(N, 0);
    auto [curScore, curMaxd] = computeDistancesAndScore(G, roots, A, dist);
    while (curMaxd > H) {
        int bestNode = -1;
        int bestDist = -1;
        int bestA = INT_MAX;
        for (int i = 0; i < N; ++i) {
            if (dist[i] > bestDist) {
                bestDist = dist[i];
                bestNode = i;
                bestA = A[i];
            } else if (dist[i] == bestDist) {
                if (A[i] < bestA) {
                    bestNode = i;
                    bestA = A[i];
                }
            }
        }
        if (bestNode == -1) break; // shouldn't happen
        roots.push_back(bestNode);
        tie(curScore, curMaxd) = computeDistancesAndScore(G, roots, A, dist);
    }

    // Attempt to remove redundant roots
    {
        vector<int> order = roots;
        // shuffle order
        for (int i = (int)order.size()-1; i > 0; --i) {
            int j = rng.nextInt(0, i);
            swap(order[i], order[j]);
        }
        vector<int> tmpDist;
        for (int r : order) {
            if ((int)roots.size() <= 1) break;
            vector<int> newRoots;
            newRoots.reserve(roots.size()-1);
            for (int x : roots) if (x != r) newRoots.push_back(x);
            auto [score2, maxd2] = computeDistancesAndScore(G, newRoots, A, tmpDist);
            if (maxd2 <= H) {
                roots.swap(newRoots);
                dist.swap(tmpDist);
                curScore = score2;
                curMaxd = maxd2;
            }
        }
    }

    long long bestScore = curScore;
    vector<int> bestRoots = roots;

    // Improvement loop: try local modifications
    vector<char> isRoot(N, 0);
    for (int r : roots) isRoot[r] = 1;

    vector<int> tempDist;
    while (timer.elapsed() < TIME_LIMIT) {
        int op = rng.nextInt(0, 1); // 0: remove, 1: replace
        if (op == 0) {
            if (roots.size() <= 1) continue;
            int idx = rng.nextInt(0, (int)roots.size()-1);
            int r = roots[idx];
            vector<int> newRoots;
            newRoots.reserve(roots.size()-1);
            for (int i = 0; i < (int)roots.size(); ++i) if (i != idx) newRoots.push_back(roots[i]);
            auto [score2, maxd2] = computeDistancesAndScore(G, newRoots, A, tempDist);
            if (maxd2 <= H && score2 >= curScore) {
                // accept
                isRoot[r] = 0;
                roots.swap(newRoots);
                dist.swap(tempDist);
                curScore = score2;
                curMaxd = maxd2;
                if (curScore > bestScore) {
                    bestScore = curScore;
                    bestRoots = roots;
                }
            }
        } else {
            if (roots.empty()) continue;
            int ridx = rng.nextInt(0, (int)roots.size()-1);
            int r = roots[ridx];
            // choose candidate c with small A
            int c = -1;
            int bestA = INT_MAX;
            for (int t = 0; t < 20; ++t) {
                int cand = rng.nextInt(0, N-1);
                if (isRoot[cand]) continue;
                if (A[cand] < bestA) {
                    bestA = A[cand];
                    c = cand;
                }
            }
            if (c == -1) continue;
            vector<int> newRoots = roots;
            newRoots[ridx] = c;
            auto [score2, maxd2] = computeDistancesAndScore(G, newRoots, A, tempDist);
            if (maxd2 <= H && score2 >= curScore) {
                // accept
                isRoot[r] = 0;
                isRoot[c] = 1;
                roots.swap(newRoots);
                dist.swap(tempDist);
                curScore = score2;
                curMaxd = maxd2;
                if (curScore > bestScore) {
                    bestScore = curScore;
                    bestRoots = roots;
                }
            }
        }
    }

    // Compute final parents using bestRoots
    vector<int> finalDist;
    vector<int> parent = computeParents(G, bestRoots, finalDist);

    // Output parents
    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << parent[i];
    }
    cout << '\n';
    return 0;
}