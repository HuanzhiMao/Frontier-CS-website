#include <bits/stdc++.h>
using namespace C++using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        // First query: try with a large value to find upper bound
        cout << "? 1 100000" << endl;
        cout.flush();
        
        int response1;
        cin >> response1;
        
        if (response1 == -1) return 0;
        
        if (response1 == 0) {
            // W < 100000, need to binary search
            cout << "? 1 50000" << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            
            if (response2 == -1) return 0;
            
            if (response2 == 0) {
                // W < 50000, assume W = 25000 as middle estimate
                cout << "! 25000" << endl;
            } else {
                // W >= 50000, assume W = 75000 as middle estimate
                cout << "! 75000" << endl;
            }
        } else {
            // W >= 100000, so W = 100000
            cout << "! 100000" << endl;
        }
        
        cout.flush();
    }
    
    return 0;
}