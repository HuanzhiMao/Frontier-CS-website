#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // First query with all indices to get overall medians
    cout << "0 " << n;
    for (int i = 1; i <= n; i++) {
        cout << " " << i;
    }
    cout << endl;
    cout.flush();
    
    int m1, m2;
    cin >> m1 >> m2;
    
    // Find positions of m1 and m2
    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    // Try to find which positions contain m1 and m2
    vector<int> pos_m1, pos_m2;
    
    // Query subsets to narrow down positions
    for (int i = 1; i <= n; i++) {
        vector<int> query_indices;
        int added = 0;
        
        // Add position i
        query_indices.push_back(i);
        added++;
        
        // Add other positions to make even length >= 4
        for (int j = 1; j <= n && added < 4; j++) {
            if (j != i) {
                query_indices.push_back(j);
                added++;
            }
        }
        
        // Make sure we have even length
        if (query_indices.size() % 2 == 1) {
            for (int j = 1; j <= n; j++) {
                if (find(query_indices.begin(), query_indices.end(), j) == query_indices.end()) {
                    query_indices.push_back(j);
                    break;
                }
            }
        }
        
        cout << "0 " << query_indices.size();
        for (int idx : query_indices) {
            cout << " " << idx;
        }
        cout << endl;
        cout.flush();
        
        int q1, q2;
        cin >> q1 >> q2;
        
        if (q1 == m1 && q2 == m2) {
            // This position might contain one of our medians
            if (pos_m1.empty()) {
                pos_m1.push_back(i);
            } else if (pos_m2.empty()) {
                pos_m2.push_back(i);
            }
        }
        
        if (pos_m1.size() == 1 && pos_m2.size() == 1) {
            break;
        }
    }
    
    // If we haven't found them yet, try a different approach
    if (pos_m1.empty() || pos_m2.empty()) {
        // Binary search approach
        for (int target : {m1, m2}) {
            for (int pos = 1; pos <= n; pos++) {
                // Create a query that should help identify if target is at pos
                vector<int> query_indices = {pos};
                
                // Add other indices to reach size 4
                for (int j = 1; j <= n && query_indices.size() < 4; j++) {
                    if (j != pos) {
                        query_indices.push_back(j);
                    }
                }
                
                cout << "0 " << query_indices.size();
                for (int idx : query_indices) {
                    cout << " " << idx;
                }
                cout << endl;
                cout.flush();
                
                int q1, q2;
                cin >> q1 >> q2;
                
                // Check if this helps us identify the position
                if ((target == m1 && pos_m1.empty() && (q1 == m1 || q2 == m1)) ||
                    (target == m2 && pos_m2.empty() && (q1 == m2 || q2 == m2))) {
                    if (target == m1) pos_m1.push_back(pos);
                    else pos_m2.push_back(pos);
                    break;
                }
            }
        }
    }
    
    // Output the answer
    int ans1 = pos_m1.empty() ? 1 : pos_m1[0];
    int ans2 = pos_m2.empty() ? 2 : pos_m2[0];
    
    cout << "1 " << ans1 << " " << ans2 << endl;
    cout.flush();
    
    return 0;
}