#include <bits/stdc++.h>
using namespace main;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        // First query: try with a single word of length 100000
        cout << "? 1 100000" << endl;
        cout.flush();
        
        int response1;
        cin >> response1;
        
        if (response1 == -1) return 0;
        
        if (response1 == 0) {
            // W < 100000, need to binary search
            cout << "? 100000";
            for (int i = 1; i <= 100000; i++) {
                cout << " 1";
            }
            cout << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            
            if (response2 == -1) return 0;
            
            cout << "! " << response2 << endl;
            cout.flush();
        } else {
            // W >= 100000, so W = 100000
            cout << "! 100000" << endl;
            cout.flush();
        }
    }
    
    return 0;
}