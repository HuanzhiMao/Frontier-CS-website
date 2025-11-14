#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int subtask, n;
    cin >> subtask >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Find all adjacent pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            
            vector<int> result(2);
            cin >> result[0] >> result[1];
            
            if (result[1] == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            
            // Turn off the lights
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            
            cin >> result[0] >> result[1];
        }
    }
    
    // Build the cycle
    vector<int> cycle;
    vector<bool> visited(n + 1, false);
    
    // Start from node 1
    cycle.push_back(1);
    visited[1] = true;
    int current = 1;
    
    while (cycle.size() < n) {
        for (int next : adj[current]) {
            if (!visited[next]) {
                cycle.push_back(next);
                visited[next] = true;
                current = next;
                break;
            }
        }
    }
    
    cout << "-1";
    for (int x : cycle) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}