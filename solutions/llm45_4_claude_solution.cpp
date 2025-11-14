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
    int max_size = floor((1.0 + eps) * ideal);
    
    vector<int> partition(n + 1);
    vector<int> part_size(k + 1, 0);
    
    // Simple greedy assignment with randomization
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 1);
    random_shuffle(vertices.begin(), vertices.end());
    
    for (int v : vertices) {
        int best_part = 1;
        int min_cost = INT_MAX;
        
        for (int p = 1; p <= k; p++) {
            if (part_size[p] >= max_size) continue;
            
            int cost = 0;
            set<int> neighbor_parts;
            
            for (int u : adj[v]) {
                if (partition[u] > 0 && partition[u] != p) {
                    cost++;
                    neighbor_parts.insert(partition[u]);
                }
            }
            
            cost += neighbor_parts.size();
            cost += part_size[p];
            
            if (cost < min_cost) {
                min_cost = cost;
                best_part = p;
            }
        }
        
        partition[v] = best_part;
        part_size[best_part]++;
    }
    
    // Local improvement
    bool improved = true;
    int iterations = 0;
    while (improved && iterations < 10) {
        improved = false;
        iterations++;
        
        vector<int> order(n);
        iota(order.begin(), order.end(), 1);
        random_shuffle(order.begin(), order.end());
        
        for (int v : order) {
            int current_part = partition[v];
            int best_part = current_part;
            int best_improvement = 0;
            
            // Calculate current cost
            set<int> current_neighbor_parts;
            for (int u : adj[v]) {
                if (partition[u] != current_part) {
                    current_neighbor_parts.insert(partition[u]);
                }
            }
            
            for (int new_part = 1; new_part <= k; new_part++) {
                if (new_part == current_part) continue;
                if (part_size[new_part] >= max_size) continue;
                
                // Calculate new cost
                set<int> new_neighbor_parts;
                for (int u : adj[v]) {
                    if (partition[u] != new_part) {
                        new_neighbor_parts.insert(partition[u]);
                    }
                }
                
                int improvement = current_neighbor_parts.size() - new_neighbor_parts.size();
                
                if (improvement > best_improvement) {
                    best_improvement = improvement;
                    best_part = new_part;
                }
            }
            
            if (best_part != current_part) {
                partition[v] = best_part;
                part_size[current_part]--;
                part_size[best_part]++;
                improved = true;
            }
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << partition[i] << " ";
    }
    cout << "\n";
    
    return 0;
}