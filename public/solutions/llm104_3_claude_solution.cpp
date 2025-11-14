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
        
        vector<int> suspects;
        
        // Binary search approach to narrow down suspects
        int left = 1, right = n;
        
        while (right - left + 1 > 4) {
            int mid = (left + right) / 2;
            
            cout << "? " << left << " " << mid << endl;
            cout.flush();
            int response1;
            cin >> response1;
            
            int expected = mid - left + 1;
            if (response1 < expected) {
                // Missing student is in this range
                right = mid;
            } else {
                // Missing student is not in this range
                left = mid + 1;
            }
        }
        
        // Now we have narrowed down to at most 4 candidates
        for (int i = left; i <= right; i++) {
            suspects.push_back(i);
        }
        
        // If we still have more than 2 suspects, try to narrow down further
        if (suspects.size() > 2) {
            // Query individual positions to get more information
            vector<int> responses;
            for (int suspect : suspects) {
                cout << "? " << suspect << " " << suspect << endl;
                cout.flush();
                int response;
                cin >> response;
                responses.push_back(response);
            }
            
            // Find positions where response is 0 (likely absent)
            vector<int> likely_absent;
            for (int i = 0; i < suspects.size(); i++) {
                if (responses[i] == 0) {
                    likely_absent.push_back(suspects[i]);
                }
            }
            
            if (likely_absent.size() <= 2 && !likely_absent.empty()) {
                suspects = likely_absent;
            } else if (suspects.size() > 2) {
                // Take first two if we can't narrow down further
                suspects = {suspects[0], suspects[1]};
            }
        }
        
        // Make our guesses
        if (suspects.size() >= 1) {
            cout << "! " << suspects[0] << endl;
            cout.flush();
            int result;
            cin >> result;
            
            if (result == 0 && suspects.size() >= 2) {
                cout << "! " << suspects[1] << endl;
                cout.flush();
                cin >> result;
            }
        }
        
        cout << "#" << endl;
        cout.flush();
    }
    
    return 0;
}