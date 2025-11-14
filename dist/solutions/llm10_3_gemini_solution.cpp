#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

map<pair<int, int>, long long> query_cache;

long long query(int u, int v) {
    if (u == v) return 0;
    if (u > v) swap(u, v);
    if (query_cache.count({u, v})) {
        return query_cache[{u, v}];
    }
    cout << "? " << u << " " << v << endl;
    long long d;
    cin >> d;
    return query_cache[{u, v}] = d;
}

struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n;
    cin >> n;
    query_cache.clear();

    if (n == 1) {
        cout << "!" << endl;
        return;
    }

    int u = 1;
    long long max_dist1 = -1;
    for (int i = 2; i <= n; ++i) {
        long long d = query(1, i);
        if (d > max_dist1) {
            max_dist1 = d;
            u = i;
        }
    }

    vector<long long> dist_u(n + 1);
    int v = -1;
    long long max_dist_u = -1;
    for (int i = 1; i <= n; ++i) {
        if (i == u) continue;
        dist_u[i] = query(u, i);
        if (dist_u[i] > max_dist_u) {
            max_dist_u = dist_u[i];
            v = i;
        }
    }
    dist_u[u] = 0;

    vector<long long> dist_v(n + 1);
    for (int i = 1; i <= n; ++i) {
        if (i == u || i == v) continue;
        dist_v[i] = query(v, i);
    }
    dist_v[v] = 0;
    dist_v[u] = dist_u[v];

    vector<Edge> edges;
    map<long long, vector<int>> branches;
    vector<pair<long long, int>> path_nodes_sorted;

    for (int i = 1; i <= n; ++i) {
        if (dist_u[i] + dist_v[i] == dist_u[v]) {
            path_nodes_sorted.push_back({dist_u[i], i});
        } else {
            long long proj_dist_from_u = (dist_u[i] - dist_v[i] + dist_u[v]) / 2;
            branches[proj_dist_from_u].push_back(i);
        }
    }

    sort(path_nodes_sorted.begin(), path_nodes_sorted.end());

    map<long long, int> dist_u_to_node;
    for (auto const& [dist, node_idx] : path_nodes_sorted) {
        dist_u_to_node[dist] = node_idx;
    }

    for (size_t i = 0; i < path_nodes_sorted.size() - 1; ++i) {
        int u1 = path_nodes_sorted[i].second;
        int v1 = path_nodes_sorted[i + 1].second;
        long long w = path_nodes_sorted[i + 1].first - path_nodes_sorted[i].first;
        edges.push_back({u1, v1, w});
    }

    for (auto const& [proj_dist, branch_nodes] : branches) {
        int root_node = dist_u_to_node[proj_dist];

        map<int, long long> dist_from_root;
        dist_from_root[root_node] = 0;
        vector<pair<long long, int>> sorted_branch;

        for (int node : branch_nodes) {
            long long dist_to_root = (dist_u[node] + dist_v[node] - dist_u[v]) / 2;
            dist_from_root[node] = dist_to_root;
            sorted_branch.push_back({dist_to_root, node});
        }
        sort(sorted_branch.begin(), sorted_branch.end());

        vector<int> connected_nodes;
        map<long long, int> dist_to_conn_node;
        
        connected_nodes.push_back(root_node);
        dist_to_conn_node[0] = root_node;

        for (auto const& [d_to_r, node] : sorted_branch) {
            long long max_lca_dist = -1;

            for (int connected_node : connected_nodes) {
                long long d_node_conn = query(node, connected_node);
                long long lca_dist = (dist_from_root[node] + dist_from_root[connected_node] - d_node_conn) / 2;
                if (lca_dist > max_lca_dist) {
                    max_lca_dist = lca_dist;
                }
            }

            int parent = dist_to_conn_node[max_lca_dist];
            edges.push_back({node, parent, dist_from_root[node] - dist_from_root[parent]});

            connected_nodes.push_back(node);
            dist_to_conn_node[dist_from_root[node]] = node;
        }
    }

    cout << "!";
    for (const auto& edge : edges) {
        cout << " " << edge.u << " " << edge.v << " " << edge.w;
    }
    cout << endl;
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