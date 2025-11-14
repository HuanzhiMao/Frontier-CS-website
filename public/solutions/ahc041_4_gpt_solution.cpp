#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, H;
    if (!(cin >> N >> M >> H)) return 0;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    vector<pair<int,int>> edges(M);
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> X(N), Y(N);
    for (int i = 0; i < N; ++i) cin >> X[i] >> Y[i];

    // Sort adjacency by descending squared distance to favor longer DFS paths
    for (int v = 0; v < N; ++v) {
        sort(adj[v].begin(), adj[v].end(), [&](int a, int b){
            long long dx1 = X[a] - X[v], dy1 = Y[a] - Y[v];
            long long dx2 = X[b] - X[v], dy2 = Y[b] - Y[v];
            long long d1 = dx1*dx1 + dy1*dy1;
            long long d2 = dx2*dx2 + dy2*dy2;
            if (d1 != d2) return d1 > d2;
            return a < b;
        });
    }

    // Choose root as node with minimal A to waste least weight at depth 0
    int root = min_element(A.begin(), A.end()) - A.begin();

    // Build DFS spanning tree
    vector<int> parent(N, -1);
    vector<vector<int>> children(N);
    vector<int> order; order.reserve(N);
    vector<char> vis(N, 0);

    struct St { int v, it; };
    vector<St> st;
    st.push_back({root, 0});
    vis[root] = 1;
    while (!st.empty()) {
        auto &top = st.back();
        int v = top.v;
        if (top.it < (int)adj[v].size()) {
            int to = adj[v][top.it++];
            if (!vis[to]) {
                vis[to] = 1;
                parent[to] = v;
                children[v].push_back(to);
                st.push_back({to, 0});
            }
        } else {
            order.push_back(v);
            st.pop_back();
        }
    }
    // In case the graph is not fully connected due to some reason (shouldn't happen), connect remaining
    for (int i = 0; i < N; ++i) if (!vis[i]) {
        // connect new component
        st.clear();
        st.push_back({i, 0});
        vis[i] = 1;
        parent[i] = -1;
        while (!st.empty()) {
            auto &top = st.back();
            int v = top.v;
            if (top.it < (int)adj[v].size()) {
                int to = adj[v][top.it++];
                if (!vis[to]) {
                    vis[to] = 1;
                    parent[to] = v;
                    children[v].push_back(to);
                    st.push_back({to, 0});
                }
            } else {
                order.push_back(v);
                st.pop_back();
            }
        }
    }

    // DP arrays
    int Hmax = H;
    vector<array<long long, 16>> dp(N); // H<=10, allocate a bit more
    vector<array<char, 16>> cutChoice(N); // 1 if cut at v for given d
    vector<long long> cutVal(N, 0);

    // Postorder DP
    for (int idx = 0; idx < N; ++idx) {
        int v = order[idx]; // order is in postorder because we pushed on pop
        // But we built order on pop; that's postorder. Good.
    }
    // However, our order currently has root last visited first? We pushed on pop, so order is postorder (children before parent).
    // Let's process in the given 'order' as constructed (children first).
    for (int idx = 0; idx < N; ++idx) {
        int v = order[idx];
        // compute cutVal[v] = sum dp[child][1]
        long long cv = 0;
        for (int c : children[v]) {
            cv += dp[c][1];
        }
        cutVal[v] = cv;
        for (int d = 0; d <= 15; ++d) { cutChoice[v][d] = 1; dp[v][d] = cv; } // init to cut
        // For d in 1..H, compute option not cut
        for (int d = 1; d <= Hmax; ++d) {
            long long noCut = 1LL * A[v] * d;
            for (int c : children[v]) {
                int nd = d + 1;
                if (nd <= Hmax) noCut += dp[c][nd];
                else noCut += cutVal[c]; // forced cut at child
            }
            if (noCut > cv) {
                dp[v][d] = noCut;
                cutChoice[v][d] = 0; // not cut at v
            } else {
                dp[v][d] = cv;
                cutChoice[v][d] = 1; // cut at v
            }
        }
        // For d=0 we won't use it, but keep as cut (already set).
    }

    // Reconstruct parent choices
    vector<int> resParent = parent;
    // Ensure root(s) are roots: cut at chosen root
    resParent[root] = -1;

    // recursive reconstruction
    // Use iterative stack to avoid recursion depth issues if extremely deep
    struct Frame {
        int v;
        int d;
    };
    vector<Frame> stack2;
    // push children of root with d=1
    for (int c : children[root]) {
        stack2.push_back({c, 1});
        resParent[c] = root; // temporarily; will be overwritten if cut
    }
    // For any other components (if any), enforce their roots too
    for (int v = 0; v < N; ++v) {
        if (parent[v] == -1 && v != root) {
            resParent[v] = -1;
            for (int c : children[v]) {
                stack2.push_back({c, 1});
                resParent[c] = v;
            }
        }
    }

    while (!stack2.empty()) {
        auto fr = stack2.back(); stack2.pop_back();
        int v = fr.v;
        int d = fr.d;
        if (d > Hmax) {
            // forced cut at v
            resParent[v] = -1;
            for (int c : children[v]) {
                // child param becomes 1
                if (resParent[c] != v) resParent[c] = v;
                stack2.push_back({c, 1});
            }
            continue;
        }
        bool cut = cutChoice[v][d];
        if (cut) {
            resParent[v] = -1;
            for (int c : children[v]) {
                if (resParent[c] != v) resParent[c] = v;
                stack2.push_back({c, 1});
            }
        } else {
            // not cut: keep tree parent
            // resParent[v] is already parent[v] from construction
            for (int c : children[v]) {
                if (resParent[c] != v) resParent[c] = v;
                stack2.push_back({c, d + 1});
            }
        }
    }

    // Output
    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << resParent[i];
    }
    cout << '\n';

    return 0;
}