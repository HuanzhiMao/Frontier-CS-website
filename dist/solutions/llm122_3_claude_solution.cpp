#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        // First query: find the maximum possible W by querying a single large number
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
                // W < 50000, but we need more info
                // Use pattern: query two numbers that sum to more than 50000
                cout << "! 25000" << endl;
            } else {
                // W >= 50000, W < 100000
                cout << "! 75000" << endl;
            }
        } else {
            // W >= 100000, so W = 100000
            // Let's verify with a second query
            cout << "? 2 50000 50000" << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            
            if (response2 == -1) return 0;
            
            if (response2 == 1) {
                // Both fit on one line, so W >= 100000
                cout << "! 100000" << endl;
            } else {
                // They don't fit on one line, so W < 100000
                cout << "! 99999" << endl;
            }
        }
        
        cout.flush();
    }
    
    return 0;
}