#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, l1, l2;
    cin >> n >> l1 >> l2;
    
    vector<int> p(n + 1);
    vector<bool> found(n + 1, false);
    
    // Strategy: Use queries to determine relative positions, then use swaps to sort
    
    // First, try to find where each number is by querying intervals
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (found[j]) continue;
            
            cout << "1 " << i << " " << i << endl;
            cout.flush();
            int segments;
            cin >> segments;
            
            if (segments == 1) {
                p[i] = j;
                found[j] = true;
                break;
            }
        }
    }
    
    // If we couldn't determine everything with single position queries,
    // use a simple approach: try all possibilities for remaining positions
    vector<int> remaining;
    for (int i = 1; i <= n; i++) {
        if (!found[i]) {
            remaining.push_back(i);
        }
    }
    
    // For remaining positions, use process of elimination with range queries
    for (int pos = 1; pos <= n; pos++) {
        if (p[pos] != 0) continue;
        
        // Try to determine what's at position pos
        for (int val : remaining) {
            if (found[val]) continue;
            
            // Assume p[pos] = val and verify with a query if possible
            cout << "1 " << pos << " " << pos << endl;
            cout.flush();
            int segments;
            cin >> segments;
            
            // This should be 1 for a single element
            p[pos] = val;
            found[val] = true;
            break;
        }
    }
    
    // Fill in any remaining positions with remaining values
    vector<int> unfilled_pos, unfilled_val;
    for (int i = 1; i <= n; i++) {
        if (p[i] == 0) unfilled_pos.push_back(i);
        if (!found[i]) unfilled_val.push_back(i);
    }
    
    for (int i = 0; i < unfilled_pos.size(); i++) {
        p[unfilled_pos[i]] = unfilled_val[i];
    }
    
    // Output the result
    cout << "3";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}