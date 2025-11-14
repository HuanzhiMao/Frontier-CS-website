#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> perm(n + 1);
    vector<bool> used(n + 1, false);
    
    // First, determine which positions have odd values
    vector<bool> is_odd(n + 1, false);
    for (int i = 1; i <= n; i++) {
        cout << "? 1 " << i << endl;
        cout.flush();
        int resp;
        cin >> resp;
        is_odd[i] = (resp == 1);
    }
    
    // Count odd and even positions
    vector<int> odd_pos, even_pos;
    for (int i = 1; i <= n; i++) {
        if (is_odd[i]) {
            odd_pos.push_back(i);
        } else {
            even_pos.push_back(i);
        }
    }
    
    // Now we need to figure out the exact values
    // We know that odd positions have odd values and even positions have even values
    vector<int> odd_vals, even_vals;
    for (int i = 1; i <= n; i += 2) {
        odd_vals.push_back(i);
    }
    for (int i = 2; i <= n; i += 2) {
        even_vals.push_back(i);
    }
    
    // Match odd positions to odd values
    vector<int> odd_perm(odd_pos.size());
    if (odd_pos.size() > 1) {
        for (int i = 0; i < odd_pos.size() - 1; i++) {
            for (int j = i + 1; j < odd_vals.size(); j++) {
                cout << "? 2 " << odd_pos[i] << " " << odd_pos[i + 1] << endl;
                cout.flush();
                int resp;
                cin >> resp;
                
                if (resp == 1) {
                    // Sum is even, so both have same parity relative to 2
                    // Try to determine which values they are
                    int sum_needed = odd_vals[0] + odd_vals[1];
                    for (int a = 0; a < odd_vals.size(); a++) {
                        for (int b = a + 1; b < odd_vals.size(); b++) {
                            if (odd_vals[a] + odd_vals[b] == sum_needed) {
                                odd_perm[i] = odd_vals[a];
                                odd_perm[i + 1] = odd_vals[b];
                                goto found_odd;
                            }
                        }
                    }
                }
                found_odd:;
                break;
            }
            break;
        }
    }
    
    // Simple approach: try all possibilities and verify
    vector<int> result(n + 1);
    
    // Generate a valid permutation by trial
    function<bool(int)> solve = [&](int pos) -> bool {
        if (pos > n) {
            return result[1] <= n / 2;
        }
        
        vector<int> candidates;
        if (is_odd[pos]) {
            for (int val : odd_vals) {
                if (!used[val]) candidates.push_back(val);
            }
        } else {
            for (int val : even_vals) {
                if (!used[val]) candidates.push_back(val);
            }
        }
        
        for (int val : candidates) {
            result[pos] = val;
            used[val] = true;
            
            if (solve(pos + 1)) return true;
            
            used[val] = false;
        }
        
        return false;
    };
    
    solve(1);
    
    // Ensure constraint p1 <= n/2
    if (result[1] > n / 2) {
        for (int i = 1; i <= n; i++) {
            result[i] = n + 1 - result[i];
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}