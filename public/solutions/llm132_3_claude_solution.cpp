#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int R, H;
    cin >> R >> H;
    
    vector<int> positions;
    for (int i = 1; i <= 1000; i++) {
        positions.push_back(i);
    }
    
    // Send robots to narrow down the search space
    vector<vector<int>> sent_queries;
    
    // Binary search approach - divide positions into groups
    int left = 1, right = 1000;
    
    while (right - left > 1) {
        int mid = (left + right) / 2;
        
        // Send robot to check left half
        cout << "? " << (mid - left + 1);
        for (int i = left; i <= mid; i++) {
            cout << " " << i;
        }
        cout << endl;
        cout.flush();
        
        vector<int> query1;
        for (int i = left; i <= mid; i++) {
            query1.push_back(i);
        }
        sent_queries.push_back(query1);
        
        // Send robot to check right half
        cout << "? " << (right - mid);
        for (int i = mid + 1; i <= right; i++) {
            cout << " " << i;
        }
        cout << endl;
        cout.flush();
        
        vector<int> query2;
        for (int i = mid + 1; i <= right; i++) {
            query2.push_back(i);
        }
        sent_queries.push_back(query2);
        
        // Get results
        cout << "@" << endl;
        cout.flush();
        
        int L;
        cin >> L;
        vector<int> results(L);
        for (int i = 0; i < L; i++) {
            cin >> results[i];
        }
        
        // If both halves have chairmen, we need to find exact positions
        if (results[L-2] && results[L-1]) {
            // Both halves contain at least one chairman
            // Need to find exact positions in both halves
            vector<int> candidates;
            for (int i = left; i <= right; i++) {
                candidates.push_back(i);
            }
            
            // Send individual queries for each position
            for (int pos : candidates) {
                cout << "? 1 " << pos << endl;
                cout.flush();
            }
            
            cout << "@" << endl;
            cout.flush();
            
            cin >> L;
            vector<int> pos_results(L);
            for (int i = 0; i < L; i++) {
                cin >> pos_results[i];
            }
            
            vector<int> chairman_pos;
            for (int i = 0; i < pos_results.size(); i++) {
                if (pos_results[i]) {
                    chairman_pos.push_back(candidates[i]);
                }
            }
            
            if (chairman_pos.size() >= 2) {
                cout << "! " << chairman_pos[0] << " " << chairman_pos[1] << endl;
            } else {
                cout << "! " << chairman_pos[0] << " " << chairman_pos[0] << endl;
            }
            return 0;
        } else if (results[L-2]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    cout << "! " << left << " " << left << endl;
    cout.flush();
    
    return 0;
}