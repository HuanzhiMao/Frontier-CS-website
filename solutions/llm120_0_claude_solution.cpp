#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<vector<int>> adj(101, vector<int>(101, 0));
    
    // Query all triplets and store results
    vector<vector<vector<int>>> triplet_count(101, vector<vector<int>>(101, vector<int>(101, -1)));
    
    for (int i = 1; i <= 100; i++) {
        for (int j = i + 1; j <= 100; j++) {
            for (int k = j + 1; k <= 100; k++) {
                cout << "? " << i << " " << j << " " << k << endl;
                cout.flush();
                int count;
                cin >> count;
                triplet_count[i][j][k] = count;
            }
        }
    }
    
    // For each pair of vertices, count how many times it appears in triplets with different edge counts
    for (int i = 1; i <= 100; i++) {
        for (int j = i + 1; j <= 100; j++) {
            int count_with_edge = 0;
            int count_without_edge = 0;
            
            for (int k = 1; k <= 100; k++) {
                if (k == i || k == j) continue;
                
                int a = min({i, j, k});
                int b = max({i, j, k});
                int c = i + j + k - a - b;
                
                int edges = triplet_count[a][c][b];
                
                // Count triangles where (i,j) would contribute
                if (edges >= 2) {
                    count_with_edge++;
                } else if (edges == 0) {
                    count_without_edge++;
                }
            }
            
            // Determine if edge exists based on pattern
            // If (i,j) is connected, triplets containing (i,j) tend to have more edges
            bool has_edge = false;
            
            for (int k = 1; k <= 100; k++) {
                if (k == i || k == j) continue;
                
                int a = min({i, j, k});
                int b = max({i, j, k});
                int c = i + j + k - a - b;
                
                int edges = triplet_count[a][c][b];
                
                if (edges == 1) {
                    // Check if removing (i,j) would make it 0
                    // We need to check other pairs in this triplet
                    int other_pairs_edges = 0;
                    
                    // Count edges between other vertices through other triplets
                    bool found_pattern = false;
                    for (int x = 1; x <= 100 && !found_pattern; x++) {
                        if (x == i || x == j || x == k) continue;
                        
                        // Check triplet (i,k,x)
                        int t1 = min({i, k, x});
                        int t2 = max({i, k, x});
                        int t3 = i + k + x - t1 - t2;
                        
                        // Check triplet (j,k,x)  
                        int u1 = min({j, k, x});
                        int u2 = max({j, k, x});
                        int u3 = j + k + x - u1 - u2;
                        
                        int edges_ik = triplet_count[t1][t3][t2];
                        int edges_jk = triplet_count[u1][u3][u2];
                        
                        if (edges_ik + edges_jk - edges >= 1) {
                            has_edge = true;
                            found_pattern = true;
                        }
                    }
                    
                    if (!found_pattern && count_with_edge > count_without_edge) {
                        has_edge = true;
                    }
                } else if (edges >= 2) {
                    has_edge = true;
                    break;
                }
            }
            
            adj[i][j] = adj[j][i] = has_edge ? 1 : 0;
        }
    }
    
    cout << "!" << endl;
    for (int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 100; j++) {
            cout << adj[i][j];
        }
        cout << endl;
    }
    cout.flush();
    
    return 0;
}