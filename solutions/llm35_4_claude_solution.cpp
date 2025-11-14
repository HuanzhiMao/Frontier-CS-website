#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        if (n == -1) break;
        
        int queries = 0;
        vector<bool> found(n + 1, false);
        
        // Strategy: Check each number x by querying all positions
        for (int x = 1; x <= n && queries < 5000; x++) {
            int count = 0;
            
            // Query first half of positions
            vector<int> first_half;
            for (int i = 1; i <= (2*n-1)/2; i++) {
                first_half.push_back(i);
            }
            
            cout << "? " << x << " " << first_half.size();
            for (int pos : first_half) {
                cout << " " << pos;
            }
            cout << endl;
            cout.flush();
            queries++;
            
            int result1;
            cin >> result1;
            if (result1 == -1) return 0;
            
            // Query second half of positions
            vector<int> second_half;
            for (int i = (2*n-1)/2 + 1; i <= 2*n-1; i++) {
                second_half.push_back(i);
            }
            
            cout << "? " << x << " " << second_half.size();
            for (int pos : second_half) {
                cout << " " << pos;
            }
            cout << endl;
            cout.flush();
            queries++;
            
            int result2;
            cin >> result2;
            if (result2 == -1) return 0;
            
            // If x appears in exactly one half, it appears once
            if ((result1 == 1 && result2 == 0) || (result1 == 0 && result2 == 1)) {
                cout << "! " << x << endl;
                cout.flush();
                break;
            }
            // If x appears in both halves, it appears twice (skip)
            // If x appears in neither half, it doesn't exist (skip)
        }
    }
    
    return 0;
}