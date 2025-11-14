#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> queries(n + 1, vector<int>(n + 1, -1));
    
    // Function to query
    auto query = [&](int i, int j) -> int {
        if (queries[i][j] != -1) return queries[i][j];
        cout << "? " << i << " " << j << "\n";
        cout.flush();
        int res;
        cin >> res;
        if (res == -1) exit(0);
        queries[i][j] = queries[j][i] = res;
        return res;
    };
    
    // Try to determine the highest bit position needed
    int max_bit = 0;
    while ((1 << max_bit) < n) max_bit++;
    
    // For each bit position, determine which positions have that bit set
    for (int bit = 0; bit < max_bit; bit++) {
        vector<int> has_bit, no_bit;
        
        // First, try to separate positions into two groups based on this bit
        // We'll use a greedy approach to minimize queries
        
        for (int i = 1; i <= n; i++) {
            bool found = false;
            
            // Try to determine if position i has this bit set
            for (int j : no_bit) {
                int or_val = query(i, j);
                if (or_val & (1 << bit)) {
                    has_bit.push_back(i);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                for (int j : has_bit) {
                    int or_val = query(i, j);
                    if (!(or_val & (1 << bit))) {
                        no_bit.push_back(i);
                        found = true;
                        break;
                    }
                }
            }
            
            if (!found) {
                if (has_bit.empty()) {
                    has_bit.push_back(i);
                } else {
                    no_bit.push_back(i);
                }
            }
        }
        
        // Set the bit for positions in has_bit
        for (int i : has_bit) {
            p[i] |= (1 << bit);
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << "\n";
    cout.flush();
    
    return 0;
}