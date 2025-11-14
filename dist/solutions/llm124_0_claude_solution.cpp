#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<bool> used(n + 1, false);
    
    // Find pairs that sum to n+1
    vector<pair<int, int>> pairs;
    
    // Query all pairs to find which ones have integer average
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? 2 " << i << " " << j << endl;
            cout.flush();
            int response;
            cin >> response;
            
            if (response == 1) {
                pairs.push_back({i, j});
            }
        }
    }
    
    // The pairs with integer average are those where p[i] + p[j] = n+1
    // We need to determine the assignment
    
    // Build a graph where edges connect positions that must sum to n+1
    vector<vector<int>> adj(n + 1);
    for (auto& pair : pairs) {
        adj[pair.first].push_back(pair.second);
        adj[pair.second].push_back(pair.first);
    }
    
    // Find connected components (should be pairs)
    vector<bool> visited(n + 1, false);
    vector<pair<int, int>> complement_pairs;
    
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            visited[i] = true;
            for (int j : adj[i]) {
                if (!visited[j]) {
                    visited[j] = true;
                    complement_pairs.push_back({i, j});
                    break;
                }
            }
        }
    }
    
    // Now we need to assign values to each pair
    // For each pair (a,b), either p[a] = k and p[b] = n+1-k, or vice versa
    
    // We'll use the constraint p[1] <= n/2 to determine the assignment
    vector<int> values;
    for (int i = 1; i <= n; i++) {
        values.push_back(i);
    }
    
    // Try to determine p[1]
    bool found_p1 = false;
    
    for (auto& pair : complement_pairs) {
        if (pair.first == 1 || pair.second == 1) {
            // Position 1 is in this pair
            int other = (pair.first == 1) ? pair.second : pair.first;
            
            // Try both assignments and see which satisfies p[1] <= n/2
            for (int val = 1; val <= n/2; val++) {
                int other_val = n + 1 - val;
                if (other_val >= 1 && other_val <= n && other_val != val) {
                    p[1] = val;
                    p[other] = other_val;
                    found_p1 = true;
                    break;
                }
            }
            break;
        }
    }
    
    // Now assign the rest based on the pairs
    used[p[1]] = true;
    for (auto& pair : complement_pairs) {
        if (pair.first == 1 || pair.second == 1) continue;
        
        int pos1 = pair.first, pos2 = pair.second;
        
        // Find unused complementary values
        for (int val = 1; val <= n; val++) {
            if (!used[val] && !used[n + 1 - val]) {
                p[pos1] = val;
                p[pos2] = n + 1 - val;
                used[val] = true;
                used[n + 1 - val] = true;
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