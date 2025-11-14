#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

int n;
vector<vector<int>> C;
vector<vector<int>> adj[2];
vector<int> p_sol;

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

bool get_path_from_start(const vector<int>& nodes, int color, int start_node, vector<int>& path, const vector<bool>& in_nodes) {
    if (nodes.empty()) return true;
    path.clear();
    vector<bool> visited(n + 1, false);
    
    int curr = start_node;
    for (size_t i = 0; i < nodes.size(); ++i) {
        path.push_back(curr);
        visited[curr] = true;
        int next_node = -1;
        for (int neighbor : adj[color][curr]) {
            if (in_nodes[neighbor] && !visited[neighbor]) {
                next_node = neighbor;
                break;
            }
        }
        if (next_node != -1) {
            curr = next_node;
        } else if (i < nodes.size() - 1) {
            return false;
        }
    }
    return path.size() == nodes.size();
}

bool check_is_path(const vector<int>& nodes, int color, vector<int>& endpoints) {
    if (nodes.empty()) return true;
    
    vector<bool> in_nodes(n + 1, false);
    for(int node : nodes) in_nodes[node] = true;

    if (nodes.size() == 1) {
        endpoints = {nodes[0], nodes[0]};
        return true;
    }

    endpoints.clear();
    
    int deg1_count = 0;
    int deg2_count = 0;

    for (int u : nodes) {
        int degree = 0;
        for (int v : adj[color][u]) {
            if (in_nodes[v]) {
                degree++;
            }
        }
        if (degree == 1) {
            deg1_count++;
            endpoints.push_back(u);
        } else if (degree == 2) {
            deg2_count++;
        } else {
            return false;
        }
    }

    if (deg1_count != 2 || deg2_count != (int)nodes.size() - 2) {
        return false;
    }

    vector<int> path;
    return get_path_from_start(nodes, color, endpoints[0], path, in_nodes);
}

void update_solution(const vector<int>& p) {
    if (p.empty() || p.size() != (size_t)n) return;
    if (p_sol.empty() || p < p_sol) {
        p_sol = p;
    }
}

void solve() {
    adj[0].assign(n + 1, {});
    adj[1].assign(n + 1, {});
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            adj[C[i][j]][i].push_back(j);
            adj[C[i][j]][j].push_back(i);
        }
    }

    p_sol.clear();

    vector<int> all_nodes(n);
    iota(all_nodes.begin(), all_nodes.end(), 1);

    for (int c = 0; c < 2; ++c) {
        vector<int> endpoints;
        if (check_is_path(all_nodes, c, endpoints)) {
            vector<bool> in_all_nodes(n + 1, true);
            vector<int> path;
            get_path_from_start(all_nodes, c, endpoints[0], path, in_all_nodes);
            update_solution(path);
            reverse(path.begin(), path.end());
            update_solution(path);
        }
    }
    
    vector<DSU> dsu(2, DSU(n));
    vector<vector<vector<int>>> components(2, vector<vector<int>>(n + 1));
    vector<int> comp_root[2];
    
    for (int c = 0; c < 2; ++c) {
        for (int i = 1; i <= n; ++i) {
            for (int neighbor : adj[c][i]) {
                if(i < neighbor) dsu[c].unite(i, neighbor);
            }
        }
        for (int i = 1; i <= n; ++i) {
            components[c][dsu[c].find(i)].push_back(i);
        }
        for(int i=1; i<=n; ++i) {
            if(!components[c][i].empty()) comp_root[c].push_back(i);
        }
    }
    
    vector<bool> comp_is_path[2];
    vector<vector<int>> comp_endpoints[2];
    comp_is_path[0].resize(n+1, false); comp_is_path[1].resize(n+1, false);
    comp_endpoints[0].resize(n+1); comp_endpoints[1].resize(n+1);

    for(int c=0; c<2; ++c) {
        for(int root : comp_root[c]) {
            comp_is_path[c][root] = check_is_path(components[c][root], c, comp_endpoints[c][root]);
        }
    }

    for (int c1 = 0; c1 < 2; ++c1) {
        int c2 = 1 - c1;
        for (int v = 1; v <= n; ++v) {
            int root1 = dsu[c1].find(v);
            int root2 = dsu[c2].find(v);
            
            const auto& U = components[c1][root1];
            const auto& W = components[c2][root2];

            if (U.size() + W.size() - 1 != (size_t)n) continue;

            if (comp_is_path[c1][root1] && comp_is_path[c2][root2]) {
                 bool v_is_endpoint1 = false;
                 for(int ep : comp_endpoints[c1][root1]) if(ep == v) v_is_endpoint1 = true;
                 if(!v_is_endpoint1) continue;

                 bool v_is_endpoint2 = false;
                 for(int ep : comp_endpoints[c2][root2]) if(ep == v) v_is_endpoint2 = true;
                 if(!v_is_endpoint2) continue;
                 
                bool check_intersect = true;
                vector<bool> in_W(n+1, false);
                for(int node : W) in_W[node] = true;
                for(int node : U){
                    if(node != v && in_W[node]){
                        check_intersect = false;
                        break;
                    }
                }
                if(!check_intersect) continue;

                vector<int> path1, path2;
                vector<bool> in_U(n+1, false); for(int node : U) in_U[node] = true;
                get_path_from_start(U, c1, v, path1, in_U);
                
                vector<bool> in_W_bool(n+1, false); for(int node : W) in_W_bool[node] = true;
                get_path_from_start(W, c2, v, path2, in_W_bool);

                vector<int> p;
                
                p = path1;
                reverse(p.begin(), p.end());
                p.insert(p.end(), path2.begin() + 1, path2.end());
                update_solution(p);

                p = path2;
                reverse(p.begin(), p.end());
                p.insert(p.end(), path1.begin() + 1, path1.end());
                update_solution(p);
            }
        }
    }

    if (p_sol.empty()) {
        cout << -1 << endl;
    } else {
        for (int i = 0; i < n; ++i) {
            cout << p_sol[i] << (i == n - 1 ? "" : " ");
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string line;
    while (cin >> n) {
        C.assign(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; ++i) {
            cin >> line;
            for (int j = 1; j <= n; ++j) {
                C[i][j] = line[j - 1] - '0';
            }
        }
        solve();
    }
    return 0;
}