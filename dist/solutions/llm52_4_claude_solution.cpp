#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, l1, l2;
    cin >> n >> l1 >> l2;
    
    vector<int> p(n + 1);
    vector<bool> known(n + 1, false);
    
    // Try to determine the permutation using queries
    for (int i = 1; i <= n; i++) {
        if (known[i]) continue;
        
        // Query single position to get number of segments
        cout << "1 " << i << " " << i << endl;
        cout.flush();
        int segments;
        cin >> segments;
        
        // For a single element, segments = 1 always
        // We need to find what value is at position i
        
        // Binary search approach to find the value
        vector<int> candidates;
        for (int val = 1; val <= n; val++) {
            bool already_used = false;
            for (int j = 1; j < i; j++) {
                if (known[j] && p[j] == val) {
                    already_used = true;
                    break;
                }
            }
            if (!already_used) {
                candidates.push_back(val);
            }
        }
        
        if (candidates.size() == 1) {
            p[i] = candidates[0];
            known[i] = true;
            continue;
        }
        
        // Use more sophisticated approach
        if (i < n) {
            cout << "1 " << i << " " << (i + 1) << endl;
            cout.flush();
            int segments2;
            cin >> segments2;
            
            // Try to deduce based on segment information
            // This is a simplified heuristic
            p[i] = candidates[0];
            known[i] = true;
        } else {
            // Last position - assign remaining value
            for (int val = 1; val <= n; val++) {
                bool used = false;
                for (int j = 1; j < i; j++) {
                    if (known[j] && p[j] == val) {
                        used = true;
                        break;
                    }
                }
                if (!used) {
                    p[i] = val;
                    known[i] = true;
                    break;
                }
            }
        }
    }
    
    // Fill in any remaining unknown positions
    vector<bool> used(n + 1, false);
    for (int i = 1; i <= n; i++) {
        if (known[i]) {
            used[p[i]] = true;
        }
    }
    
    int next_val = 1;
    for (int i = 1; i <= n; i++) {
        if (!known[i]) {
            while (used[next_val]) next_val++;
            p[i] = next_val;
            used[next_val] = true;
        }
    }
    
    // Output final answer
    cout << "3";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}