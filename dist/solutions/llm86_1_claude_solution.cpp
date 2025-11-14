#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    
    // For small n, we can use a more direct approach
    if (n == 3) {
        cout << "0 1 2 3" << endl;
        cout.flush();
        int center;
        cin >> center;
        
        cout << "1 ";
        for (int i = 1; i <= n; i++) {
            if (i != center) {
                cout << center << " " << i << " ";
            }
        }
        cout << endl;
        cout.flush();
        return 0;
    }
    
    // Find center of tree by querying triplets
    vector<int> centers;
    map<int, int> center_count;
    
    // Query some triplets to find potential centers
    for (int i = 1; i <= n && centers.size() < min(n, 100); i += max(1, n/20)) {
        for (int j = i + 1; j <= n && centers.size() < min(n, 100); j += max(1, n/20)) {
            for (int k = j + 1; k <= n && centers.size() < min(n, 100); k += max(1, n/20)) {
                cout << "0 " << i << " " << j << " " << k << endl;
                cout.flush();
                int center;
                cin >> center;
                center_count[center]++;
                if (center_count[center] == 1) {
                    centers.push_back(center);
                }
            }
        }
    }
    
    // Sort centers by frequency
    sort(centers.begin(), centers.end(), [&](int a, int b) {
        return center_count[a] > center_count[b];
    });
    
    // Build tree structure by finding connections
    vector<bool> used(n + 1, false);
    queue<int> q;
    
    if (!centers.empty()) {
        q.push(centers[0]);
        used[centers[0]] = true;
    } else {
        q.push(1);
        used[1] = true;
    }
    
    while (!q.empty() && edges.size() < n - 1) {
        int curr = q.front();
        q.pop();
        
        // Find nodes connected to curr
        for (int i = 1; i <= n; i++) {
            if (used[i]) continue;
            
            // Find a third node to complete the query
            int third = -1;
            for (int j = 1; j <= n; j++) {
                if (j != curr && j != i) {
                    third = j;
                    break;
                }
            }
            
            if (third == -1) continue;
            
            cout << "0 " << curr << " " << i << " " << third << endl;
            cout.flush();
            int result;
            cin >> result;
            
            // If result is curr or i, they might be connected
            if (result == curr || result == i) {
                // Check with another query to confirm
                int fourth = -1;
                for (int j = 1; j <= n; j++) {
                    if (j != curr && j != i && j != third) {
                        fourth = j;
                        break;
                    }
                }
                
                if (fourth != -1) {
                    cout << "0 " << curr << " " << i << " " << fourth << endl;
                    cout.flush();
                    int result2;
                    cin >> result2;
                    
                    if (result2 == curr || result2 == i) {
                        edges.push_back({curr, i});
                        used[i] = true;
                        q.push(i);
                        break;
                    }
                }
            }
        }
    }
    
    // If we don't have enough edges, add remaining nodes to any connected node
    while (edges.size() < n - 1) {
        int unconnected = -1;
        for (int i = 1; i <= n; i++) {
            if (!used[i]) {
                unconnected = i;
                break;
            }
        }
        if (unconnected == -1) break;
        
        // Connect to first used node
        for (int i = 1; i <= n; i++) {
            if (used[i]) {
                edges.push_back({i, unconnected});
                used[unconnected] = true;
                break;
            }
        }
    }
    
    cout << "1 ";
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << " ";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}