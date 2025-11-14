#include <bits/stdc++.h>
using namespace std;

int query(vector<int> vertices) {
    cout << "? " << vertices.size() << "\n";
    for (int i = 0; i < vertices.size(); i++) {
        if (i > 0) cout << " ";
        cout << vertices[i];
    }
    cout << "\n";
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // First query: get total number of edges
    vector<int> all_vertices;
    for (int i = 1; i <= n; i++) {
        all_vertices.push_back(i);
    }
    int total_edges = query(all_vertices);
    
    // Find edges by querying pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int edges_in_pair = query({i, j});
            if (edges_in_pair == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    // Check if graph is bipartite using BFS coloring
    vector<int> color(n + 1, -1);
    bool is_bipartite = true;
    vector<int> odd_cycle;
    
    for (int start = 1; start <= n && is_bipartite; start++) {
        if (color[start] == -1) {
            queue<int> q;
            vector<int> parent(n + 1, -1);
            q.push(start);
            color[start] = 0;
            
            while (!q.empty() && is_bipartite) {
                int u = q.front();
                q.pop();
                
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        parent[v] = u;
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // Found odd cycle
                        is_bipartite = false;
                        
                        // Reconstruct the cycle
                        vector<int> path1, path2;
                        int curr = u;
                        while (curr != -1) {
                            path1.push_back(curr);
                            curr = parent[curr];
                        }
                        curr = v;
                        while (curr != -1) {
                            path2.push_back(curr);
                            curr = parent[curr];
                        }
                        
                        reverse(path1.begin(), path1.end());
                        reverse(path2.begin(), path2.end());
                        
                        // Find common ancestor
                        int lca_idx = 0;
                        while (lca_idx < path1.size() && lca_idx < path2.size() && 
                               path1[lca_idx] == path2[lca_idx]) {
                            lca_idx++;
                        }
                        lca_idx--;
                        
                        // Build odd cycle
                        for (int i = path1.size() - 1; i > lca_idx; i--) {
                            odd_cycle.push_back(path1[i]);
                        }
                        for (int i = lca_idx; i < path2.size(); i++) {
                            odd_cycle.push_back(path2[i]);
                        }
                        break;
                    }
                }
            }
        }
    }
    
    if (is_bipartite) {
        vector<int> partition1;
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                partition1.push_back(i);
            }
        }
        
        cout << "Y " << partition1.size() << "\n";
        for (int i = 0; i < partition1.size(); i++) {
            if (i > 0) cout << " ";
            cout << partition1[i];
        }
        cout << "\n";
    } else {
        cout << "N " << odd_cycle.size() << "\n";
        for (int i = 0; i < odd_cycle.size(); i++) {
            if (i > 0) cout << " ";
            cout << odd_cycle[i];
        }
        cout << "\n";
    }
    
    return 0;
}