#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> a(10);
    for (int i = 0; i < 10; i++) {
        cin >> a[i];
    }
    
    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1, 0);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        indegree[v]++;
    }
    
    // Try topological sort to find Hamiltonian path
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> topo_order;
    vector<int> temp_indegree = indegree;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        for (int v : adj[u]) {
            temp_indegree[v]--;
            if (temp_indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // Check if topological order gives Hamiltonian path
    if (topo_order.size() == n) {
        bool valid = true;
        for (int i = 0; i < n - 1; i++) {
            bool found = false;
            for (int v : adj[topo_order[i]]) {
                if (v == topo_order[i + 1]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            cout << n << "\n";
            for (int i = 0; i < n; i++) {
                cout << topo_order[i];
                if (i < n - 1) cout << " ";
            }
            cout << "\n";
            return 0;
        }
    }
    
    // Greedy approach: find longest path
    vector<bool> visited(n + 1, false);
    vector<int> path;
    int best_len = 0;
    vector<int> best_path;
    
    for (int start = 1; start <= n; start++) {
        fill(visited.begin(), visited.end(), false);
        path.clear();
        
        queue<pair<int, vector<int>>> bfs;
        visited[start] = true;
        bfs.push({start, {start}});
        
        while (!bfs.empty() && path.size() < best_len + 50) {
            auto [u, curr_path] = bfs.front();
            bfs.pop();
            
            if (curr_path.size() > best_len) {
                best_len = curr_path.size();
                best_path = curr_path;
            }
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    vector<bool> new_visited = visited;
                    new_visited[v] = true;
                    vector<int> new_path = curr_path;
                    new_path.push_back(v);
                    
                    queue<pair<int, vector<int>>> dfs;
                    dfs.push({v, new_path});
                    
                    while (!dfs.empty()) {
                        auto [curr, curr_p] = dfs.front();
                        dfs.pop();
                        
                        if (curr_p.size() > best_len) {
                            best_len = curr_p.size();
                            best_path = curr_p;
                        }
                        
                        bool extended = false;
                        for (int next : adj[curr]) {
                            bool next_visited = false;
                            for (int x : curr_p) {
                                if (x == next) {
                                    next_visited = true;
                                    break;
                                }
                            }
                            if (!next_visited) {
                                vector<int> next_path = curr_p;
                                next_path.push_back(next);
                                dfs.push({next, next_path});
                                extended = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    cout << best_len << "\n";
    for (int i = 0; i < best_len; i++) {
        cout << best_path[i];
        if (i < best_len - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}