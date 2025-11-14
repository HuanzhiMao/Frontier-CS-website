#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> adj;
vector<pair<int, int>> edges;

bool query(int v, vector<int>& S) {
    cout << "? " << S.size() << " " << v;
    for (int s : S) {
        cout << " " << s;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return result == 1;
}

bool isOnPath(int v, int u1, int u2) {
    if (v == u1 || v == u2) return true;
    vector<int> S = {u1, u2};
    return query(v, S);
}

void findEdges() {
    // First, find all edges by checking all pairs
    vector<bool> connected(n + 1, false);
    connected[1] = true;
    
    for (int step = 0; step < n - 1; step++) {
        int bestU = -1, bestV = -1;
        
        // Find the next edge to add
        for (int u = 1; u <= n; u++) {
            if (!connected[u]) continue;
            for (int v = 1; v <= n; v++) {
                if (connected[v]) continue;
                
                // Check if u and v are directly connected
                bool directConnection = true;
                
                // Test with a few other vertices to see if any lie on the path
                for (int w = 1; w <= n && directConnection; w++) {
                    if (w == u || w == v) continue;
                    if (isOnPath(w, u, v)) {
                        directConnection = false;
                    }
                }
                
                if (directConnection) {
                    bestU = u;
                    bestV = v;
                    break;
                }
            }
            if (bestV != -1) break;
        }
        
        if (bestU != -1 && bestV != -1) {
            edges.push_back({bestU, bestV});
            connected[bestV] = true;
        }
    }
}

int main() {
    cin >> n;
    adj.resize(n + 1);
    
    if (n == 1) {
        cout << "!" << endl;
        cout.flush();
        return 0;
    }
    
    if (n == 2) {
        cout << "!" << endl;
        cout << "1 2" << endl;
        cout.flush();
        return 0;
    }
    
    // For small n, use a different approach
    set<pair<int, int>> edgeSet;
    
    // Check all possible edges
    for (int u = 1; u <= n; u++) {
        for (int v = u + 1; v <= n; v++) {
            // Check if u and v are directly connected
            bool isDirectEdge = true;
            
            // Test if any other vertex lies on the path between u and v
            for (int w = 1; w <= n && isDirectEdge; w++) {
                if (w == u || w == v) continue;
                
                vector<int> S = {u, v};
                if (query(w, S)) {
                    isDirectEdge = false;
                }
            }
            
            if (isDirectEdge) {
                edgeSet.insert({u, v});
            }
        }
    }
    
    cout << "!" << endl;
    for (auto& edge : edgeSet) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout.flush();
    
    return 0;
}