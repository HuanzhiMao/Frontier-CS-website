#include <bits/stdc++.h>
using namespace std;

int n;
vector<pair<int, int>> edges;

int query(int v, vector<int>& S) {
    cout << "? " << S.size() << " " << v;
    for (int s : S) {
        cout << " " << s;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return result;
}

bool isOnPath(int v, int a, int b) {
    vector<int> S = {a, b};
    return query(v, S) == 1;
}

int main() {
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Find edges by checking if vertices are on paths between pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Check if there's a direct edge between i and j
            bool direct = true;
            
            // Check if any other vertex lies on path between i and j
            for (int k = 1; k <= n && direct; k++) {
                if (k == i || k == j) continue;
                if (isOnPath(k, i, j)) {
                    direct = false;
                }
            }
            
            if (direct) {
                edges.push_back({i, j});
                adj[i].push_back(j);
                adj[j].push_back(i);
                
                if (edges.size() == n - 1) break;
            }
        }
        if (edges.size() == n - 1) break;
    }
    
    cout << "!" << endl;
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout.flush();
    
    return 0;
}