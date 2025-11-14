#include <bits/stdc++.h>
using namespace std;

int query(vector<int>& vertices) {
    cout << "? " << vertices.size() << "\n";
    for (int i = 0; i < vertices.size(); i++) {
        if (i > 0) cout << " ";
        cout << vertices[i];
    }
    cout << "\n";
    cout.flush();
    
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return result;
}

int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Query all pairs to build the graph
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            vector<int> pair = {i, j};
            int edges = query(pair);
            if (edges == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    // Try to 2-color the graph using BFS
    vector<int> color(n + 1, -1);
    bool bipartite = true;
    
    for (int start = 1; start <= n && bipartite; start++) {
        if (color[start] == -1) {
            queue<int> q;
            q.push(start);
            color[start] = 0;
            
            while (!q.empty() && bipartite) {
                int u = q.front();
                q.pop();
                
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        bipartite = false;
                        // Find odd cycle using DFS
                        vector<int> parent(n + 1, -1);
                        vector<bool> visited(n + 1, false);
                        
                        function<bool(int, int)> dfs = [&](int node, int par) -> bool {
                            visited[node] = true;
                            parent[node] = par;
                            
                            for (int next : adj[node]) {
                                if (next == par) continue;
                                if (visited[next]) {
                                    // Found cycle, reconstruct it
                                    vector<int> cycle;
                                    int curr = node;
                                    while (curr != next) {
                                        cycle.push_back(curr);
                                        curr = parent[curr];
                                    }
                                    cycle.push_back(next);
                                    
                                    cout << "N " << cycle.size() << "\n";
                                    for (int i = 0; i < cycle.size(); i++) {
                                        if (i > 0) cout << " ";
                                        cout << cycle[i];
                                    }
                                    cout << "\n";
                                    cout.flush();
                                    return true;
                                }
                                if (dfs(next, node)) return true;
                            }
                            return false;
                        };
                        
                        if (dfs(1, -1)) return 0;
                        break;
                    }
                }
            }
        }
    }
    
    if (bipartite) {
        vector<int> partition;
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                partition.push_back(i);
            }
        }
        
        cout << "Y " << partition.size() << "\n";
        for (int i = 0; i < partition.size(); i++) {
            if (i > 0) cout << " ";
            cout << partition[i];
        }
        cout << "\n";
        cout.flush();
    }
    
    return 0;
}