#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> ops(n + 1);
    
    // Query with all 1s and all 2s to get baseline
    vector<int> query1(n + 1, 1);
    vector<int> query2(n + 1, 2);
    
    cout << "?";
    for (int i = 0; i <= n; i++) {
        cout << " " << query1[i];
    }
    cout << endl;
    cout.flush();
    
    int result1;
    cin >> result1;
    
    cout << "?";
    for (int i = 0; i <= n; i++) {
        cout << " " << query2[i];
    }
    cout << endl;
    cout.flush();
    
    int result2;
    cin >> result2;
    
    // Binary search approach
    for (int i = 1; i <= n; i++) {
        // Test if operator i is + or *
        // Use query: 1, 1, ..., 1, 0, 1, 1, ..., 1 (0 at position i)
        vector<int> query(n + 1, 1);
        query[i] = 0;
        
        cout << "?";
        for (int j = 0; j <= n; j++) {
            cout << " " << query[j];
        }
        cout << endl;
        cout.flush();
        
        int result;
        cin >> result;
        
        // If operator i is +, result should be n (since we have n additions of 1, but one position is 0)
        // If operator i is *, result should be 0 (since we multiply by 0)
        if (result == 0) {
            ops[i] = 1; // multiplication
        } else {
            ops[i] = 0; // addition
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