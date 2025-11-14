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
        
        // Query all pairs of distances
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                cout << "? " << i << " " << j << endl;
                cout.flush();
                cin >> dist[i][j];
                dist[j][i] = dist[i][j];
            }
        }
        
        // Find the tree structure using MST approach
        vector<tuple<int, int, int>> edges; // weight, u, v
        vector<bool> used(n + 1, false);
        vector<int> min_edge(n + 1, INT_MAX);
        vector<int> sel_edge(n + 1, -1);
        
        min_edge[1] = 0;
        
        for (int i = 0; i < n; i++) {
            int v = -1;
            for (int j = 1; j <= n; j++) {
                if (!used[j] && (v == -1 || min_edge[j] < min_edge[v])) {
                    v = j;
                }
            }
            
            used[v] = true;
            if (sel_edge[v] != -1) {
                edges.push_back({min_edge[v], v, sel_edge[v]});
            }
            
            for (int to = 1; to <= n; to++) {
                if (!used[to] && dist[v][to] < min_edge[to]) {
                    min_edge[to] = dist[v][to];
                    sel_edge[to] = v;
                }
            }
        }
        
        cout << "!";
        for (auto& edge : edges) {
            cout << " " << get<1>(edge) << " " << get<2>(edge) << " " << get<0>(edge);
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}