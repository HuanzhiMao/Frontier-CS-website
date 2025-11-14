#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int R, H;
    cin >> R >> H;
    
    vector<int> robots_sent;
    vector<vector<int>> robot_positions;
    
    // Send 10 robots each covering 100 positions
    for (int i = 0; i < 10; i++) {
        cout << "? 100";
        vector<int> positions;
        for (int j = 0; j < 100; j++) {
            int pos = i * 100 + j + 1;
            positions.push_back(pos);
            cout << " " << pos;
        }
        cout << endl;
        cout.flush();
        robot_positions.push_back(positions);
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
    for (int i = 0; i < results.size(); i++) {
        if (results[i] == 1) {
            positive_groups.push_back(i);
        }
    }
    
    if (positive_groups.size() == 1) {
        // Both chairmen in same group, do binary search
        int group = positive_groups[0];
        vector<int> candidates;
        for (int j = 0; j < 100; j++) {
            candidates.push_back(group * 100 + j + 1);
        }
        
        // Binary search to find positions
        while (candidates.size() > 2) {
            int mid = candidates.size() / 2;
            cout << "? " << mid;
            for (int i = 0; i < mid; i++) {
                cout << " " << candidates[i];
            }
            cout << endl;
            cout.flush();
            
            cout << "@" << endl;
            cout.flush();
            
            cin >> L;
            int result;
            cin >> result;
            
            if (result == 1) {
                candidates = vector<int>(candidates.begin(), candidates.begin() + mid);
            } else {
                candidates = vector<int>(candidates.begin() + mid, candidates.end());
            }
        }
        
        cout << "! " << candidates[0] << " " << candidates[0] << endl;
        cout.flush();
        
    } else if (positive_groups.size() == 2) {
        // One chairman in each group, narrow down each group
        vector<vector<int>> group_candidates(2);
        
        for (int g = 0; g < 2; g++) {
            int group = positive_groups[g];
            for (int j = 0; j < 100; j++) {
                group_candidates[g].push_back(group * 100 + j + 1);
            }
        }
        
        // Binary search for first group
        while (group_candidates[0].size() > 1) {
            int mid = group_candidates[0].size() / 2;
            cout << "? " << mid;
            for (int i = 0; i < mid; i++) {
                cout << " " << group_candidates[0][i];
            }
            cout << endl;
            cout.flush();
            
            cout << "@" << endl;
            cout.flush();
            
            cin >> L;
            int result;
            cin >> result;
            
            if (result == 1) {
                group_candidates[0] = vector<int>(group_candidates[0].begin(), group_candidates[0].begin() + mid);
            } else {
                group_candidates[0] = vector<int>(group_candidates[0].begin() + mid, group_candidates[0].end());
            }
        }
        
        // Binary search for second group
        while (group_candidates[1].size() > 1) {
            int mid = group_candidates[1].size() / 2;
            cout << "? " << mid;
            for (int i = 0; i < mid; i++) {
                cout << " " << group_candidates[1][i];
            }
            cout << endl;
            cout.flush();
            
            cout << "@" << endl;
            cout.flush();
            
            cin >> L;
            int result;
            cin >> result;
            
            if (result == 1) {
                group_candidates[1] = vector<int>(group_candidates[1].begin(), group_candidates[1].begin() + mid);
            } else {
                group_candidates[1] = vector<int>(group_candidates[1].begin() + mid, group_candidates[1].end());
            }
        }
        
        cout << "! " << group_candidates[0][0] << " " << group_candidates[1][0] << endl;
        cout.flush();
    }
    
    return 0;
}