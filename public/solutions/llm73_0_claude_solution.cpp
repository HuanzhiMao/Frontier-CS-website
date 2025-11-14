#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    
    // For each position, we'll determine the value
    for (int pos = 1; pos <= n; pos++) {
        // Binary search to find the correct value at position pos
        vector<bool> possible(n + 1, true);
        for (int i = 1; i < pos; i++) {
            possible[p[i]] = false;
        }
        
        vector<int> candidates;
        for (int i = 1; i <= n; i++) {
            if (possible[i]) {
                candidates.push_back(i);
            }
        }
        
        while (candidates.size() > 1) {
            int mid = candidates.size() / 2;
            
            // Create a test permutation with the first half of candidates at position pos
            vector<int> test_perm = p;
            test_perm[pos] = candidates[mid - 1];
            
            // Query the parity of inversions for prefix [1, pos]
            cout << "? 1 " << pos << endl;
            cout.flush();
            int parity;
            cin >> parity;
            
            // Calculate expected parity based on our test
            int expected_parity = 0;
            for (int i = 1; i < pos; i++) {
                if (p[i] > test_perm[pos]) {
                    expected_parity ^= 1;
                }
            }
            for (int i = 1; i < pos; i++) {
                for (int j = i + 1; j < pos; j++) {
                    if (p[i] > p[j]) {
                        expected_parity ^= 1;
                    }
                }
            }
            
            if (parity == expected_parity) {
                // The actual value is in the first half or equal to mid-1
                vector<int> new_candidates;
                for (int i = 0; i < mid; i++) {
                    new_candidates.push_back(candidates[i]);
                }
                candidates = new_candidates;
            } else {
                // The actual value is in the second half
                vector<int> new_candidates;
                for (int i = mid; i < candidates.size(); i++) {
                    new_candidates.push_back(candidates[i]);
                }
                candidates = new_candidates;
            }
        }
        
        p[pos] = candidates[0];
    }
    
    cout << "! ";
    for (int i = 1; i <= n; i++) {
        cout << p[i];
        if (i < n) cout << " ";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}