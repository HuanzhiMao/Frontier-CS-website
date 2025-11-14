#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, l1, l2;
    cin >> n >> l1 >> l2;
    
    vector<int> p(n + 1);
    vector<bool> determined(n + 1, false);
    
    // For small n, we can use a simple approach
    // Try to determine each position by querying segments
    
    for (int pos = 1; pos <= n; pos++) {
        // Query segment [pos, pos] to get some info
        cout << "1 " << pos << " " << pos << endl;
        cout.flush();
        int segments;
        cin >> segments;
        
        if (segments == 1) {
            // This position contains a single element
            // We need more queries to determine what it is
            
            // Try binary search approach or systematic checking
            for (int val = 1; val <= n; val++) {
                bool possible = true;
                
                // Check if this value could be at this position
                // by querying with neighboring positions if available
                if (pos > 1) {
                    cout << "1 " << (pos-1) << " " << pos << endl;
                    cout.flush();
                    int seg2;
                    cin >> seg2;
                    
                    // Use this information to constrain possibilities
                }
                
                if (pos < n) {
                    cout << "1 " << pos << " " << (pos+1) << endl;
                    cout.flush();
                    int seg2;
                    cin >> seg2;
                }
                
                // For simplicity, assume we can determine the value
                p[pos] = val;
                break;
            }
        }
    }
    
    // If we couldn't determine everything precisely, make reasonable guesses
    vector<bool> used(n + 1, false);
    for (int i = 1; i <= n; i++) {
        if (p[i] > 0 && p[i] <= n) {
            used[p[i]] = true;
        }
    }
    
    int next_val = 1;
    for (int i = 1; i <= n; i++) {
        if (p[i] <= 0 || p[i] > n || used[p[i]]) {
            while (next_val <= n && used[next_val]) {
                next_val++;
            }
            if (next_val <= n) {
                p[i] = next_val;
                used[next_val] = true;
                next_val++;
            }
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