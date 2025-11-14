#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

int n;
vector<vector<int>> adj;
vector<pair<int, int>> edges;

bool query(int v, vector<int>& s) {
    cout << "? " << s.size() << " " << v;
    for (int x : s) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return result == 1;
}

bool isOnPath(int v, int a, int b) {
    vector<int> s = {a, b};
    return query(v, s);
}

int main() {
    cin >> n;
    
    adj.resize(n + 1);
    
    // Find edges by checking all pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Check if edge (i,j) exists by seeing if any other vertex lies on path i-j
            bool isEdge = true;
            
            for (int k = 1; k <= n && isEdge; k++) {
                if (k == i || k == j) continue;
                
                if (isOnPath(k, i, j)) {
                    isEdge = false;
                }
            }
            
            if (isEdge) {
                adj[i].push_back(j);
                adj[j].push_back(i);
                edges.push_back({i, j});
            }
        }
    }
    
    cout << "!" << endl;
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout.flush();
    
    return 0;
}