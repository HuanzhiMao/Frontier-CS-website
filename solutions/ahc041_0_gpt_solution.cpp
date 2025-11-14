#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, H;
    if (!(cin >> N >> M >> H)) return 0;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    vector<vector<int>> g(N);
    vector<pair<int,int>> edges(M);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<pair<int,int>> coords(N);
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;
        coords[i] = {x, y};
    }
    
    int K = H + 1;
    
    // Prepare candidate roots
    vector<int> deg(N);
    for (int i = 0; i < N; ++i) deg[i] = (int)g[i].size();
    vector<int> ordA(N);
    iota(ordA.begin(), ordA.end(), 0);
    sort(ordA.begin(), ordA.end(), [&](int i, int j){
        if (A[i] != A[j]) return A[i] < A[j];
        return deg[i] < deg[j];
    });
    vector<int> ordDeg(N);
    iota(ordDeg.begin(), ordDeg.end(), 0);
    sort(ordDeg.begin(), ordDeg.end(), [&](int i, int j){
        if (deg[i] != deg[j]) return deg[i] < deg[j];
        return A[i] < A[j];
    });
    
    vector<int> candidates;
    vector<char> used(N, 0);
    auto add_cand = [&](int x) {
        if (!used[x]) {
            used[x] = 1;
            candidates.push_back(x);
        }
    };
    // Add some low A vertices
    int takeA = min(80, N);
    for (int i = 0; i < takeA; ++i) add_cand(ordA[i]);
    // Add some low degree vertices
    int takeD = min(40, N);
    for (int i = 0; i < takeD; ++i) add_cand(ordDeg[i]);
    // Add fixed set of deterministic pseudo-random vertices
    mt19937 rng(712367);
    uniform_int_distribution<int> distN(0, N - 1);
    int extra = 80;
    for (int i = 0; i < extra; ++i) add_cand(distN(rng));
    // Ensure index 0 present
    add_cand(0);
    
    long long bestScore = LLONG_MIN;
    vector<int> bestParent(N, -1);
    
    vector<int> dist(N), par(N);
    deque<int> q;
    
    vector<long long> W(K);
    
    for (int root : candidates) {
        // BFS
        fill(dist.begin(), dist.end(), -1);
        fill(par.begin(), par.end(), -1);
        q.clear();
        dist[root] = 0;
        q.push_back(root);
        while (!q.empty()) {
            int v = q.front(); q.pop_front();
            for (int to : g[v]) {
                if (dist[to] == -1) {
                    dist[to] = dist[v] + 1;
                    par[to] = v;
                    q.push_back(to);
                }
            }
        }
        // Accumulate W
        fill(W.begin(), W.end(), 0);
        for (int v = 0; v < N; ++v) {
            int r = dist[v] % K;
            W[r] += A[v];
        }
        // Choose best offset s
        long long bestLocalScore = LLONG_MIN;
        int bestS = 0;
        for (int s = 0; s < K; ++s) {
            long long tot = 0;
            for (int k = 0; k < K; ++k) {
                int depth = (k - s + K) % K;
                tot += W[k] * (depth + 1);
            }
            if (s != 0) {
                // Adjust because BFS root must be a root (parent = -1)
                // Predicted depth for root would be (K - s), but actual depth is 0
                tot -= 1LL * A[root] * (K - s);
            }
            if (tot > bestLocalScore) {
                bestLocalScore = tot;
                bestS = s;
            }
        }
        if (bestLocalScore > bestScore) {
            bestScore = bestLocalScore;
            // Build parent assignment
            for (int v = 0; v < N; ++v) {
                if (dist[v] % K == bestS) bestParent[v] = -1;
                else bestParent[v] = par[v];
            }
            // Ensure root has -1 parent
            bestParent[root] = -1;
        }
    }
    
    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << bestParent[i];
    }
    cout << '\n';
    return 0;
}