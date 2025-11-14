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
        
        int result;
        cin >> result;
        
        // Calculate expected result for addition
        long long expected_add = 1;
        for (int j = 1; j <= n; j++) {
            if (j == i + 1) {
                expected_add = (expected_add + 2) % MOD;
            } else {
                if (j <= i) {
                    expected_add = (expected_add + 1) % MOD;
                } else {
                    expected_add = (expected_add * 1) % MOD;
                }
            }
        }
        
        // We need to be more careful about the calculation
        // Let's use a different approach
        
        // Query with all 1s except position i+1 has value 2
        vector<long long> test_query(n + 1, 1);
        test_query[i + 1] = 2;
        
        // If op[i] is addition: (...(1 op1 1) op2 1)... + 2) op[i+1] 1)...
        // If op[i] is multiplication: (...(1 op1 1) op2 1)... * 2) op[i+1] 1)...
        
        // Let's try a simpler approach with base query all 1s
        if (i == 0) {
            cout << "?";
            for (int j = 0; j <= n; j++) {
                cout << " 1";
            }
            cout << endl;
            cout.flush();
            
            int base_result;
            cin >> base_result;
            
            if (result == (base_result + 1) % MOD) {
                ops[i] = 0; // addition
            } else {
                ops[i] = 1; // multiplication
            }
        } else {
            // For position i > 0, we need to consider previous operations
            // Let's calculate what the prefix result would be
            long long prefix = 1;
            for (int j = 0; j < i; j++) {
                if (ops[j] == 0) {
                    prefix = (prefix + 1) % MOD;
                } else {
                    prefix = (prefix * 1) % MOD;
                }
            }
            
            // Now check if op[i] is + or *
            long long expected_add = prefix;
            expected_add = (expected_add + 2) % MOD;
            // Continue with remaining operations (all 1s)
            for (int j = i + 1; j < n; j++) {
                expected_add = (expected_add + 1) % MOD; // assume remaining are +
            }
            
            long long expected_mul = prefix;
            expected_mul = (expected_mul * 2) % MOD;
            for (int j = i + 1; j < n; j++) {
                expected_mul = (expected_mul + 1) % MOD; // assume remaining are +
            }
            
            // This approach is getting complex, let's use binary approach
            vector<long long> simple_query(n + 1, 1);
            simple_query[0] = 0;
            simple_query[i + 1] = 1;
            
            cout << "?";
            for (int j = 0; j <= n; j++) {
                cout << " " << simple_query[j];
            }
            cout << endl;
            cout.flush();
            
            int result2;
            cin >> result2;
            
            // Compare with query where a[i+1] = 0
            simple_query[i + 1] = 0;
            cout << "?";
            for (int j = 0; j <= n; j++) {
                cout << " " << simple_query[j];
            }
            cout << endl;
            cout.flush();
            
            int result3;
            cin >> result3;
            
            if (result2 == result3) {
                ops[i] = 1; // multiplication (0*anything = 0, so changing doesn't matter after multiplication by 0)
            } else {
                ops[i] = 0; // addition
            }
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