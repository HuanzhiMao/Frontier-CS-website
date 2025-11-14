#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 0));
        
        // Query all pairs
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                cout << "? " << i << " " << j << endl;
                cout.flush();
                cin >> dist[i][j];
                dist[j][i] = dist[i][j];
            }
        }
        
        // Find MST using distances
        vector<tuple<int, int, int>> edges;
        vector<bool> inMST(n + 1, false);
        vector<int> key(n + 1, INT_MAX);
        vector<int> parent(n + 1, -1);
        
        key[1] = 0;
        
        for (int count = 0; count < n - 1; count++) {
            int u = -1;
            for (int v = 1; v <= n; v++) {
                if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                    u = v;
                }
            }
            
            inMST[u] = true;
            
            if (parent[u] != -1) {
                edges.push_back({parent[u], u, key[u]});
            }
            
            for (int v = 1; v <= n; v++) {
                if (!inMST[v] && dist[u][v] < key[v]) {
                    key[v] = dist[u][v];
                    parent[v] = u;
                }
            }
        }
        
        // Check if this forms a valid tree by verifying distances
        vector<vector<pair<int, int>>> adj(n + 1);
        for (auto& e : edges) {
            int u = get<0>(e), v = get<1>(e), w = get<2>(e);
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }
        
        // Verify with BFS from node 1
        bool valid = true;
        for (int start = 1; start <= n && valid; start++) {
            vector<int> d(n + 1, -1);
            queue<int> q;
            q.push(start);
            d[start] = 0;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (auto& edge : adj[u]) {
                    int v = edge.first;
                    int w = edge.second;
                    
                    if (d[v] == -1) {
                        d[v] = d[u] + w;
                        q.push(v);
                    }
                }
            }
            
            for (int end = 1; end <= n; end++) {
                if (start != end && d[end] != dist[start][end]) {
                    valid = false;
                    break;
                }
            }
        }
        
        cout << "!";
        for (auto& e : edges) {
            cout << " " << get<0>(e) << " " << get<1>(e) << " " << get<2>(e);
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}