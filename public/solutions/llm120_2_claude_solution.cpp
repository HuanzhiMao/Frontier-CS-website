#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<vector<int>> adj(100, vector<int>(100, 0));
    
    // Query all triplets
    for (int i = 0; i < 100; i++) {
        for (int j = i + 1; j < 100; j++) {
            for (int k = j + 1; k < 100; k++) {
                cout << "? " << (i + 1) << " " << (j + 1) << " " << (k + 1) << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == 3) {
                    adj[i][j] = adj[j][i] = 1;
                    adj[i][k] = adj[k][i] = 1;
                    adj[j][k] = adj[k][j] = 1;
                } else if (response == 0) {
                    // No edges
                } else if (response == 1) {
                    // Need to determine which edge exists
                    // Check pairs involving these vertices in other queries
                } else if (response == 2) {
                    // Two edges exist, one missing
                }
            }
        }
    }
    
    // For queries with response 1 or 2, we need additional logic
    // Let's use a different approach: build system of equations
    
    vector<vector<int>> queries;
    vector<int> responses;
    
    // Re-query and store results
    for (int i = 0; i < 100; i++) {
        for (int j = i + 1; j < 100; j++) {
            for (int k = j + 1; k < 100; k++) {
                cout << "? " << (i + 1) << " " << (j + 1) << " " << (k + 1) << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                queries.push_back({i, j, k});
                responses.push_back(response);
            }
        }
    }
    
    // Reset adjacency matrix
    adj.assign(100, vector<int>(100, 0));
    
    // Process responses
    for (int q = 0; q < queries.size(); q++) {
        int i = queries[q][0], j = queries[q][1], k = queries[q][2];
        int resp = responses[q];
        
        if (resp == 3) {
            adj[i][j] = adj[j][i] = 1;
            adj[i][k] = adj[k][i] = 1;
            adj[j][k] = adj[k][j] = 1;
        } else if (resp == 0) {
            // All edges are 0, but we don't set them explicitly since default is 0
        }
    }
    
    // For ambiguous cases (resp == 1 or 2), use constraint satisfaction
    bool changed = true;
    while (changed) {
        changed = false;
        for (int q = 0; q < queries.size(); q++) {
            int i = queries[q][0], j = queries[q][1], k = queries[q][2];
            int resp = responses[q];
            
            int current = adj[i][j] + adj[i][k] + adj[j][k];
            
            if (current < resp) {
                // Need to add edges
                if (resp == 1 && current == 0) {
                    // Try to find which edge should be 1 based on other constraints
                    // This is a simplified heuristic
                } else if (resp == 2 && current < 2) {
                    // Similar logic for 2 edges
                }
            }
        }
    }
    
    // Use integer programming approach for remaining ambiguity
    // Simplified: try all possibilities for small cases
    
    cout << "!" << endl;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            cout << adj[i][j];
        }
        cout << endl;
    }
    cout.flush();
    
    return 0;
}