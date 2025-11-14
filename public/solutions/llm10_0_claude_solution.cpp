#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 0));
        
        // Query all pairwise distances
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                cout << "? " << i << " " << j << endl;
                cout.flush();
                cin >> dist[i][j];
                dist[j][i] = dist[i][j];
            }
        }
        
        vector<tuple<int, int, int>> edges;
        vector<bool> used(n + 1, false);
        
        // Find minimum spanning tree using modified approach
        // Start with vertex 1
        used[1] = true;
        
        for (int count = 0; count < n - 1; count++) {
            int min_dist = 1e9;
            int u = -1, v = -1;
            
            // Find minimum distance edge from used vertices to unused vertices
            for (int i = 1; i <= n; i++) {
                if (used[i]) {
                    for (int j = 1; j <= n; j++) {
                        if (!used[j] && dist[i][j] < min_dist) {
                            min_dist = dist[i][j];
                            u = i;
                            v = j;
                        }
                    }
                }
            }
            
            edges.push_back({u, v, min_dist});
            used[v] = true;
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