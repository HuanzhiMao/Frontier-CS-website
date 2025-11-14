#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    double eps;
    cin >> n >> m >> k >> eps;
    
    vector<vector<int>> adj(n + 1);
    set<pair<int, int>> edges;
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u != v && edges.find({min(u, v), max(u, v)}) == edges.end()) {
            edges.insert({min(u, v), max(u, v)});
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    
    int ideal = (n + k - 1) / k;
    int max_part_size = (int)floor((1.0 + eps) * ideal);
    
    vector<int> part(n + 1);
    vector<int> part_size(k + 1, 0);
    
    // Initialize with simple assignment
    for (int i = 1; i <= n; i++) {
        part[i] = ((i - 1) % k) + 1;
        part_size[part[i]]++;
    }
    
    // Balance parts
    for (int i = 1; i <= n; i++) {
        if (part_size[part[i]] > max_part_size) {
            for (int j = 1; j <= k; j++) {
                if (part_size[j] < max_part_size) {
                    part_size[part[i]]--;
                    part_size[j]++;
                    part[i] = j;
                    break;
                }
            }
        }
    }
    
    // Local improvement
    for (int iter = 0; iter < 10; iter++) {
        bool improved = false;
        
        for (int v = 1; v <= n; v++) {
            int current_part = part[v];
            int best_part = current_part;
            int min_external_edges = INT_MAX;
            
            for (int new_part = 1; new_part <= k; new_part++) {
                if (new_part == current_part) continue;
                
                // Check balance constraint
                if (part_size[new_part] >= max_part_size) continue;
                
                // Count external edges if moved to new_part
                int external = 0;
                for (int u : adj[v]) {
                    if (part[u] != new_part) {
                        external++;
                    }
                }
                
                if (external < min_external_edges) {
                    min_external_edges = external;
                    best_part = new_part;
                }
            }
            
            if (best_part != current_part) {
                part_size[current_part]--;
                part_size[best_part]++;
                part[v] = best_part;
                improved = true;
            }
        }
        
        if (!improved) break;
    }
    
    // Kernighan-Lin style improvements
    for (int iter = 0; iter < 5; iter++) {
        bool improved = false;
        
        for (int v = 1; v <= n; v++) {
            int current_part = part[v];
            
            // Try swapping with vertices from other parts
            for (int u = v + 1; u <= n; u++) {
                if (part[u] == current_part) continue;
                
                // Calculate improvement from swap
                int v_external_before = 0, v_external_after = 0;
                int u_external_before = 0, u_external_after = 0;
                
                for (int w : adj[v]) {
                    if (part[w] != current_part) v_external_before++;
                    if (part[w] != part[u]) v_external_after++;
                }
                
                for (int w : adj[u]) {
                    if (part[w] != part[u]) u_external_before++;
                    if (part[w] != current_part) u_external_after++;
                }
                
                int improvement = (v_external_before + u_external_before) - (v_external_after + u_external_after);
                
                if (improvement > 0) {
                    swap(part[v], part[u]);
                    improved = true;
                }
            }
        }
        
        if (!improved) break;
    }
    
    for (int i = 1; i <= n; i++) {
        cout << part[i] << "\n";
    }
    
    return 0;
}