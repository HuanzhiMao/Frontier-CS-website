#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int subtask, n;
    cin >> subtask >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Query all pairs to find adjacencies
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Query pair (i, j)
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            
            int r1, r2;
            cin >> r1 >> r2;
            
            if (r2 == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            
            // Turn off the lights
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            cin >> r1 >> r2;
        }
    }
    
    // Build the circular arrangement
    vector<int> result;
    vector<bool> visited(n + 1, false);
    
    // Start from node 1
    int current = 1;
    result.push_back(current);
    visited[current] = true;
    
    while (result.size() < n) {
        for (int next : adj[current]) {
            if (!visited[next]) {
                result.push_back(next);
                visited[next] = true;
                current = next;
                break;
            }
        }
    }
    
    // Output the result
    cout << "-1";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}