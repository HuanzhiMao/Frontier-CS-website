#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> perm(n + 1);
    vector<bool> used(n + 1, false);
    
    // Find pairs that sum to n+1
    vector<pair<int, int>> pairs;
    
    for (int i = 1; i <= n; i++) {
        if (used[i]) continue;
        
        for (int j = i + 1; j <= n; j++) {
            if (used[j]) continue;
            
            cout << "? 2 " << i << " " << j << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            if (response == 1) {
                pairs.push_back({i, j});
                used[i] = used[j] = true;
                break;
            }
        }
    }
    
    // Now we need to determine which element in each pair gets which value
    // We know that pairs sum to n+1, so if pair is (a,b), then p[a] + p[b] = n+1
    
    vector<int> assignment(n/2 + 1);
    vector<bool> value_used(n + 1, false);
    
    // Try to assign values 1 to n/2 to one element from each pair
    for (int val = 1; val <= n/2; val++) {
        bool found = false;
        
        for (int i = 0; i < pairs.size() && !found; i++) {
            if (assignment[val] != 0) break;
            
            int pos1 = pairs[i].first;
            int pos2 = pairs[i].second;
            
            if (perm[pos1] != 0 || perm[pos2] != 0) continue;
            
            // Try assigning val to pos1
            vector<int> query_pos;
            query_pos.push_back(pos1);
            
            // Add other positions we know have values <= n/2
            for (int j = 1; j <= n; j++) {
                if (j != pos1 && perm[j] != 0 && perm[j] <= n/2) {
                    query_pos.push_back(j);
                }
            }
            
            if (query_pos.size() > 1) {
                cout << "? " << query_pos.size();
                for (int pos : query_pos) cout << " " << pos;
                cout << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                // Calculate what the sum would be if pos1 has value val
                int sum = val;
                for (int j = 1; j < query_pos.size(); j++) {
                    sum += perm[query_pos[j]];
                }
                
                bool would_be_integer = (sum * 2) % (2 * query_pos.size()) == 0;
                
                if ((response == 1) == would_be_integer) {
                    perm[pos1] = val;
                    perm[pos2] = n + 1 - val;
                    found = true;
                } else {
                    perm[pos1] = n + 1 - val;
                    perm[pos2] = val;
                    found = true;
                }
            } else {
                // Just assign arbitrarily and fix later if needed
                perm[pos1] = val;
                perm[pos2] = n + 1 - val;
                found = true;
            }
        }
        
        if (!found) {
            // Find unassigned pair
            for (int i = 0; i < pairs.size(); i++) {
                int pos1 = pairs[i].first;
                int pos2 = pairs[i].second;
                if (perm[pos1] == 0) {
                    perm[pos1] = val;
                    perm[pos2] = n + 1 - val;
                    break;
                }
            }
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << perm[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}