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
        
        int ans = -1;
        
        for (int x = 1; x <= n; x++) {
            // Query if x appears in positions 1 to n-1
            cout << "? " << x << " " << n-1 << " ";
            for (int i = 1; i <= n-1; i++) {
                cout << i << " ";
            }
            cout << endl;
            cout.flush();
            
            int response1;
            cin >> response1;
            if (response1 == -1) return 0;
            
            // Query if x appears in positions 2 to n
            cout << "? " << x << " " << n-1 << " ";
            for (int i = 2; i <= n; i++) {
                cout << i << " ";
            }
            cout << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            if (response2 == -1) return 0;
            
            // If x appears in both ranges, it appears twice
            // If x appears in exactly one range, it appears once
            if (response1 + response2 == 1) {
                ans = x;
                break;
            }
        }
        
        cout << "! " << ans << endl;
        cout.flush();
    }
    
    return 0;
}