#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int R, H;
    cin >> R >> H;
    
    vector<int> robots_sent;
    vector<vector<int>> robot_positions;
    
    // Send robots to cover ranges of positions
    int pos = 1;
    while (pos <= 1000) {
        int end_pos = min(1000, pos + 39);
        cout << "? " << (end_pos - pos + 1);
        vector<int> current_positions;
        for (int i = pos; i <= end_pos; i++) {
            cout << " " << i;
            current_positions.push_back(i);
        }
        cout << endl;
        cout.flush();
        
        robot_positions.push_back(current_positions);
        pos = end_pos + 1;
    }
    
    // Wait for results
    cout << "@" << endl;
    cout.flush();
    
    int L;
    cin >> L;
    vector<int> results(L);
    for (int i = 0; i < L; i++) {
        cin >> results[i];
    }
    
    // Find which ranges contain chairmen
    vector<int> positive_ranges;
    for (int i = 0; i < L; i++) {
        if (results[i] == 1) {
            positive_ranges.push_back(i);
        }
    }
    
    if (positive_ranges.size() == 1) {
        // Both chairmen in same range, need to narrow down
        vector<int>& range = robot_positions[positive_ranges[0]];
        int mid = range.size() / 2;
        
        cout << "? " << mid;
        for (int i = 0; i < mid; i++) {
            cout << " " << range[i];
        }
        cout << endl;
        cout.flush();
        
        cout << "@" << endl;
        cout.flush();
        
        cin >> L;
        int result;
        cin >> result;
        
        if (result == 1) {
            // Both in first half
            cout << "! " << range[0] << " " << range[0] << endl;
        } else {
            // Both in second half
            cout << "! " << range[mid] << " " << range[mid] << endl;
        }
    } else if (positive_ranges.size() >= 2) {
        // Chairmen in different ranges
        int pos1 = robot_positions[positive_ranges[0]][0];
        int pos2 = robot_positions[positive_ranges[1]][0];
        cout << "! " << pos1 << " " << pos2 << endl;
    } else {
        // No positive results, shouldn't happen
        cout << "! 1 1" << endl;
    }
    
    return 0;
}