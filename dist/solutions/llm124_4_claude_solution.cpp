#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<bool> used(n + 1, false);
    
    // First, find pairs that sum to n+1
    vector<pair<int, int>> pairs;
    vector<bool> paired(n + 1, false);
    
    for (int i = 1; i <= n; i++) {
        if (paired[i]) continue;
        
        for (int j = i + 1; j <= n; j++) {
            if (paired[j]) continue;
            
            // Check if positions i and j form a pair (sum to n+1)
            cout << "? 2 " << i << " " << j << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            if (response == 1) {
                pairs.push_back({i, j});
                paired[i] = paired[j] = true;
                break;
            }
        }
    }
    
    // Now we need to determine the actual values
    // We know that for each pair (i,j), p[i] + p[j] = n + 1
    
    // Try to determine which element in each pair gets the smaller value
    vector<int> result(n + 1);
    vector<bool> determined(n + 1, false);
    
    // Start with first pair - we know p[1] <= n/2
    int pos1 = pairs[0].first, pos2 = pairs[0].second;
    
    if (pos1 == 1) {
        // We know p[1] <= n/2, so determine the smaller value
        for (int val = 1; val <= n/2; val++) {
            bool works = true;
            result[pos1] = val;
            result[pos2] = n + 1 - val;
            determined[pos1] = determined[pos2] = true;
            
            // Try to determine other pairs based on this
            for (int k = 1; k < pairs.size(); k++) {
                int a = pairs[k].first, b = pairs[k].second;
                
                // Query with one known position and one unknown
                cout << "? 3 " << pos1 << " " << a << " " << b << endl;
                cout.flush();
                
                int resp;
                cin >> resp;
                
                // (p[pos1] + p[a] + p[b]) should be integer iff p[pos1] + n + 1 is even
                bool should_be_int = (val + n + 1) % 2 == 0;
                
                if ((resp == 1) == should_be_int) {
                    // This assignment works so far
                    continue;
                } else {
                    works = false;
                    break;
                }
            }
            
            if (works) break;
        }
    } else if (pos2 == 1) {
        for (int val = 1; val <= n/2; val++) {
            result[pos2] = val;
            result[pos1] = n + 1 - val;
            break;
        }
        determined[pos1] = determined[pos2] = true;
    } else {
        // Neither position is 1, need different approach
        result[pos1] = 1;  // Assume smaller value
        result[pos2] = n;
        determined[pos1] = determined[pos2] = true;
    }
    
    // Determine remaining pairs
    for (int i = 1; i < pairs.size(); i++) {
        int a = pairs[i].first, b = pairs[i].second;
        if (determined[a]) continue;
        
        // Find a determined position to help
        int known_pos = -1;
        for (int j = 1; j <= n; j++) {
            if (determined[j]) {
                known_pos = j;
                break;
            }
        }
        
        cout << "? 3 " << known_pos << " " << a << " " << b << endl;
        cout.flush();
        
        int resp;
        cin >> resp;
        
        // Try both assignments and see which makes sense
        for (int val = 1; val <= n; val++) {
            if (used[val] || used[n + 1 - val]) continue;
            
            int sum = result[known_pos] + val + (n + 1 - val);
            bool should_be_int = (sum * 2) % 6 == 0;  // sum/3 is integer
            
            if ((resp == 1) == (sum % 3 == 0)) {
                result[a] = val;
                result[b] = n + 1 - val;
                used[val] = used[n + 1 - val] = true;
                determined[a] = determined[b] = true;
                break;
            }
        }
    }
    
    cout << "! ";
    for (int i = 1; i <= n; i++) {
        cout << result[i];
        if (i < n) cout << " ";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}