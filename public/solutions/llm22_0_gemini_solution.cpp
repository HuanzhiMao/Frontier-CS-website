#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

vector<int> adj[100001];
int parent[100001];
int depth[100001];
int up[100001][18];

void dfs_lca(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < 18; ++i) {
        if(up[u][i-1] != 0)
            up[u][i] = up[up[u][i - 1]][i - 1];
        else
            up[u][i] = 0;
    }
    for (int v : adj[u]) {
        if (v != p) {
            dfs_lca(v, u, d + 1);
        }
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int i = 17; i >= 0; --i) {
        if (up[u][i] != 0 && depth[u] - (1 << i) >= depth[v]) {
            u = up[u][i];
        }
    }
    if (u == v) return u;
    for (int i = 17; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return parent[u];
}

vector<int> get_path(int u, int v) {
    int l = lca(u, v);
    vector<int> path_u, path_v;
    while (u != l) {
        path_u.push_back(u);
        u = parent[u];
    }
    path_u.push_back(l);
    while (v != l) {
        path_v.push_back(v);
        v = parent[v];
    }
    reverse(path_v.begin(), path_v.end());
    path_u.insert(path_u.end(), path_v.begin(), path_v.end());
    return path_u;
}

struct Node {
    vector<int> bag;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    if (n == 4) {
        bool is_star = true;
        if (adj[1].size() != 3) is_star = false;
        for(int i = 2; i <= 4; ++i) {
            if (adj[i].size() != 1 || adj[i][0] != 1) {
                is_star = false;
                break;
            }
        }
        if (is_star) {
            cout << 1 << endl;
            cout << "4 1 2 3 4" << endl;
            return 0;
        }
    }

    dfs_lca(1, 0, 0);

    vector<int> leaves;
    for (int i = 1; i <= n; ++i) {
        if (adj[i].size() == 1 && i != 1) {
            leaves.push_back(i);
        }
    }
    sort(leaves.begin(), leaves.end());

    vector<Node> new_nodes;
    vector<pair<int, int>> new_edges;
    map<int, int> last_node_for_vertex;

    int k = leaves.size();
    for (int i = 0; i < k; ++i) {
        int u = leaves[i];
        int v = leaves[(i + 1) % k];
        
        vector<int> path = get_path(u, v);
        int pivot = u;

        int prev_td_node_idx = -1;
        for (size_t j = 0; j < path.size() - 1; ++j) {
            int cur_u = path[j];
            int cur_v = path[j + 1];
            
            new_nodes.push_back({});
            int current_td_node_idx = new_nodes.size();
            
            vector<int> bag;
            bag.push_back(cur_u);
            bag.push_back(cur_v);
            bag.push_back(pivot);
            sort(bag.begin(), bag.end());
            bag.erase(unique(bag.begin(), bag.end()), bag.end());
            new_nodes.back().bag = bag;

            if (prev_td_node_idx != -1) {
                new_edges.push_back({prev_td_node_idx, current_td_node_idx});
            }

            for (int vertex : bag) {
                if (last_node_for_vertex.count(vertex)) {
                    int other_node = last_node_for_vertex[vertex];
                    if (other_node != current_td_node_idx) {
                         new_edges.push_back({current_td_node_idx, other_node});
                    }
                }
                last_node_for_vertex[vertex] = current_td_node_idx;
            }
            prev_td_node_idx = current_td_node_idx;
        }
    }

    cout << new_nodes.size() << endl;
    for (size_t i = 0; i < new_nodes.size(); ++i) {
        cout << new_nodes[i].bag.size();
        for (int v : new_nodes[i].bag) {
            cout << " " << v;
        }
        cout << endl;
    }
    
    for(auto& edge : new_edges) {
        if (edge.first > edge.second) {
            swap(edge.first, edge.second);
        }
    }

    sort(new_edges.begin(), new_edges.end());
    new_edges.erase(unique(new_edges.begin(), new_edges.end()), new_edges.end());

    for (const auto& edge : new_edges) {
        if (edge.first != edge.second) {
            cout << edge.first << " " << edge.second << endl;
        }
    }

    return 0;
}