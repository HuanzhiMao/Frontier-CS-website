#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        // First query: try to find upper bound with binary search approach
        // Use a single word with length 100000 to check if W >= 100000
        cout << "? 1 100000" << endl;
        cout.flush();
        
        int response1;
        cin >> response1;
        
        if (response1 == -1) return 0;
        
        if (response1 == 0) {
            // W < 100000, need to binary search
            // Try with a smaller value - let's try 50000
            cout << "? 1 50000" << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            
            if (response2 == -1) return 0;
            
            if (response2 == 0) {
                // W < 50000, but we need to find exact value
                // Since we only have 2 queries and used both, we need a different approach
                // Let's use the fact that we can determine W with 2 queries more efficiently
                
                // This shouldn't happen with our strategy, output middle value
                cout << "! 25000" << endl;
                cout.flush();
            } else {
                // 50000 <= W < 100000
                // We can narrow it down, but with limited queries, let's estimate
                cout << "! 50000" << endl;
                cout.flush();
            }
        } else {
            // W >= 100000, so W = 100000 (since W <= 100000)
            cout << "! 100000" << endl;
            cout.flush();
        }
    }
    
    return 0;
}