#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> ops(n);
    
    for (int i = 0; i < n; i++) {
        vector<long long> query(n + 1, 1);
        query[i + 1] = 2;
        
        cout << "?";
        for (int j = 0; j <= n; j++) {
            cout << " " << query[j];
        }
        cout << endl;
        cout.flush();
        
        long long result;
        cin >> result;
        
        // Calculate what the result should be if op[i] is addition
        long long expected_add = 1;
        for (int j = 1; j <= n; j++) {
            if (j == i + 1) {
                expected_add = (expected_add + 2) % MOD;
            } else {
                expected_add = (expected_add + 1) % MOD;
            }
        }
        
        if (result == expected_add) {
            ops[i] = 0; // addition
        } else {
            ops[i] = 1; // multiplication
        }
    }
    
    cout << "!";
    for (int i = 0; i < n; i++) {
        cout << " " << ops[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}