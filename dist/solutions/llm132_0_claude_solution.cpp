#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int R, H;
    cin >> R >> H;
    
    vector<int> robots_sent;
    vector<vector<int>> queries;
    
    // Send 10 robots with 100 positions each to cover all 1000 positions
    for (int i = 0; i < 10; i++) {
        cout << "? 100";
        vector<int> positions;
        for (int j = 1; j <= 100; j++) {
            positions.push_back(i * 100 + j);
            cout << " " << (i * 100 + j);
        }
        cout << endl;
        cout.flush();
        queries.push_back(positions);
    }
    
    // Get results
    cout << "@" << endl;
    cout.flush();
    
    int L;
    cin >> L;
    vector<int> results(L);
    for (int i = 0; i < L; i++) {
        cin >> results[i];
    }
    
    // Find which groups have chairmen
    vector<int> positive_groups;
    for (int i = 0; i < L; i++) {
        if (results[i] == 1) {
            positive_groups.push_back(i);
        }
    }
    
    if (positive_groups.size() == 1) {
        // Both chairmen are in the same group
        int group = positive_groups[0];
        int start = group * 100 + 1;
        int end = (group + 1) * 100;
        
        // Binary search within the group
        vector<int> candidates;
        for (int i = start; i <= end; i++) {
            candidates.push_back(i);
        }
        
        // Send individual queries to find exact positions
        for (int pos : candidates) {
            cout << "? 1 " << pos << endl;
            cout.flush();
        }
        
        cout << "@" << endl;
        cout.flush();
        
        cin >> L;
        vector<int> individual_results(L);
        for (int i = 0; i < L; i++) {
            cin >> individual_results[i];
        }
        
        vector<int> chairman_positions;
        for (int i = 0; i < L; i++) {
            if (individual_results[i] == 1) {
                chairman_positions.push_back(candidates[i]);
            }
        }
        
        if (chairman_positions.size() >= 2) {
            cout << "! " << chairman_positions[0] << " " << chairman_positions[1] << endl;
        } else if (chairman_positions.size() == 1) {
            cout << "! " << chairman_positions[0] << " " << chairman_positions[0] << endl;
        }
    } else if (positive_groups.size() >= 2) {
        // Chairmen are in different groups
        // Binary search in each group
        vector<int> final_positions;
        
        for (int group_idx = 0; group_idx < min(2, (int)positive_groups.size()); group_idx++) {
            int group = positive_groups[group_idx];
            int start = group * 100 + 1;
            int end = (group + 1) * 100;
            
            int left = start, right = end;
            while (left < right) {
                int mid = (left + right) / 2;
                cout << "? " << (mid - start + 1);
                for (int i = start; i <= mid; i++) {
                    cout << " " << i;
                }
                cout << endl;
                cout.flush();
                
                cout << "@" << endl;
                cout.flush();
                
                int L_temp;
                cin >> L_temp;
                int result;
                cin >> result;
                
                if (result == 1) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }
            final_positions.push_back(left);
        }
        
        if (final_positions.size() >= 2) {
            cout << "! " << final_positions[0] << " " << final_positions[1] << endl;
        } else {
            cout << "! " << final_positions[0] << " " << final_positions[0] << endl;
        }
    } else {
        // No positive results - shouldn't happen
        cout << "! 1 1" << endl;
    }
    
    cout.flush();
    return 0;
}