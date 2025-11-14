#include <bits/stdc++.h>
using namespace std;

int k, n;
vector<vector<int>> adj;

vector<pair<int,int>> query(int x, int y, int z) {
    cout << "? " << x << " " << y << " " << z << endl;
    cout.flush();
    
    int r;
    cin >> r;
    
    vector<pair<int,int>> result;
    for(int i = 0; i < r; i++) {
        int a, b;
        cin >> a >> b;
        result.push_back({a, b});
    }
    
    return result;
}

bool areAdjacent(int x, int y, int z) {
    auto result = query(x, y, z);
    for(auto p : result) {
        if((p.first == x && p.second == y) || (p.first == y && p.second == x)) {
            return true;
        }
    }
    return false;
}

int main() {
    cin >> k >> n;
    
    adj.assign(n, vector<int>());
    
    // Find adjacencies for each node
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            // Find a third node to test with
            int third = -1;
            for(int t = 0; t < n; t++) {
                if(t != i && t != j) {
                    third = t;
                    break;
                }
            }
            
            if(areAdjacent(i, j, third)) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    // Find the cycle
    vector<int> path;
    vector<bool> visited(n, false);
    
    // Start from node 0
    path.push_back(0);
    visited[0] = true;
    
    int current = 0;
    while(path.size() < n) {
        bool found = false;
        for(int next : adj[current]) {
            if(!visited[next]) {
                path.push_back(next);
                visited[next] = true;
                current = next;
                found = true;
                break;
            }
        }
        if(!found) break;
    }
    
    cout << "!";
    for(int x : path) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}