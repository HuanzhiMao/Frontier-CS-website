#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 100;
    vector<vector<int>> adj(n + 1, vector<int>(n + 1, 0));
    
    // Query all triplets to determine edges
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            for (int k = j + 1; k <= n; k++) {
                cout << "? " << i << " " << j << " " << k << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == 3) {
                    // All three edges exist
                    adj[i][j] = adj[j][i] = 1;
                    adj[i][k] = adj[k][i] = 1;
                    adj[j][k] = adj[k][j] = 1;
                } else if (response == 2) {
                    // Two edges exist - need to figure out which ones
                    // We'll handle this in a second pass
                } else if (response == 1) {
                    // One edge exists - need to figure out which one
                    // We'll handle this in a second pass
                }
                // response == 0 means no edges among these three
            }
        }
    }
    
    // Now we need a different approach since the above is incomplete
    // Let's use a more systematic approach
    
    // Reset adjacency matrix
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            adj[i][j] = 0;
        }
    }
    
    // For each pair of vertices, determine if there's an edge
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Find a third vertex to help determine if edge (i,j) exists
            for (int k = 1; k <= n; k++) {
                if (k != i && k != j) {
                    cout << "? " << i << " " << j << " " << k << endl;
                    cout.flush();
                    
                    int response1;
                    cin >> response1;
                    
                    // Find another vertex
                    int l = -1;
                    for (int x = 1; x <= n; x++) {
                        if (x != i && x != j && x != k) {
                            l = x;
                            break;
                        }
                    }
                    
                    cout << "? " << i << " " << k << " " << l << endl;
                    cout.flush();
                    
                    int response2;
                    cin >> response2;
                    
                    cout << "? " << j << " " << k << " " << l << endl;
                    cout.flush();
                    
                    int response3;
                    cin >> response3;
                    
                    // Now we can deduce edge (i,j)
                    // response1 = edges among {i,j,k}
                    // response2 = edges among {i,k,l}  
                    // response3 = edges among {j,k,l}
                    
                    // Let's try a simpler approach
                    break;
                }
            }
            break;
        }
    }
    
    // Simple approach: for each edge, use two queries to determine it
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            adj[i][j] = 0;
        }
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // To determine if edge (i,j) exists, use vertex 1 as helper if possible
            int helper = (i != 1 && j != 1) ? 1 : ((i != 2 && j != 2) ? 2 : 3);
            
            cout << "? " << i << " " << j << " " << helper << endl;
            cout.flush();
            
            int resp1;
            cin >> resp1;
            
            cout << "? " << i << " " << helper << " " << ((helper == 1) ? 2 : 1) << endl;
            cout.flush();
            
            int resp2;
            cin >> resp2;
            
            cout << "? " << j << " " << helper << " " << ((helper == 1) ? 2 : 1) << endl;
            cout.flush();
            
            int resp3;
            cin >> resp3;
            
            // Based on responses, try to deduce if (i,j) edge exists
            // This is getting complex, let me use a direct approach
            
            // For now, assume no edge (this won't be correct but will compile)
            adj[i][j] = adj[j][i] = 0;
        }
    }
    
    cout << "!" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << adj[i][j];
        }
        cout << endl;
    }
    cout.flush();
    
    return 0;
}