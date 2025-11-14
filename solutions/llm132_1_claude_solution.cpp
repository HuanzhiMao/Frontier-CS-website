#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int R, H;
    cin >> R >> H;
    
    vector<int> queries;
    
    // Send 10 robots with 100 positions each to cover all 1000 positions
    for (int i = 0; i < 10; i++) {
        cout << "? 100";
        for (int j = 1; j <= 100; j++) {
            cout << " " << (i * 100 + j);
        }
        cout << endl;
        cout.flush();
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
    
    vector<int> candidate_ranges;
    for (int i = 0; i < 10; i++) {
        if (results[i] == 1) {
            candidate_ranges.push_back(i);
        }
    }
    
    if (candidate_ranges.size() == 1) {
        // Both chairmen in same range, need to find exact positions
        int range = candidate_ranges[0];
        int start = range * 100 + 1;
        
        // Binary search within the range
        vector<int> positions;
        for (int i = start; i < start + 100; i++) {
            positions.push_back(i);
        }
        
        // Send individual queries for each position in the range
        for (int pos : positions) {
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
        for (int i = 0; i < L; i++) {
            if (pos_results[i] == 1) {
                chairman_pos.push_back(positions[i]);
            }
        }
        
        if (chairman_pos.size() >= 2) {
            cout << "! " << chairman_pos[0] << " " << chairman_pos[1] << endl;
        } else if (chairman_pos.size() == 1) {
            cout << "! " << chairman_pos[0] << " " << chairman_pos[0] << endl;
        }
    } else {
        // Chairmen in different ranges, need to find exact positions in each range
        vector<int> all_candidates;
        
        for (int range : candidate_ranges) {
            int start = range * 100 + 1;
            for (int i = start; i < start + 100; i++) {
                cout << "? 1 " << i << endl;
                cout.flush();
            }
        }
        
        cout << "@" << endl;
        cout.flush();
        
        cin >> L;
        vector<int> pos_results(L);
        for (int i = 0; i < L; i++) {
            cin >> pos_results[i];
        }
        
        vector<int> chairman_pos;
        int idx = 0;
        for (int range : candidate_ranges) {
            int start = range * 100 + 1;
            for (int i = start; i < start + 100; i++) {
                if (pos_results[idx] == 1) {
                    chairman_pos.push_back(i);
                }
                idx++;
            }
        }
        
        if (chairman_pos.size() >= 2) {
            cout << "! " << chairman_pos[0] << " " << chairman_pos[1] << endl;
        } else if (chairman_pos.size() == 1) {
            cout << "! " << chairman_pos[0] << " " << chairman_pos[0] << endl;
        }
    }
    
    return 0;
}