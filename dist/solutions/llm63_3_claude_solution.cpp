#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> edges(M);
    vector<vector<int>> adj(N);
    
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
        adj[edges[i].first].push_back(i);
        adj[edges[i].second].push_back(i);
    }
    
    // Binary search for A (key location)
    int left_A = 0, right_A = N - 1;
    
    while (left_A < right_A) {
        int mid = (left_A + right_A) / 2;
        
        // Query: direct all edges to create two components
        // Left component: nodes 0 to mid
        // Right component: nodes mid+1 to N-1
        vector<int> directions(M);
        
        for (int i = 0; i < M; i++) {
            int u = edges[i].first;
            int v = edges[i].second;
            
            if (u <= mid && v > mid) {
                directions[i] = 0; // u -> v (left to right)
            } else if (u > mid && v <= mid) {
                directions[i] = 1; // v -> u (left to right)
            } else {
                directions[i] = 0; // within same component, arbitrary
            }
        }
        
        cout << "0";
        for (int d : directions) {
            cout << " " << d;
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            // Key can reach treasure, so key is in left component (0 to mid)
            right_A = mid;
        } else {
            // Key cannot reach treasure, so key is in right component (mid+1 to N-1)
            left_A = mid + 1;
        }
    }
    
    int A = left_A;
    
    // Binary search for B (treasure location)
    int left_B = 0, right_B = N - 1;
    
    while (left_B < right_B) {
        int mid = (left_B + right_B) / 2;
        
        // Query: direct all edges so that we can test reachability to left vs right
        vector<int> directions(M);
        
        for (int i = 0; i < M; i++) {
            int u = edges[i].first;
            int v = edges[i].second;
            
            if (u <= mid && v > mid) {
                directions[i] = 1; // v -> u (right to left)
            } else if (u > mid && v <= mid) {
                directions[i] = 0; // u -> v (right to left)  
            } else {
                directions[i] = 0; // within same component, arbitrary
            }
        }
        
        cout << "0";
        for (int d : directions) {
            cout << " " << d;
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            // Key can reach treasure, so treasure is in left component (0 to mid)
            right_B = mid;
        } else {
            // Key cannot reach treasure, so treasure is in right component (mid+1 to N-1)
            left_B = mid + 1;
        }
    }
    
    int B = left_B;
    
    cout << "1 " << A << " " << B << endl;
    cout.flush();
    
    return 0;
}