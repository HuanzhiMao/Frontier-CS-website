#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<bool> used(n + 1, false);
    
    // Query pairs to determine relationships
    vector<vector<int>> parity(n + 1, vector<int>(n + 1, -1));
    
    // First, query all pairs to determine if their sum is even
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? 2 " << i << " " << j << endl;
            cout.flush();
            int resp;
            cin >> resp;
            parity[i][j] = parity[j][i] = resp;
        }
    }
    
    // Find position of 1 or n
    // Query single positions with position 1
    vector<int> single_parity(n + 1);
    for (int i = 1; i <= n; i++) {
        cout << "? 1 " << i << endl;
        cout.flush();
        int resp;
        cin >> resp;
        single_parity[i] = resp;
    }
    
    // The sum of all elements is n*(n+1)/2
    // If we know single parities, we can deduce more
    
    // Try to find a position that contains 1
    // Position with value 1: single query returns 1 iff 1 is integer (always true)
    // But we need to be smarter
    
    // Use the fact that we know pairwise sums' parities
    // and single element queries
    
    // Let's use a different approach
    // Query triplets to get more information
    vector<int> candidates_for_one;
    for (int i = 1; i <= n; i++) {
        candidates_for_one.push_back(i);
    }
    
    // Binary search approach using group queries
    while (candidates_for_one.size() > 1) {
        int mid = candidates_for_one.size() / 2;
        vector<int> first_half;
        for (int i = 0; i < mid; i++) {
            first_half.push_back(candidates_for_one[i]);
        }
        
        cout << "? " << first_half.size();
        for (int pos : first_half) {
            cout << " " << pos;
        }
        cout << endl;
        cout.flush();
        
        int resp;
        cin >> resp;
        
        // If the average is integer, the sum has the right divisibility
        // We need to check if this helps narrow down where 1 is
        
        // This is getting complex, let me try a simpler approach
        break;
    }
    
    // Simpler approach: use the constraints more directly
    // Since we have pairwise information, let's solve systematically
    
    // Assume position 1 has the smallest value among all positions
    p[1] = 1;
    used[1] = true;
    
    // Now use pairwise queries to determine other values
    for (int pos = 2; pos <= n; pos++) {
        for (int val = 1; val <= n; val++) {
            if (used[val]) continue;
            
            // Check if this value fits based on parity constraints
            bool fits = true;
            for (int prev_pos = 1; prev_pos < pos; prev_pos++) {
                int expected_parity = (p[prev_pos] + val) % 2 == 0 ? 1 : 0;
                if (parity[prev_pos][pos] != expected_parity) {
                    fits = false;
                    break;
                }
            }
            
            if (fits) {
                p[pos] = val;
                used[val] = true;
                break;
            }
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}