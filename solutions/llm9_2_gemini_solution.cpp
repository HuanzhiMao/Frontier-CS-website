#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

vector<int> adj[1005];
int p[1005];
int pos[1005];
map<pair<int, int>, int> edge_map;

// BFS from a start_node to compute parents for path reconstruction
void bfs_path(int start_node, int n, int* parent, vector<int>& q) {
    for (int i = 1; i <= n; ++i) {
        parent[i] = -1;
    }
    parent[start_node] = start_node;
    q.clear();
    q.push_back(start_node);

    int head = 0;
    while(head < q.size()){
        int u = q[head++];
        for (int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                q.push_back(v);
            }
        }
    }
}

int next_hops[1005][1005];

void solve() {
    int n;
    cin >> n;

    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    edge_map.clear();

    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
    }

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_map[{u, v}] = i;
    }
    
    // Precomputation of next hops
    int parents[1005];
    vector<int> q;
    for (int i = 1; i <= n; ++i) {
        bfs_path(i, n, parents, q);
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            int curr = j;
            while (parents[curr] != i) {
                curr = parents[curr];
            }
            next_hops[i][j] = curr;
        }
    }
    
    for(int i = 1; i <= n; ++i) {
        pos[p[i]] = i;
    }

    vector<vector<int>> operations;

    while (true) {
        bool sorted = true;
        for (int i = 1; i <= n; ++i) {
            if (p[i] != i) {
                sorted = false;
                break;
            }
        }
        if (sorted) break;

        vector<pair<int, int>> desired_edges;
        for (int val = 1; val <= n; ++val) {
            if (pos[val] != val) {
                int u = pos[val];
                int v = next_hops[u][val];
                if (u > v) swap(u, v);
                desired_edges.push_back({u, v});
            }
        }

        sort(desired_edges.begin(), desired_edges.end());
        desired_edges.erase(unique(desired_edges.begin(), desired_edges.end()), desired_edges.end());

        vector<pair<int, int>> matching;
        vector<bool> matched(n + 1, false);

        for (auto const& edge : desired_edges) {
            int u = edge.first;
            int v = edge.second;
            if (!matched[u] && !matched[v]) {
                matching.push_back({u, v});
                matched[u] = true;
                matched[v] = true;
            }
        }

        vector<int> current_op;
        if(matching.empty() && !desired_edges.empty()){
             matching.push_back(desired_edges[0]);
        }

        for (auto const& edge : matching) {
            current_op.push_back(edge_map[edge]);
            int u = edge.first;
            int v = edge.second;
            
            int val_u = p[u];
            int val_v = p[v];
            swap(p[u], p[v]);
            pos[val_u] = v;
            pos[val_v] = u;
        }
        operations.push_back(current_op);
    }

    cout << operations.size() << endl;
    for (const auto& op : operations) {
        cout << op.size();
        for (int edge_idx : op) {
            cout << " " << edge_idx;
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}