#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> ops(n + 1);
    
    for (int i = 1; i <= n; i++) {
        vector<int> query(n + 1, 1);
        query[i] = 2;
        
        cout << "?";
        for (int j = 0; j <= n; j++) {
            cout << " " << query[j];
        }
        cout << endl;
        cout.flush();
        
        int result1;
        cin >> result1;
        
        query[i] = 3;
        cout << "?";
        for (int j = 0; j <= n; j++) {
            cout << " " << query[j];
        }
        cout << endl;
        cout.flush();
        
        int result2;
        cin >> result2;
        
        // If operation is addition: (prev + 2) vs (prev + 3) -> diff = 1
        // If operation is multiplication: (prev * 2) vs (prev * 3) -> diff = prev
        
        if ((result2 - result1 + MOD) % MOD == 1) {
            ops[i] = 0; // addition
        } else {
            ops[i] = 1; // multiplication
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << ops[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}