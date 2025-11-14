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
    
    // Binary search to find A (key star)
    int left_A = 0, right_A = N - 1;
    
    while (left_A < right_A) {
        int mid = (left_A + right_A) / 2;
        
        // Query: try to separate vertices 0..mid from mid+1..N-1
        // Direct all edges from lower to higher numbered vertices
        cout << "0";
        for (int i = 0; i < M; i++) {
            if (edges[i].first < edges[i].second) {
                cout << " 0"; // U_i to V_i
            } else {
                cout << " 1"; // V_i to U_i
            }
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            // Key can reach treasure, so A <= mid
            right_A = mid;
        } else {
            // Key cannot reach treasure, so A > mid
            left_A = mid + 1;
        }
    }
    
    int A = left_A;
    
    // Binary search to find B (treasure star)
    int left_B = 0, right_B = N - 1;
    
    while (left_B < right_B) {
        int mid = (left_B + right_B) / 2;
        
        // Query: try to separate vertices 0..mid from mid+1..N-1
        // Direct all edges from higher to lower numbered vertices
        cout << "0";
        for (int i = 0; i < M; i++) {
            if (edges[i].first > edges[i].second) {
                cout << " 0"; // U_i to V_i
            } else {
                cout << " 1"; // V_i to U_i
            }
        }
        cout << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 1) {
            // Key can reach treasure, so B >= mid+1
            left_B = mid + 1;
        } else {
            // Key cannot reach treasure, so B <= mid
            right_B = mid;
        }
    }
    
    int B = left_B;
    
    cout << "1 " << A << " " << B << endl;
    cout.flush();
    
    return 0;
}