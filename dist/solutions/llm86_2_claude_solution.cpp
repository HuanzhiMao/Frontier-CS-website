#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    
    // For each pair of nodes, find their LCA using a third node
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Find a third node that's not i or j
            int k = (i != 1 && j != 1) ? 1 : ((i != 2 && j != 2) ? 2 : 3);
            
            cout << "0 " << i << " " << j << " " << k << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            // The response gives us information about the path structure
            // If response is i, then i is on the path from j to k
            // If response is j, then j is on the path from i to k
            // If response is k, then k is on the path from i to j
            // Otherwise, response is the LCA or a node on one of the paths
            
            if (response == i) {
                // i is between j and k, so there might be edges involving i
                continue;
            } else if (response == j) {
                // j is between i and k
                continue;
            } else if (response == k) {
                // k is between i and j, so i and j are not directly connected
                continue;
            }
        }
    }
    
    // Use a different approach - build the tree incrementally
    vector<bool> connected(n + 1, false);
    connected[1] = true;
    
    for (int node = 2; node <= n; node++) {
        // Find where this node connects
        vector<int> candidates;
        for (int i = 1; i < node; i++) {
            if (connected[i]) {
                candidates.push_back(i);
            }
        }
        
        if (candidates.size() >= 2) {
            // Query with node and two candidates
            cout << "0 " << node << " " << candidates[0] << " " << candidates[1] << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            if (response == candidates[0]) {
                edges.push_back({node, candidates[0]});
                connected[node] = true;
            } else if (response == candidates[1]) {
                edges.push_back({node, candidates[1]});
                connected[node] = true;
            } else if (response == node) {
                // node is between the two candidates
                edges.push_back({node, candidates[0]});
                connected[node] = true;
            } else {
                // response is some other node - connect to it
                edges.push_back({node, response});
                connected[node] = true;
            }
        } else if (candidates.size() == 1) {
            edges.push_back({node, candidates[0]});
            connected[node] = true;
        }
    }
    
    // If we don't have n-1 edges yet, connect remaining nodes
    while (edges.size() < n - 1) {
        for (int i = 2; i <= n; i++) {
            if (!connected[i]) {
                edges.push_back({i, 1});
                connected[i] = true;
                break;
            }
        }
    }
    
    cout << "1";
    for (auto& edge : edges) {
        cout << " " << edge.first << " " << edge.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}