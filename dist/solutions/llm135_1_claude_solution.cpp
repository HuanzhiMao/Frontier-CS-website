#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<vector<int>> adj;
vector<bool> used;

bool query(int x, int y, int z) {
    cout << "? " << x << " " << y << " " << z << endl;
    cout.flush();
    
    int r;
    cin >> r;
    
    bool xy = false, yz = false, zx = false;
    
    for (int i = 0; i < r; i++) {
        int a, b;
        cin >> a >> b;
        if ((a == x && b == y) || (a == y && b == x)) xy = true;
        if ((a == y && b == z) || (a == z && b == y)) yz = true;
        if ((a == z && b == x) || (a == x && b == z)) zx = true;
    }
    
    if (xy && !yz && !zx) {
        adj[x].push_back(y);
        adj[y].push_back(x);
        return true;
    }
    if (yz && !xy && !zx) {
        adj[y].push_back(z);
        adj[z].push_back(y);
        return true;
    }
    if (zx && !xy && !yz) {
        adj[z].push_back(x);
        adj[x].push_back(z);
        return true;
    }
    
    return false;
}

int main() {
    cin >> k >> n;
    
    adj.resize(n);
    used.resize(n, false);
    
    vector<int> nodes;
    for (int i = 0; i < n; i++) {
        nodes.push_back(i);
    }
    
    random_shuffle(nodes.begin(), nodes.end());
    
    // Try to find edges by asking queries
    int queries = 0;
    
    for (int i = 0; i < n && queries < k - 100; i++) {
        for (int j = i + 1; j < n && queries < k - 100; j++) {
            for (int l = j + 1; l < n && queries < k - 100; l++) {
                if (adj[nodes[i]].size() >= 2 && adj[nodes[j]].size() >= 2 && adj[nodes[l]].size() >= 2) {
                    continue;
                }
                
                query(nodes[i], nodes[j], nodes[l]);
                queries++;
                
                // Check if we have enough edges
                int complete = 0;
                for (int x = 0; x < n; x++) {
                    if (adj[x].size() == 2) complete++;
                }
                if (complete == n) break;
            }
            if (queries >= k - 100) break;
        }
        if (queries >= k - 100) break;
    }
    
    // Find a node with degree 2 to start traversal
    int start = 0;
    for (int i = 0; i < n; i++) {
        if (adj[i].size() >= 1) {
            start = i;
            break;
        }
    }
    
    // Try to construct the cycle
    vector<int> result;
    vector<bool> visited(n, false);
    
    result.push_back(start);
    visited[start] = true;
    
    int current = start;
    int prev = -1;
    
    while (result.size() < n) {
        int next = -1;
        for (int neighbor : adj[current]) {
            if (neighbor != prev && !visited[neighbor]) {
                next = neighbor;
                break;
            }
        }
        
        if (next == -1) {
            // Try to find any unvisited node
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    next = i;
                    break;
                }
            }
        }
        
        if (next == -1) break;
        
        result.push_back(next);
        visited[next] = true;
        prev = current;
        current = next;
    }
    
    // If we don't have a complete cycle, add remaining nodes
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            result.push_back(i);
        }
    }
    
    cout << "!";
    for (int i = 0; i < n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}