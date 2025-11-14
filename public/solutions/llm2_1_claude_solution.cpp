#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> perm(n + 1, 0);
    vector<bool> used(n + 1, false);
    
    // For each position, try each number from 1 to n
    for (int pos = 1; pos <= n; pos++) {
        vector<int> query(n + 1, 1);
        
        // Set all known positions
        for (int i = 1; i < pos; i++) {
            query[i] = perm[i];
        }
        
        // Query with current position set to 1, rest unknown positions to 1
        cout << "0";
        for (int i = 1; i <= n; i++) {
            cout << " " << query[i];
        }
        cout << endl;
        cout.flush();
        
        int baseline;
        cin >> baseline;
        
        int found = -1;
        for (int val = 1; val <= n; val++) {
            if (used[val]) continue;
            
            // Try this value at current position
            query[pos] = val;
            
            cout << "0";
            for (int i = 1; i <= n; i++) {
                cout << " " << query[i];
            }
            cout << endl;
            cout.flush();
            
            int result;
            cin >> result;
            
            if (result > baseline) {
                found = val;
                break;
            }
        }
        
        perm[pos] = found;
        used[found] = true;
    }
    
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << perm[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}