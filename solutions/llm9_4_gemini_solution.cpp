#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

const int MAXN = 1005;

vector<pair<int, int>> adj[MAXN];
int p[MAXN];
int n;

int parent[MAXN];
int depth[MAXN];
int dist[MAXN][MAXN];

struct EdgeInfo {
    int u, v, id;
};
vector<EdgeInfo> edge_list;

void bfs_dist_from(int start_node) {
    for (int i = 1; i <= n; ++i) {
        dist[start_node][i] = -1;
    }
    vector<int> q;
    q.push_back(start_node);
    dist[start_node][start_node] = 0;
    int head = 0;
    while(head < (int)q.size()){
        int u = q[head++];
        for(auto& edge : adj[u]){
            int v = edge.first;
            if(dist[start_node][v] == -1){
                dist[start_node][v] = dist[start_node][u] + 1;
                q.push_back(v);
            }
        }
    }
}

void build_dist_matrix() {
    for(int i = 1; i <= n; ++i) {
        bfs_dist_from(i);
    }
}

void dfs_tree_data(int u, int p_node, int d) {
    parent[u] = p_node;
    depth[u] = d;
    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (v != p_node) {
            dfs_tree_data(v, u, d + 1);
        }
    }
}

bool is_sorted() {
    for (int i = 1; i <= n; ++i) {
        if (p[i] != i) {
            return false;
        }
    }
    return true;
}

void solve() {
    cin >> n;
    edge_list.clear();
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }

    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
    }

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        edge_list.push_back({u, v, i});
    }
    
    build_dist_matrix();
    dfs_tree_data(1, 0, 0);

    vector<vector<int>> operations;

    for (int op_count = 0; op_count < 3 * n + 10 && !is_sorted(); ++op_count) {
        for (int phase = 0; phase < 2; ++phase) {
            if (is_sorted()) break;

            vector<int> current_matching_edge_ids;
            vector<bool> used_vertices(n + 1, false);
            
            for (int u = 1; u <= n; ++u) {
                if (parent[u] == 0 || depth[u] % 2 != phase) {
                    continue;
                }
                int v = parent[u];

                if (used_vertices[u] || used_vertices[v]) {
                    continue;
                }

                int val_u = p[u];
                int val_v = p[v];

                long long current_dist_sum = (long long)dist[u][val_u] + dist[v][val_v];
                long long swapped_dist_sum = (long long)dist[v][val_u] + dist[u][val_v];
                
                bool should_swap = false;
                if (swapped_dist_sum < current_dist_sum) {
                    should_swap = true;
                } else if (swapped_dist_sum == current_dist_sum) {
                    if (u < v) { // Simple deterministic tie-break
                        should_swap = true;
                    }
                }

                if (should_swap) {
                    int edge_id = -1;
                    for(auto& edge: adj[u]){
                        if(edge.first == v){
                            edge_id = edge.second;
                            break;
                        }
                    }
                    current_matching_edge_ids.push_back(edge_id);
                    used_vertices[u] = true;
                    used_vertices[v] = true;
                }
            }

            if (!current_matching_edge_ids.empty()) {
                operations.push_back(current_matching_edge_ids);
                for (int edge_id : current_matching_edge_ids) {
                    int u_edge = edge_list[edge_id - 1].u;
                    int v_edge = edge_list[edge_id - 1].v;
                    swap(p[u_edge], p[v_edge]);
                }
            }
        }
    }
    
    cout << operations.size() << endl;
    for (auto& op : operations) {
        cout << op.size();
        sort(op.begin(), op.end());
        for (int edge_id : op) {
            cout << " " << edge_id;
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