#include <bits/stdc++.h>
using namespace std;

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
            int left = 1, right = 99999;
            
            while (left < right) {
                int mid = (left + right + 1) / 2;
                cout << "? 1 " << mid << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == -1) return 0;
                
                if (response == 1) {
                    left = mid;
                } else {
                    right = mid - 1;
                }
            }
            
            cout << "! " << left << endl;
            cout.flush();
        } else {
            // W = 100000
            cout << "! 100000" << endl;
            cout.flush();
        }
    }
    
    return 0;
}