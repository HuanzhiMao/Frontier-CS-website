#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<bool> used(n + 1, false);
    
    // For each position from 1 to n
    for (int pos = 1; pos <= n; pos++) {
        // Try each unused value
        for (int val = 1; val <= n; val++) {
            if (used[val]) continue;
            
            // Check if this value can be at this position
            bool valid = true;
            
            // Check consistency with all previous positions
            for (int prev = 1; prev < pos; prev++) {
                // Query the inversion parity between prev and pos
                cout << "? " << prev << " " << pos << endl;
                cout.flush();
                int parity;
                cin >> parity;
                
                // Calculate expected parity if p[pos] = val
                int expected = 0;
                for (int i = prev; i < pos; i++) {
                    if (p[i] > val) expected++;
                }
                expected %= 2;
                
                if (expected != parity) {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                p[pos] = val;
                used[val] = true;
                break;
            }
        }
    }
    
    cout << "! ";
    for (int i = 1; i <= n; i++) {
        cout << p[i] << " ";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}