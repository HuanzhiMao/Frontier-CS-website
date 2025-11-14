#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        if (n == 1) {
            cout << "!\n";
            cout.flush();
            continue;
        }
        
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 0));
        
        // Query distances from node 1 to all other nodes
        for (int i = 2; i <= n; i++) {
            cout << "? 1 " << i << "\n";
            cout.flush();
            cin >> dist[1][i];
            dist[i][1] = dist[1][i];
        }
        
        // Query distances from node 2 to all other nodes (except 1)
        for (int i = 3; i <= n; i++) {
            cout << "? 2 " << i << "\n";
            cout.flush();
            cin >> dist[2][i];
            dist[i][2] = dist[2][i];
        }
        
        // Build the tree using the distance information
        vector<pair<pair<int, int>, int>> edges;
        vector<bool> used(n + 1, false);
        
        // Start with node 1
        used[1] = true;
        
        while (edges.size() < n - 1) {
            int best_node = -1;
            int best_parent = -1;
            int min_dist = INT_MAX;
            
            // Find the closest unused node to any used node
            for (int i = 2; i <= n; i++) {
                if (used[i]) continue;
                
                for (int j = 1; j <= n; j++) {
                    if (!used[j]) continue;
                    
                    int d = (j == 1) ? dist[1][i] : 
                            (j == 2) ? dist[2][i] : 
                            ((dist[1][i] + dist[1][j] - dist[2][i] - dist[2][j]) / 2 + dist[2][i]);
                    
                    if (d < min_dist) {
                        min_dist = d;
                        best_node = i;
                        best_parent = j;
                    }
                }
            }
            
            if (best_node == -1) break;
            
            // Calculate edge weight
            int weight;
            if (best_parent == 1) {
                weight = dist[1][best_node];
            } else if (best_parent == 2) {
                weight = dist[2][best_node];
            } else {
                // Need to query the distance between best_parent and best_node
                cout << "? " << best_parent << " " << best_node << "\n";
                cout.flush();
                cin >> weight;
            }
            
            edges.push_back({{best_parent, best_node}, weight});
            used[best_node] = true;
        }
        
        // Output the result
        cout << "!";
        for (auto& edge : edges) {
            cout << " " << edge.first.first << " " << edge.first.second << " " << edge.second;
        }
        cout << "\n";
        cout.flush();
    }
    
    return 0;
}