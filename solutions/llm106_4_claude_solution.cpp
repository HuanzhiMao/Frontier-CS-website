#include <bits/stdc++.h>
using namespace std;

int query(vector<int>& vertices) {
    cout << "? " << vertices.size() << "\n";
    for (int i = 0; i < vertices.size(); i++) {
        if (i > 0) cout << " ";
        cout << vertices[i];
    }
    cout << "\n";
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<int> degree(n + 1, 0);
    
    // Query total edges
    vector<int> all_vertices;
    for (int i = 1; i <= n; i++) {
        all_vertices.push_back(i);
    }
    int total_edges = query(all_vertices);
    
    // Find degrees and adjacencies
    for (int i = 1; i <= n; i++) {
        vector<int> without_i;
        for (int j = 1; j <= n; j++) {
            if (j != i) without_i.push_back(j);
        }
        int edges_without_i = query(without_i);
        degree[i] = total_edges - edges_without_i;
    }
    
    // Build adjacency matrix using binary search approach
    vector<vector<bool>> is_adj(n + 1, vector<bool>(n + 1, false));
    
    for (int i = 1; i <= n; i++) {
        vector<int> neighbors;
        vector<int> candidates;
        
        for (int j = i + 1; j <= n; j++) {
            candidates.push_back(j);
        }
        
        while (neighbors.size() < degree[i] && !candidates.empty()) {
            if (candidates.size() == 1) {
                neighbors.push_back(candidates[0]);
                candidates.clear();
                break;
            }
            
            int mid = candidates.size() / 2;
            vector<int> test_set = {i};
            for (int k = 0; k < mid; k++) {
                test_set.push_back(candidates[k]);
            }
            
            int edges_in_test = query(test_set);
            int expected_internal = 0;
            
            // Count edges among the test vertices we already know
            for (int a = 0; a < test_set.size(); a++) {
                for (int b = a + 1; b < test_set.size(); b++) {
                    if (is_adj[test_set[a]][test_set[b]]) {
                        expected_internal++;
                    }
                }
            }
            
            int new_edges = edges_in_test - expected_internal;
            
            if (new_edges > 0) {
                // There are edges from i to first half
                for (int k = 0; k < mid; k++) {
                    neighbors.push_back(candidates[k]);
                    is_adj[i][candidates[k]] = is_adj[candidates[k]][i] = true;
                }
                candidates.erase(candidates.begin(), candidates.begin() + mid);
            } else {
                // No edges from i to first half
                candidates.erase(candidates.begin(), candidates.begin() + mid);
            }
        }
    }
    
    // Try to color the graph (BFS)
    vector<int> color(n + 1, -1);
    bool is_bipartite = true;
    
    for (int start = 1; start <= n && is_bipartite; start++) {
        if (color[start] == -1) {
            queue<int> q;
            q.push(start);
            color[start] = 0;
            
            while (!q.empty() && is_bipartite) {
                int u = q.front();
                q.pop();
                
                for (int v = 1; v <= n; v++) {
                    if (is_adj[u][v]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q.push(v);
                        } else if (color[v] == color[u]) {
                            // Found odd cycle, find it
                            is_bipartite = false;
                            cout << "N 3\n";
                            cout << u << " " << v << " ";
                            // Find a third vertex connected to both
                            for (int w = 1; w <= n; w++) {
                                if (w != u && w != v && is_adj[u][w] && is_adj[v][w]) {
                                    cout << w << "\n";
                                    break;
                                }
                            }
                            return 0;
                        }
                    }
                }
            }
        }
    }
    
    if (is_bipartite) {
        vector<int> partition0;
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                partition0.push_back(i);
            }
        }
        
        cout << "Y " << partition0.size() << "\n";
        for (int i = 0; i < partition0.size(); i++) {
            if (i > 0) cout << " ";
            cout << partition0[i];
        }
        cout << "\n";
    }
    
    return 0;
}