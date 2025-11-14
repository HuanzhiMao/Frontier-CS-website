#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

long long query(int u, int v) {
    if (u == v) return 0;
    cout << "? " << u << " " << v << endl;
    long long d;
    cin >> d;
    return d;
}

struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n;
    cin >> n;

    if (n == 1) {
        cout << "!" << endl;
        return;
    }
    if (n == 2) {
        long long w = query(1, 2);
        cout << "! 1 2 " << w << endl;
        return;
    }

    // Step 1: Find an endpoint of a diameter 'f'
    int f = 1;
    long long max_d = -1;
    for (int i = 2; i <= n; ++i) {
        long long d = query(1, i);
        if (d > max_d) {
            max_d = d;
            f = i;
        }
    }

    // Step 2: Find the other endpoint 'g' and distances from 'f'
    vector<long long> distf(n + 1);
    int g = -1;
    max_d = -1;
    for (int i = 1; i <= n; ++i) {
        if (i == f) continue;
        distf[i] = query(f, i);
        if (distf[i] > max_d) {
            max_d = distf[i];
            g = i;
        }
    }
    distf[g] = max_d;

    // Step 3: Get distances from 'g'
    vector<long long> distg(n + 1);
    distg[f] = distf[g];
    for (int i = 1; i <= n; ++i) {
        if (i == g || i == f) continue;
        distg[i] = query(g, i);
    }
    long long D = distf[g];

    // Step 4: Partition nodes based on their projection on the diameter
    map<long long, vector<int>> groups;
    vector<pair<long long, int>> diam_nodes_info;
    map<long long, int> proj_dist_to_node;

    for (int i = 1; i <= n; ++i) {
        long long proj_dist = (distf[i] - distg[i] + D) / 2;
        groups[proj_dist].push_back(i);
        long long dist_to_diam = (distf[i] + distg[i] - D) / 2;
        if (dist_to_diam == 0) {
            diam_nodes_info.push_back({proj_dist, i});
            proj_dist_to_node[proj_dist] = i;
        }
    }

    vector<Edge> edges;
    sort(diam_nodes_info.begin(), diam_nodes_info.end());

    // Step 5: Reconstruct the diameter path
    for (size_t i = 0; i < diam_nodes_info.size() - 1; ++i) {
        int u = diam_nodes_info[i].second;
        int v = diam_nodes_info[i+1].second;
        edges.push_back({u, v, distf[v] - distf[u]});
    }

    // Step 6: Solve subproblems for branches off the diameter
    for (auto const& [proj_d, nodes] : groups) {
        int diam_node = proj_dist_to_node[proj_d];
        vector<pair<long long, int>> sub_nodes;
        for (int node : nodes) {
            long long dist_to_diam = (distf[node] + distg[node] - D) / 2;
            if (dist_to_diam > 0) {
                sub_nodes.push_back({dist_to_diam, node});
            }
        }
        sort(sub_nodes.begin(), sub_nodes.end());
        
        vector<pair<long long, int>> processed_in_subproblem;
        processed_in_subproblem.push_back({0, diam_node});

        for (auto const& [d, u] : sub_nodes) {
            int best_p = -1;
            long long max_lca_dist = -1;

            for(auto const& [d_cand, p_cand] : processed_in_subproblem) {
                long long dist_u_pcand = query(u, p_cand);
                long long dist_vj_u = d;
                long long dist_vj_pcand = d_cand;

                long long current_lca_dist = (dist_vj_u + dist_vj_pcand - dist_u_pcand) / 2;
                if(current_lca_dist > max_lca_dist) {
                    max_lca_dist = current_lca_dist;
                    best_p = p_cand;
                }
            }
            long long weight = d - max_lca_dist;
            edges.push_back({u, best_p, weight});
            processed_in_subproblem.push_back({d, u});
        }
    }

    cout << "! ";
    for (size_t i = 0; i < edges.size(); ++i) {
        cout << edges[i].u << " " << edges[i].v << " " << edges[i].w << (i == edges.size() - 1 ? "" : " ");
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