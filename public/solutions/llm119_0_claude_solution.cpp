#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> ops(n + 1);
    
    // Query with all 1s and all 2s to get base cases
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
    
    // Determine operators one by one
    for (int pos = 1; pos <= n; pos++) {
        // Create query where we change only position pos
        vector<int> query(n + 1, 1);
        query[pos] = 2;
        
        cout << "?";
        for (int i = 0; i <= n; i++) {
            cout << " " << query[i];
        }
        cout << endl;
        cout.flush();
        
        int result;
        cin >> result;
        
        // Simulate both operations to see which one matches
        // First simulate with addition
        long long sim_add = 1;
        for (int i = 1; i < pos; i++) {
            if (ops[i] == 0) { // addition
                sim_add = (sim_add + 1) % MOD;
            } else { // multiplication
                sim_add = (sim_add * 1) % MOD;
            }
        }
        sim_add = (sim_add + 2) % MOD; // add operation at pos
        for (int i = pos + 1; i <= n; i++) {
            sim_add = (sim_add + 1) % MOD; // assume remaining are additions for now
        }
        
        // Simulate with multiplication
        long long sim_mult = 1;
        for (int i = 1; i < pos; i++) {
            if (ops[i] == 0) { // addition
                sim_mult = (sim_mult + 1) % MOD;
            } else { // multiplication
                sim_mult = (sim_mult * 1) % MOD;
            }
        }
        sim_mult = (sim_mult * 2) % MOD; // mult operation at pos
        for (int i = pos + 1; i <= n; i++) {
            sim_mult = (sim_mult + 1) % MOD; // assume remaining are additions for now
        }
        
        // We need a different approach - use the fact that changing one position
        // and seeing the difference can tell us about the operator
        
        // Let's use a simpler approach: test each position individually
        vector<int> test_query(n + 1, 1);
        test_query[0] = 2; // base case
        
        if (pos == 1) {
            // For position 1, we already have the query result
            // Check if result matches addition or multiplication
            // 2 + 1 = 3 vs 2 * 1 = 2 for remaining all 1s
            // But we need to account for remaining operations
            
            // Use a different strategy: binary search approach
            // Test with specific values that make it easier to distinguish
            vector<int> add_test(n + 1, 0);
            add_test[0] = 3;
            add_test[1] = 5;
            for (int i = 2; i <= n; i++) add_test[i] = 1;
            
            cout << "?";
            for (int i = 0; i <= n; i++) {
                cout << " " << add_test[i];
            }
            cout << endl;
            cout.flush();
            
            int add_result;
            cin >> add_result;
            
            // If op1 is +: (3 + 5) = 8, then continue
            // If op1 is *: (3 * 5) = 15, then continue
            // We can determine by the final result since 8 != 15
            ops[1] = (add_result == (8 + n - 1) % MOD) ? 0 : 1;
        } else {
            // For other positions, use the cumulative approach
            vector<int> pos_test(n + 1, 1);
            pos_test[pos] = 3;
            
            cout << "?";
            for (int i = 0; i <= n; i++) {
                cout << " " << pos_test[i];
            }
            cout << endl;
            cout.flush();
            
            int pos_result;
            cin >> pos_result;
            
            // Simulate what the result should be with addition vs multiplication
            long long sim = 1;
            for (int i = 1; i < pos; i++) {
                if (ops[i] == 0) {
                    sim = (sim + 1) % MOD;
                } else {
                    sim = (sim * 1) % MOD;
                }
            }
            
            long long add_sim = (sim + 3) % MOD;
            long long mult_sim = (sim * 3) % MOD;
            
            // Continue with remaining 1s (assume addition)
            add_sim = (add_sim + (n - pos)) % MOD;
            mult_sim = (mult_sim + (n - pos)) % MOD;
            
            ops[pos] = (pos_result == add_sim) ? 0 : 1;
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