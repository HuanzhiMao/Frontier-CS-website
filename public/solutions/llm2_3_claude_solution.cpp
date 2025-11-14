#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> perm(n + 1);
    vector<set<int>> candidates(n + 1);
    
    // Initialize candidates
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            candidates[i].insert(j);
        }
    }
    
    // Query each position with each possible value
    for (int val = 1; val <= n; val++) {
        vector<int> query(n + 1, val);
        cout << "0";
        for (int i = 1; i <= n; i++) {
            cout << " " << query[i];
        }
        cout << endl;
        cout.flush();
        
        int matches;
        cin >> matches;
        
        // Update candidates based on the response
        int remaining_positions = 0;
        for (int i = 1; i <= n; i++) {
            if (candidates[i].count(val)) {
                remaining_positions++;
            }
        }
        
        if (matches == 0) {
            // val is not in the permutation at any position we consider
            for (int i = 1; i <= n; i++) {
                candidates[i].erase(val);
            }
        } else {
            // val appears exactly 'matches' times in correct positions
            // We need more queries to determine exact positions
        }
    }
    
    // Use process of elimination with targeted queries
    vector<bool> fixed(n + 1, false);
    
    while (true) {
        bool found_new = false;
        
        // Check if any position has only one candidate left
        for (int pos = 1; pos <= n; pos++) {
            if (!fixed[pos] && candidates[pos].size() == 1) {
                int val = *candidates[pos].begin();
                perm[pos] = val;
                fixed[pos] = true;
                found_new = true;
                
                // Remove this value from all other positions
                for (int i = 1; i <= n; i++) {
                    if (i != pos) {
                        candidates[i].erase(val);
                    }
                }
            }
        }
        
        if (!found_new) {
            // Use additional queries to resolve ambiguity
            int pos = -1, val = -1;
            for (int i = 1; i <= n; i++) {
                if (!fixed[i] && candidates[i].size() > 1) {
                    pos = i;
                    val = *candidates[i].begin();
                    break;
                }
            }
            
            if (pos == -1) break; // All positions determined
            
            // Query with val at position pos
            vector<int> query(n + 1);
            for (int i = 1; i <= n; i++) {
                if (i == pos) {
                    query[i] = val;
                } else {
                    // Use a value that's definitely not at position i
                    for (int v = 1; v <= n; v++) {
                        if (candidates[i].count(v) == 0) {
                            query[i] = v;
                            break;
                        }
                    }
                }
            }
            
            cout << "0";
            for (int i = 1; i <= n; i++) {
                cout << " " << query[i];
            }
            cout << endl;
            cout.flush();
            
            int matches;
            cin >> matches;
            
            if (matches > 0) {
                // val is at position pos
                perm[pos] = val;
                fixed[pos] = true;
                for (int i = 1; i <= n; i++) {
                    if (i != pos) {
                        candidates[i].erase(val);
                    }
                }
            } else {
                // val is not at position pos
                candidates[pos].erase(val);
            }
        }
        
        // Check if we're done
        bool all_fixed = true;
        for (int i = 1; i <= n; i++) {
            if (!fixed[i]) {
                all_fixed = false;
                break;
            }
        }
        if (all_fixed) break;
    }
    
    // Output the final permutation
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << perm[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}