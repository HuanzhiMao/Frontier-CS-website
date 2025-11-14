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
        if (u != v) {  // ignore self-loops
            adj[u].insert(v);
            adj[v].insert(u);
        }
    }
    
    int ideal = (n + k - 1) / k;  // ceil(n / k)
    int max_part_size = (int)floor((1 + eps) * ideal);
    
    vector<int> partition(n + 1);
    vector<int> part_sizes(k + 1, 0);
    
    // Simple greedy assignment with some randomization
    random_device rd;
    mt19937 gen(rd());
    
    vector<int> vertices;
    for (int i = 1; i <= n; i++) {
        vertices.push_back(i);
    }
    
    // Sort vertices by degree (descending) for better initial placement
    sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        return adj[a].size() > adj[b].size();
    });
    
    // Initial assignment
    int current_part = 1;
    for (int v : vertices) {
        while (part_sizes[current_part] >= max_part_size && current_part <= k) {
            current_part++;
        }
        if (current_part > k) current_part = 1;
        
        partition[v] = current_part;
        part_sizes[current_part]++;
        
        if (part_sizes[current_part] >= max_part_size) {
            current_part++;
        }
    }
    
    // Local improvement: try to move vertices to reduce edge cut
    bool improved = true;
    int iterations = 0;
    while (improved && iterations < 10) {
        improved = false;
        iterations++;
        
        for (int v = 1; v <= n; v++) {
            int current_part = partition[v];
            vector<int> neighbor_parts(k + 1, 0);
            
            // Count neighbors in each part
            for (int u : adj[v]) {
                neighbor_parts[partition[u]]++;
            }
            
            int best_part = current_part;
            int best_cut_reduction = 0;
            
            for (int new_part = 1; new_part <= k; new_part++) {
                if (new_part == current_part) continue;
                if (part_sizes[new_part] >= max_part_size) continue;
                
                // Calculate cut reduction if we move v to new_part
                int cut_reduction = neighbor_parts[current_part] - neighbor_parts[new_part];
                
                if (cut_reduction > best_cut_reduction) {
                    best_cut_reduction = cut_reduction;
                    best_part = new_part;
                }
            }
            
            if (best_part != current_part && best_cut_reduction > 0) {
                partition[v] = best_part;
                part_sizes[current_part]--;
                part_sizes[best_part]++;
                improved = true;
            }
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << partition[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}