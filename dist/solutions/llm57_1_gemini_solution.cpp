#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>

using namespace std;

vector<int> adj[1005];
int n;

void bfs_parents(int root, vector<int>& parent) {
    parent.assign(n + 1, 0);
    vector<int> q;
    
    q.push_back(root);
    parent[root] = root;
    
    int head = 0;
    while(head < q.size()) {
        int u = q[head++];
        for(int v : adj[u]) {
            if (parent[v] == 0) {
                parent[v] = u;
                q.push_back(v);
            }
        }
    }
}

void color_bfs(int start_node, vector<int>& colors) {
    colors.assign(n + 1, -1);
    vector<int> q;

    q.push_back(start_node);
    colors[start_node] = 0;

    int head = 0;
    while(head < q.size()) {
        int u = q[head++];
        for (int v : adj[u]) {
            if (colors[v] == -1) {
                colors[v] = 1 - colors[u];
                q.push_back(v);
            }
        }
    }
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> f_vals(n + 1);
    for (int i = 1; i <= n; ++i) {
        cout << "? 1 1 " << i << endl;
        cin >> f_vals[i];
    }

    vector<int> colors;
    color_bfs(1, colors);

    vector<int> s_even_nodes;
    
    // f(u) = sum of values on path from root to u
    // f(u) mod 2 = (depth(u) + 1) mod 2
    // So, f(u) and depth(u) have different parity.
    // depth(1) is even if f(1) is odd.
    bool depth1_is_even = (((f_vals[1] % 2) + 2) % 2) == 1;

    // For any node u, depth(u) = dist(R, u).
    // In a bipartite graph, dist(R, u) mod 2 = (dist(R, 1) + dist(1, u)) mod 2.
    // depth(u) mod 2 = (depth(1) mod 2 + colors[u]) mod 2.
    //
    // A node u has even depth if depth(u) mod 2 == 0.
    // (depth(1) mod 2 + colors[u]) mod 2 == 0
    //
    // If depth(1) is even: (0 + colors[u]) mod 2 == 0 -> colors[u] is 0.
    // If depth(1) is odd: (1 + colors[u]) mod 2 == 0 -> colors[u] is 1.
    for (int i = 1; i <= n; ++i) {
        bool has_even_depth;
        if (depth1_is_even) {
            has_even_depth = (colors[i] == 0);
        } else {
            has_even_depth = (colors[i] == 1);
        }
        if (has_even_depth) {
            s_even_nodes.push_back(i);
        }
    }

    // The root R must have depth 0, so R is in S_even.
    for (int r : s_even_nodes) {
        vector<int> parent;
        bfs_parents(r, parent);
        
        vector<int> val(n + 1);
        bool ok = true;
        
        val[r] = f_vals[r];
        if (val[r] != 1 && val[r] != -1) {
            continue;
        }

        for (int i = 1; i <= n; ++i) {
            if (i == r) continue;
            val[i] = f_vals[i] - f_vals[parent[i]];
            if (val[i] != 1 && val[i] != -1) {
                ok = false;
                break;
            }
        }
        
        if (ok) {
            cout << "! ";
            for (int i = 1; i <= n; ++i) {
                cout << val[i] << (i == n ? "" : " ");
            }
            cout << endl;
            return;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}