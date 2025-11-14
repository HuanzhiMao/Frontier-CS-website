#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    double eps;
    cin >> n >> m >> k >> eps;
    
    vector<set<int>> adj(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u != v) {
            adj[u].insert(v);
            adj[v].insert(u);
        }
    }
    
    int ideal = (n + k - 1) / k;
    int max_size = floor((1 + eps) * ideal);
    
    vector<int> partition(n + 1);
    vector<int> part_sizes(k + 1, 0);
    
    // Simple greedy assignment with balance
    for (int v = 1; v <= n; v++) {
        int best_part = 1;
        
        // Find part with minimum size that's not full
        for (int p = 1; p <= k; p++) {
            if (part_sizes[p] < max_size && part_sizes[p] < part_sizes[best_part]) {
                best_part = p;
            }
        }
        
        // If all parts are full, use the first one
        if (part_sizes[best_part] >= max_size) {
            best_part = 1;
        }
        
        partition[v] = best_part;
        part_sizes[best_part]++;
    }
    
    // Local improvement
    bool improved = true;
    int iterations = 0;
    while (improved && iterations < 100) {
        improved = false;
        iterations++;
        
        for (int v = 1; v <= n; v++) {
            int current_part = partition[v];
            int best_part = current_part;
            int best_score = INT_MAX;
            
            // Calculate current contribution
            int current_ec = 0;
            for (int u : adj[v]) {
                if (partition[u] != current_part) {
                    current_ec++;
                }
            }
            
            for (int p = 1; p <= k; p++) {
                if (p == current_part) continue;
                
                // Check balance constraint
                if (part_sizes[p] >= max_size) continue;
                
                // Calculate new EC contribution
                int new_ec = 0;
                for (int u : adj[v]) {
                    if (partition[u] != p) {
                        new_ec++;
                    }
                }
                
                int ec_diff = new_ec - current_ec;
                if (ec_diff < best_score) {
                    best_score = ec_diff;
                    best_part = p;
                }
            }
            
            if (best_part != current_part && best_score < 0) {
                partition[v] = best_part;
                part_sizes[current_part]--;
                part_sizes[best_part]++;
                improved = true;
            }
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << partition[i] << "\n";
    }
    
    return 0;
}