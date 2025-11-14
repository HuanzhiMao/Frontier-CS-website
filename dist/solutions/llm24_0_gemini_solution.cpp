#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>

using namespace std;

int n;
vector<vector<int>> C;
vector<int> best_p;

// Helper to check if a new permutation is lexicographically smaller
bool is_better(const vector<int>& p) {
    if (best_p.empty()) {
        return true;
    }
    return p < best_p;
}

// Greedily constructs a path starting from a node, within a given set of vertices
vector<int> greedy_path(int start_node, const vector<int>& nodes, int color) {
    if (nodes.empty()) {
        return {};
    }
    
    vector<bool> is_in_nodes(n + 1, false);
    for (int node : nodes) {
        is_in_nodes[node] = true;
    }

    if (!is_in_nodes[start_node]) {
        return {};
    }

    vector<int> path;
    path.push_back(start_node);
    vector<bool> used(n + 1, false);
    used[start_node] = true;

    int curr = start_node;
    while (path.size() < nodes.size()) {
        int next_node = -1;
        // The problem statement doesn't specify which neighbor to take if multiple are available.
        // To find the lexicographically smallest permutation, we should prioritize smaller node indices.
        for (int neighbor = 1; neighbor <= n; ++neighbor) {
            if (is_in_nodes[neighbor] && !used[neighbor] && C[curr][neighbor] == color) {
                if (next_node == -1 || neighbor < next_node) {
                    next_node = neighbor;
                }
            }
        }
        if (next_node == -1) {
            return {}; // Failed to extend path
        }
        path.push_back(next_node);
        used[next_node] = true;
        curr = next_node;
    }
    return path;
}

void solve() {
    best_p.clear();

    vector<int> all_nodes(n);
    iota(all_nodes.begin(), all_nodes.end(), 1);

    // Case 0: Monochromatic cycle
    for (int k = 0; k < 2; ++k) {
        for (int i = 1; i <= n; ++i) {
            vector<int> p = greedy_path(i, all_nodes, k);
            if (p.size() == n && C[p.back()][p.front()] == k) {
                if (is_better(p)) {
                    best_p = p;
                }
            }
        }
    }
    
    // Case 1: One color change
    for (int k = 0; k < 2; ++k) {
        vector<int> component(n + 1, 0);
        vector<bool> visited(n + 1, false);
        int comp_id = 0;
        function<void(int)> dfs = 
            [&](int u) {
            visited[u] = true;
            component[u] = comp_id;
            for (int v = 1; v <= n; ++v) {
                if (C[u][v] == k && !visited[v]) {
                    dfs(v);
                }
            }
        };

        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                comp_id++;
                dfs(i);
            }
        }

        if (comp_id <= 1) continue;

        vector<vector<int>> components_nodes(comp_id + 1);
        for (int i = 1; i <= n; ++i) {
            components_nodes[component[i]].push_back(i);
        }
        
        for (int i = 1; i <= comp_id; ++i) {
            vector<int> A = components_nodes[i];
            vector<int> B;
            for (int j = 1; j <= comp_id; ++j) {
                if (i != j) {
                    B.insert(B.end(), components_nodes[j].begin(), components_nodes[j].end());
                }
            }

            if (A.empty() || B.empty()) continue;

            sort(A.begin(), A.end());
            sort(B.begin(), B.end());
            
            vector<vector<int>> paths_A;
            for(int start_node : A) {
                vector<int> p = greedy_path(start_node, A, k);
                if (p.size() == A.size()) {
                    paths_A.push_back(p);
                }
            }

            vector<vector<int>> paths_B;
             for(int start_node : B) {
                vector<int> p = greedy_path(start_node, B, 1 - k);
                if (p.size() == B.size()) {
                    paths_B.push_back(p);
                }
            }
            
            for(const auto& p_A : paths_A) {
                for(const auto& p_B : paths_B) {
                    vector<int> p_A_rev = p_A;
                    reverse(p_A_rev.begin(), p_A_rev.end());
                    vector<int> p_B_rev = p_B;
                    reverse(p_B_rev.begin(), p_B_rev.end());

                    vector<vector<int>> pA_versions = {p_A, p_A_rev};
                    vector<vector<int>> pB_versions = {p_B, p_B_rev};
                    
                    for(const auto& pa : pA_versions) {
                        for(const auto& pb : pB_versions) {
                            vector<int> current_p = pa;
                            current_p.insert(current_p.end(), pb.begin(), pb.end());
                            if (is_better(current_p)) {
                                best_p = current_p;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    while (cin >> n) {
        C.assign(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; ++i) {
            string row;
            cin >> row;
            for (int j = 1; j <= n; ++j) {
                C[i][j] = row[j - 1] - '0';
            }
        }

        solve();

        if (best_p.empty()) {
            cout << -1 << "\n";
        } else {
            for (int i = 0; i < n; ++i) {
                cout << best_p[i] << (i == n - 1 ? "" : " ");
            }
            cout << "\n";
        }
    }

    return 0;
}