#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<vector<int>> adj(101, vector<int>(101, -1));
    
    // Initialize diagonal to 0 (no self-loops)
    for (int i = 1; i <= 100; i++) {
        adj[i][i] = 0;
    }
    
    // Query all triplets and store results
    vector<vector<vector<int>>> triplet_result(101, vector<vector<int>>(101, vector<int>(101, -1)));
    
    for (int i = 1; i <= 100; i++) {
        for (int j = i + 1; j <= 100; j++) {
            for (int k = j + 1; k <= 100; k++) {
                cout << "? " << i << " " << j << " " << k << endl;
                cout.flush();
                int result;
                cin >> result;
                triplet_result[i][j][k] = result;
            }
        }
    }
    
    // Process results to determine edges
    for (int i = 1; i <= 100; i++) {
        for (int j = i + 1; j <= 100; j++) {
            if (adj[i][j] != -1) continue;
            
            // Count how many triplets (i,j,k) have result 1 or 3
            int count_odd = 0;
            int total_count = 0;
            
            for (int k = 1; k <= 100; k++) {
                if (k == i || k == j) continue;
                
                int result = -1;
                if (i < j && j < k) {
                    result = triplet_result[i][j][k];
                } else if (i < k && k < j) {
                    result = triplet_result[i][k][j];
                } else if (k < i && i < j) {
                    result = triplet_result[k][i][j];
                }
                
                if (result != -1) {
                    total_count++;
                    if (result % 2 == 1) {
                        count_odd++;
                    }
                }
            }
            
            // If more than half of the triplets have odd result, there's likely an edge
            if (count_odd * 2 > total_count) {
                adj[i][j] = adj[j][i] = 1;
            } else {
                adj[i][j] = adj[j][i] = 0;
            }
        }
    }
    
    // Refine using constraint satisfaction
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 1; i <= 100; i++) {
            for (int j = i + 1; j <= 100; j++) {
                for (int k = j + 1; k <= 100; k++) {
                    int expected = adj[i][j] + adj[i][k] + adj[j][k];
                    int actual = triplet_result[i][j][k];
                    
                    if (expected != actual) {
                        // Try flipping edges that are most uncertain
                        if (adj[i][j] == 1 && expected > actual) {
                            adj[i][j] = adj[j][i] = 0;
                            changed = true;
                        } else if (adj[i][j] == 0 && expected < actual) {
                            adj[i][j] = adj[j][i] = 1;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    cout << "!" << endl;
    for (int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 100; j++) {
            cout << (adj[i][j] == 1 ? '1' : '0');
        }
        cout << endl;
    }
    cout.flush();
    
    return 0;
}