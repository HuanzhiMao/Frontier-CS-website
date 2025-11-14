#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        if (n == -1) break;
        
        int answer = -1;
        
        for (int x = 1; x <= n; x++) {
            // Query all positions for number x
            cout << "? " << x << " " << (2*n-1);
            for (int i = 1; i <= 2*n-1; i++) {
                cout << " " << i;
            }
            cout << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            if (response == -1) return 0;
            
            if (response == 0) {
                // x doesn't appear at all, which means x is the answer
                answer = x;
                break;
            }
            
            // If response is 1, x appears at least once
            // We need to check if it appears exactly once or twice
            // We'll use binary search approach on positions
            
            vector<int> positions;
            for (int i = 1; i <= 2*n-1; i++) {
                positions.push_back(i);
            }
            
            // Find all positions where x appears
            vector<int> found_positions;
            
            while (positions.size() > 0) {
                if (positions.size() == 1) {
                    found_positions.push_back(positions[0]);
                    break;
                }
                
                int mid = positions.size() / 2;
                vector<int> left_half(positions.begin(), positions.begin() + mid);
                
                cout << "? " << x << " " << left_half.size();
                for (int pos : left_half) {
                    cout << " " << pos;
                }
                cout << endl;
                cout.flush();
                
                int left_response;
                cin >> left_response;
                
                if (left_response == -1) return 0;
                
                if (left_response == 1) {
                    // x is in left half, continue searching left
                    positions = left_half;
                } else {
                    // x is in right half
                    vector<int> right_half(positions.begin() + mid, positions.end());
                    positions = right_half;
                }
            }
            
            // Now find the second occurrence if it exists
            vector<int> remaining_positions;
            for (int i = 1; i <= 2*n-1; i++) {
                bool skip = false;
                for (int found : found_positions) {
                    if (i == found) {
                        skip = true;
                        break;
                    }
                }
                if (!skip) {
                    remaining_positions.push_back(i);
                }
            }
            
            if (remaining_positions.empty()) {
                answer = x;
                break;
            }
            
            cout << "? " << x << " " << remaining_positions.size();
            for (int pos : remaining_positions) {
                cout << " " << pos;
            }
            cout << endl;
            cout.flush();
            
            int remaining_response;
            cin >> remaining_response;
            
            if (remaining_response == -1) return 0;
            
            if (remaining_response == 0) {
                // x appears exactly once
                answer = x;
                break;
            }
        }
        
        cout << "! " << answer << endl;
        cout.flush();
    }
    
    return 0;
}