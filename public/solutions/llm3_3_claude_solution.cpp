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
            
            vector<int> response(2);
            cin >> response[0] >> response[1];
            
            if (response[1] == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            
            // Clear the set
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            cin >> response[0] >> response[1];
        }
    }
    
    // Build the cycle from adjacency information
    vector<int> result;
    vector<bool> visited(n + 1, false);
    
    int start = 1;
    result.push_back(start);
    visited[start] = true;
    
    int current = start;
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
    
    cout << "-1";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}