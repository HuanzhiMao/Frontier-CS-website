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
    
    // Simple greedy assignment with balance
    for (int v = 1; v <= n; v++) {
        int best_part = 1;
        int min_cost = INT_MAX;
        
        for (int p = 1; p <= k; p++) {
            if (part_size[p] >= max_part_size) continue;
            
            int cost = 0;
            set<int> neighbor_parts;
            
            for (int u : adj[v]) {
                if (part[u] != 0 && part[u] != p) {
                    neighbor_parts.insert(part[u]);
                }
            }
            
            cost = neighbor_parts.size();
            
            if (cost < min_cost) {
                min_cost = cost;
                best_part = p;
            }
        }
        
        part[v] = best_part;
        part_size[best_part]++;
    }
    
    // Local improvement iterations
    for (int iter = 0; iter < 10; iter++) {
        bool improved = false;
        
        for (int v = 1; v <= n; v++) {
            int current_part = part[v];
            int best_part = current_part;
            int best_improvement = 0;
            
            for (int new_part = 1; new_part <= k; new_part++) {
                if (new_part == current_part) continue;
                if (part_size[new_part] >= max_part_size) continue;
                
                // Calculate improvement
                int old_cut = 0, new_cut = 0;
                
                for (int u : adj[v]) {
                    if (part[u] != current_part) old_cut++;
                    if (part[u] != new_part) new_cut++;
                }
                
                int improvement = old_cut - new_cut;
                
                if (improvement > best_improvement) {
                    best_improvement = improvement;
                    best_part = new_part;
                }
            }
            
            if (best_part != current_part) {
                part[v] = best_part;
                part_size[current_part]--;
                part_size[best_part]++;
                improved = true;
            }
        }
        
        if (!improved) break;
    }
    
    for (int i = 1; i <= n; i++) {
        cout << part[i] << (i == n ? "\n" : " ");
    }
    
    return 0;
}