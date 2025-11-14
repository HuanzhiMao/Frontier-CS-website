#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<vector<int>> adj(101, vector<int>(101, 0));
    
    // Query all triples to determine edges
    for (int i = 1; i <= 100; i++) {
        for (int j = i + 1; j <= 100; j++) {
            for (int k = j + 1; k <= 100; k++) {
                cout << "? " << i << " " << j << " " << k << endl;
                cout.flush();
                int response;
                cin >> response;
                
                if (response == 1) {
                    // Exactly one edge among three vertices
                    // Need to determine which edge exists
                    // Check pairs by using other vertices
                    bool found = false;
                    
                    // Try to find another vertex to help determine the edge
                    for (int x = 1; x <= 100 && !found; x++) {
                        if (x == i || x == j || x == k) continue;
                        
                        cout << "? " << i << " " << j << " " << x << endl;
                        cout.flush();
                        int r1;
                        cin >> r1;
                        
                        cout << "? " << i << " " << k << " " << x << endl;
                        cout.flush();
                        int r2;
                        cin >> r2;
                        
                        cout << "? " << j << " " << k << " " << x << endl;
                        cout.flush();
                        int r3;
                        cin >> r3;
                        
                        // Determine which edge exists based on responses
                        if (r1 > r2 && r1 > r3) {
                            adj[i][j] = adj[j][i] = 1;
                        } else if (r2 > r1 && r2 > r3) {
                            adj[i][k] = adj[k][i] = 1;
                        } else if (r3 > r1 && r3 > r2) {
                            adj[j][k] = adj[k][j] = 1;
                        }
                        found = true;
                        break;
                    }
                } else if (response == 2) {
                    // Two edges exist - need to find which vertex is isolated
                    for (int x = 1; x <= 100; x++) {
                        if (x == i || x == j || x == k) continue;
                        
                        cout << "? " << i << " " << j << " " << x << endl;
                        cout.flush();
                        int r1;
                        cin >> r1;
                        
                        cout << "? " << i << " " << k << " " << x << endl;
                        cout.flush();
                        int r2;
                        cin >> r2;
                        
                        cout << "? " << j << " " << k << " " << x << endl;
                        cout.flush();
                        int r3;
                        cin >> r3;
                        
                        // The pair with lowest count doesn't have an edge
                        if (r1 <= r2 && r1 <= r3) {
                            adj[i][k] = adj[k][i] = 1;
                            adj[j][k] = adj[k][j] = 1;
                        } else if (r2 <= r1 && r2 <= r3) {
                            adj[i][j] = adj[j][i] = 1;
                            adj[j][k] = adj[k][j] = 1;
                        } else {
                            adj[i][j] = adj[j][i] = 1;
                            adj[i][k] = adj[k][i] = 1;
                        }
                        break;
                    }
                } else if (response == 3) {
                    // All three edges exist
                    adj[i][j] = adj[j][i] = 1;
                    adj[i][k] = adj[k][i] = 1;
                    adj[j][k] = adj[k][j] = 1;
                }
            }
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