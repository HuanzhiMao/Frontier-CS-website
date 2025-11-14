#include <bits/stdc++.h>
using namespace std;

struct Info {
    vector<vector<int>> children;
    vector<int> depth;
    vector<long long> subtreeSum;
    vector<int> subtreeMaxDepth;
    vector<int> tin, tout;
    vector<int> roots;
};

struct Move {
    int v = -1;
    int newParent = -1;
    long long gain = 0;
};

int N, M, H;
vector<int> A;
vector<vector<int>> adj;
vector<int> parentGlobal;

const int INF_INT = 1e9;

// BFS from single source
vector<int> bfs_from(int s) {
    vector<int> dist(N, INF_INT);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        int nd = dist[v] + 1;
        for (int to : adj[v]) {
            if (dist[to] == INF_INT) {
                dist[to] = nd;
                q.push(to);
            }
        }
    }
    return dist;
}

// Multi-source BFS producing parent and depth
void multi_source_bfs(const vector<int>& centers, vector<int>& parent, vector<int>& depth) {
    parent.assign(N, -1);
    depth.assign(N, INF_INT);
    queue<int> q;
    for (int s : centers) {
        depth[s] = 0;
        parent[s] = -1;
        q.push(s);
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        int nd = depth[v] + 1;
        for (int to : adj[v]) {
            if (depth[to] > nd) {
                depth[to] = nd;
                parent[to] = v;
                q.push(to);
            }
        }
    }
}

// Compute tree info from parentGlobal
Info compute_info() {
    Info info;
    info.children.assign(N, {});
    info.depth.assign(N, 0);
    info.subtreeSum.assign(N, 0);
    info.subtreeMaxDepth.assign(N, 0);
    info.tin.assign(N, 0);
    info.tout.assign(N, 0);
    info.roots.clear();

    // Build children
    vector<int> indeg(N, 0);
    for (int v = 0; v < N; ++v) {
        int p = parentGlobal[v];
        if (p != -1) {
            info.children[p].push_back(v);
            indeg[v]++;
        }
    }
    for (int v = 0; v < N; ++v) {
        if (parentGlobal[v] == -1) info.roots.push_back(v);
    }

    // DFS to compute depth, tin/tout, subtree sums and max depth
    int timer = 0;
    function<void(int,int)> dfs = [&](int v, int d) {
        info.depth[v] = d;
        info.tin[v] = timer++;
        long long sum = A[v];
        int mxd = d;
        for (int c : info.children[v]) {
            dfs(c, d + 1);
            sum += info.subtreeSum[c];
            mxd = max(mxd, info.subtreeMaxDepth[c]);
        }
        info.subtreeSum[v] = sum;
        info.subtreeMaxDepth[v] = mxd;
        info.tout[v] = timer;
    };
    for (int r : info.roots) {
        dfs(r, 0);
    }
    return info;
}

// Check if u is in subtree of v using tin/tout
inline bool is_descendant(const Info& info, int u, int v) {
    return info.tin[v] <= info.tin[u] && info.tout[u] <= info.tout[v];
}

// Find best reparent move
Move find_best_move(const Info& info) {
    Move best;
    for (int v = 0; v < N; ++v) {
        int slack = H - info.subtreeMaxDepth[v];
        if (slack <= 0) continue;
        int dv = info.depth[v];
        for (int u : adj[v]) {
            if (is_descendant(info, u, v)) continue; // avoid cycle
            int inc = (info.depth[u] + 1) - dv;
            if (inc <= 0) continue;
            if (inc > slack) continue;
            long long gain = (long long)inc * info.subtreeSum[v];
            if (gain > best.gain) {
                best.gain = gain;
                best.v = v;
                best.newParent = u;
            }
        }
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> H;
    A.resize(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    adj.assign(N, {});
    vector<pair<int,int>> edges;
    edges.reserve(M);
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    // Read coordinates (unused)
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;
        (void)x; (void)y;
    }

    // Center selection: farthest-first until all within H
    vector<int> centers;
    vector<int> bestDist(N, INF_INT);

    // Initial center: node with minimal A
    int first = 0;
    for (int i = 1; i < N; ++i) if (A[i] < A[first]) first = i;
    centers.push_back(first);
    bestDist = bfs_from(first);

    auto maxBestDist = [&]() -> int {
        int mx = -1;
        for (int d : bestDist) mx = max(mx, d);
        return mx;
    };

    while (true) {
        int mx = maxBestDist();
        if (mx <= H) break;
        // pick node with maximal bestDist, tie by minimal A
        int pick = -1;
        int bestD = -1;
        int bestA = INT_MAX;
        for (int v = 0; v < N; ++v) {
            int d = bestDist[v];
            if (d > bestD) {
                bestD = d;
                bestA = A[v];
                pick = v;
            } else if (d == bestD) {
                if (A[v] < bestA) {
                    bestA = A[v];
                    pick = v;
                }
            }
        }
        centers.push_back(pick);
        vector<int> dist = bfs_from(pick);
        for (int v = 0; v < N; ++v) bestDist[v] = min(bestDist[v], dist[v]);
    }

    // Build initial forest by multi-source BFS
    vector<int> parent(N, -1), depth(N, INF_INT);
    multi_source_bfs(centers, parent, depth);
    parentGlobal = parent;

    // Improvement loop: greedily reparent for gain
    Info info = compute_info();
    // Limit iterations to be safe
    const int MAX_MOVES = 5000;
    int moves = 0;

    while (moves < MAX_MOVES) {
        Move mv = find_best_move(info);
        if (mv.gain <= 0) break;
        // Apply move
        parentGlobal[mv.v] = mv.newParent;
        // Recompute info
        info = compute_info();
        moves++;
    }

    // Output parent array
    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << parentGlobal[i];
    }
    cout << '\n';
    return 0;
}