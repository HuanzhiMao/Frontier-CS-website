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
        
        vector<int> candidates;
        
        // Binary search approach to narrow down candidates
        int left = 1, right = n;
        
        while (right - left + 1 > 4) {
            int mid = (left + right) / 2;
            
            cout << "? " << left << " " << mid << endl;
            cout.flush();
            int response;
            cin >> response;
            
            int expected = mid - left + 1;
            if (response == expected - 1) {
                // One student is missing in this range
                right = mid;
            } else {
                // Missing student is in the other half
                left = mid + 1;
            }
        }
        
        // Now we have at most 4 candidates in range [left, right]
        for (int i = left; i <= right; i++) {
            candidates.push_back(i);
        }
        
        // Test individual candidates
        vector<int> likely_absent;
        
        for (int candidate : candidates) {
            cout << "? " << candidate << " " << candidate << endl;
            cout.flush();
            int response;
            cin >> response;
            
            if (response == 0) {
                likely_absent.push_back(candidate);
            }
        }
        
        // If we found clear candidates, try them
        if (!likely_absent.empty()) {
            cout << "! " << likely_absent[0] << endl;
            cout.flush();
            int result;
            cin >> result;
            
            if (result == 0 && likely_absent.size() > 1) {
                cout << "! " << likely_absent[1] << endl;
                cout.flush();
                cin >> result;
            } else if (result == 0 && candidates.size() > likely_absent.size()) {
                // Try another candidate
                for (int candidate : candidates) {
                    if (find(likely_absent.begin(), likely_absent.end(), candidate) == likely_absent.end()) {
                        cout << "! " << candidate << endl;
                        cout.flush();
                        cin >> result;
                        break;
                    }
                }
            }
        } else {
            // If no clear individual response, try all candidates
            cout << "! " << candidates[0] << endl;
            cout.flush();
            int result;
            cin >> result;
            
            if (result == 0 && candidates.size() > 1) {
                cout << "! " << candidates[1] << endl;
                cout.flush();
                cin >> result;
            }
        }
        
        cout << "#" << endl;
        cout.flush();
    }
    
    return 0;
}