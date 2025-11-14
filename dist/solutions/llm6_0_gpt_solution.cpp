#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    // Build adjacency
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < M; ++i) {
        int u = A[i], v = B[i];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Build a spanning tree using DFS
    vector<vector<int>> tree(N + 1);
    vector<int> vis(N + 1, 0);
    function<void(int)> dfs_tree = [&](int u) {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (!vis[v]) {
                tree[u].push_back(v);
                tree[v].push_back(u);
                dfs_tree(v);
            }
        }
    };
    if (N >= 1) dfs_tree(1);

    // Euler tour of the tree to get a walk of length 2N-1
    vector<int> walk;
    function<void(int,int)> euler = [&](int u, int p) {
        walk.push_back(u);
        for (int v : tree[u]) if (v != p) {
            euler(v, u);
            walk.push_back(u);
        }
    };
    if (N >= 1) euler(1, 0);

    // Build base row P with runs: first occurrence length 3, others length 1
    vector<int> slotcol(N + 1, -1);
    vector<int> seen(N + 1, 0);
    vector<int> P; P.reserve(4 * N - 1);
    for (int x : walk) {
        if (!seen[x]) {
            int base = (int)P.size();
            P.push_back(x);
            P.push_back(x);
            P.push_back(x);
            slotcol[x] = base + 1; // center position
            seen[x] = 1;
        } else {
            P.push_back(x);
        }
    }
    int K = (int)P.size();
    if (K == 0) K = 1; // safety

    // Assign edges to the smaller vertex
    vector<vector<int>> assign(N + 1);
    for (int i = 0; i < M; ++i) {
        int u = A[i], v = B[i];
        if (u < v) assign[u].push_back(v);
        else assign[v].push_back(u);
    }
    int smax = 0;
    for (int u = 1; u <= N; ++u) smax = max(smax, (int)assign[u].size());
    int T = smax == 0 ? 0 : 2 * smax - 1; // number of boundaries used for toggles

    // Build the grid
    vector<vector<int>> grid(K, vector<int>(K));
    // Row 0
    for (int j = 0; j < K; ++j) grid[0][j] = P[j];

    vector<int> progress(N + 1, 0);
    vector<char> atU(N + 1, 1); // current state at pivot column: 1 means color is u, 0 means color is last neighbor

    for (int i = 1; i < K; ++i) {
        grid[i] = grid[i - 1]; // start as a copy
        int r = i - 1; // boundary index between row r and r+1
        if (r < T) {
            if ((r & 1) == 0) {
                // even step: u -> next neighbor
                for (int u = 1; u <= N; ++u) {
                    if (progress[u] < (int)assign[u].size() && atU[u] && slotcol[u] != -1) {
                        int col = slotcol[u];
                        int v = assign[u][progress[u]];
                        grid[i][col] = v;
                        atU[u] = 0;
                    }
                }
            } else {
                // odd step: neighbor -> u
                for (int u = 1; u <= N; ++u) {
                    if (progress[u] < (int)assign[u].size() && !atU[u] && slotcol[u] != -1) {
                        int col = slotcol[u];
                        grid[i][col] = u;
                        atU[u] = 1;
                        progress[u]++;
                    }
                }
            }
        }
    }

    return grid;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int N, M;
        cin >> N >> M;
        vector<int> A(M), B(M);
        for (int i = 0; i < M; ++i) cin >> A[i] >> B[i];
        auto C = create_map(N, M, A, B);
        int P = (int)C.size();
        cout << P << "\n";
        for (int i = 0; i < P; ++i) {
            if (i) cout << ' ';
            cout << P;
        }
        cout << "\n\n";
        for (int i = 0; i < P; ++i) {
            for (int j = 0; j < P; ++j) {
                if (j) cout << ' ';
                cout << C[i][j];
            }
            cout << "\n";
        }
    }
    return 0;
}