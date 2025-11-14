#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> result(n + 1);
    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1, 0);
    
    // For each position, we need to determine what number goes there
    // We'll use comparisons to build a partial order
    
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << endl;
            cout.flush();
            
            char response;
            cin >> response;
            
            if (response == '<') {
                adj[i].push_back(j);
                indegree[j]++;
            } else {
                adj[j].push_back(i);
                indegree[i]++;
            }
        }
    }
    
    // Topological sort to get the order
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> order;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        order.push_back(curr);
        
        for (int next : adj[curr]) {
            indegree[next]--;
            if (indegree[next] == 0) {
                q.push(next);
            }
        }
    }
    
    // Assign values based on topological order
    for (int i = 0; i < n; i++) {
        result[order[i]] = i + 1;
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}