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
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Query all pairs to determine edges
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            vector<int> pair_query = {i, j};
            int edges = query(pair_query);
            if (edges == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    // Try to color the graph with BFS
    vector<int> color(n + 1, -1);
    bool is_bipartite = true;
    vector<int> odd_cycle;
    
    for (int start = 1; start <= n && is_bipartite; start++) {
        if (color[start] != -1) continue;
        
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
                    
                    // Find LCA
                    reverse(path1.begin(), path1.end());
                    reverse(path2.begin(), path2.end());
                    
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
    
    if (is_bipartite) {
        vector<int> partition0;
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                partition0.push_back(i);
            }
        }
        
        cout << "Y " << partition0.size() << "\n";
        for (int i = 0; i < partition0.size(); i++) {
            if (i > 0) cout << " ";
            cout << partition0[i];
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