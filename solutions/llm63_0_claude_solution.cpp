#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> edges(M);
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
    }
    
    // Binary search for A (key location)
    int left_A = 0, right_A = N - 1;
    
    while (left_A < right_A) {
        int mid = (left_A + right_A) / 2;
        
        // Create a query where all edges point away from nodes <= mid
        cout << "0";
        for (int i = 0; i < M; i++) {
            int u = edges[i].first, v = edges[i].second;
            if (u <= mid && v > mid) {
                cout << " 0"; // u -> v
            } else if (u > mid && v <= mid) {
                cout << " 1"; // v -> u
            } else {
                cout << " 0"; // arbitrary direction
            }
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 0) {
            // Key is in [0, mid], treasure is in [mid+1, N-1]
            right_A = mid;
        } else {
            // Key is in [mid+1, N-1]
            left_A = mid + 1;
        }
    }
    
    int A = left_A;
    
    // Binary search for B (treasure location)
    int left_B = 0, right_B = N - 1;
    
    while (left_B < right_B) {
        int mid = (left_B + right_B) / 2;
        
        // Create a query where all edges point towards nodes <= mid
        cout << "0";
        for (int i = 0; i < M; i++) {
            int u = edges[i].first, v = edges[i].second;
            if (u <= mid && v > mid) {
                cout << " 1"; // v -> u
            } else if (u > mid && v <= mid) {
                cout << " 0"; // u -> v
            } else {
                cout << " 0"; // arbitrary direction
            }
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            // Treasure is in [0, mid]
            right_B = mid;
        } else {
            // Treasure is in [mid+1, N-1]
            left_B = mid + 1;
        }
    }
    
    int B = left_B;
    
    cout << "1 " << A << " " << B << endl;
    cout.flush();
    
    return 0;
}