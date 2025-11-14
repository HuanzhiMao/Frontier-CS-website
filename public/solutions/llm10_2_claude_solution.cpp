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
            cout << "!" << endl;
            cout.flush();
            continue;
        }
        
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 0));
        
        // Query all distances from node 1
        for (int i = 2; i <= n; i++) {
            cout << "? 1 " << i << endl;
            cout.flush();
            cin >> dist[1][i];
            dist[i][1] = dist[1][i];
        }
        
        // Query distances between all other pairs
        for (int i = 2; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                cout << "? " << i << " " << j << endl;
                cout.flush();
                cin >> dist[i][j];
                dist[j][i] = dist[i][j];
            }
        }
        
        vector<tuple<int, int, int>> edges;
        vector<bool> used(n + 1, false);
        
        // Find the tree structure
        used[1] = true;
        
        while (edges.size() < n - 1) {
            int min_dist = INT_MAX;
            int next_node = -1, parent = -1;
            
            // Find the closest unused node to any used node
            for (int i = 1; i <= n; i++) {
                if (!used[i]) continue;
                for (int j = 1; j <= n; j++) {
                    if (used[j] || i == j) continue;
                    if (dist[i][j] < min_dist) {
                        min_dist = dist[i][j];
                        next_node = j;
                        parent = i;
                    }
                }
            }
            
            if (next_node != -1) {
                // Check if this is a direct edge
                bool is_direct = true;
                for (int k = 1; k <= n; k++) {
                    if (k == parent || k == next_node || !used[k]) continue;
                    if (dist[parent][k] + dist[k][next_node] == dist[parent][next_node]) {
                        is_direct = false;
                        break;
                    }
                }
                
                if (is_direct) {
                    edges.push_back({parent, next_node, dist[parent][next_node]});
                    used[next_node] = true;
                } else {
                    // Find the actual parent
                    for (int k = 1; k <= n; k++) {
                        if (k == next_node || !used[k]) continue;
                        bool is_parent = true;
                        for (int m = 1; m <= n; m++) {
                            if (m == k || m == next_node || !used[m]) continue;
                            if (dist[k][m] + dist[k][next_node] != dist[m][next_node]) {
                                is_parent = false;
                                break;
                            }
                        }
                        if (is_parent) {
                            edges.push_back({k, next_node, dist[k][next_node]});
                            used[next_node] = true;
                            break;
                        }
                    }
                }
            }
        }
        
        cout << "!";
        for (auto& edge : edges) {
            cout << " " << get<0>(edge) << " " << get<1>(edge) << " " << get<2>(edge);
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}