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
        
        // Binary search approach to narrow down candidates
        int left = 1, right = n;
        
        while (right - left > 3) {
            int mid = (left + right) / 2;
            
            cout << "? " << left << " " << mid << endl;
            cout.flush();
            int response1;
            cin >> response1;
            
            int expected = mid - left + 1;
            if (response1 < expected) {
                // Missing student likely in this range
                right = mid;
            } else {
                // Missing student likely in other half
                left = mid + 1;
            }
        }
        
        // Now we have narrowed down to a small range
        for (int i = left; i <= right; i++) {
            suspects.push_back(i);
        }
        
        // If we don't have enough suspects, add some nearby candidates
        if (suspects.size() < 2) {
            for (int i = 1; i <= n && suspects.size() < 3; i++) {
                bool found = false;
                for (int s : suspects) {
                    if (s == i) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    suspects.push_back(i);
                }
            }
        }
        
        // Test individual suspects
        vector<int> likely_absent;
        
        for (int suspect : suspects) {
            if (likely_absent.size() >= 2) break;
            
            cout << "? " << suspect << " " << suspect << endl;
            cout.flush();
            int response;
            cin >> response;
            
            if (response == 0) {
                likely_absent.push_back(suspect);
            }
        }
        
        // If we still don't have clear candidates, use broader queries
        if (likely_absent.empty()) {
            // Try first half
            if (n > 1) {
                cout << "? 1 " << n/2 << endl;
                cout.flush();
                int response;
                cin >> response;
                
                int expected = n/2;
                if (response < expected) {
                    for (int i = 1; i <= n/2 && likely_absent.size() < 2; i++) {
                        likely_absent.push_back(i);
                    }
                } else {
                    for (int i = n/2 + 1; i <= n && likely_absent.size() < 2; i++) {
                        likely_absent.push_back(i);
                    }
                }
            }
        }
        
        // Ensure we have at least 2 candidates
        if (likely_absent.size() < 2) {
            for (int i = 1; i <= n && likely_absent.size() < 2; i++) {
                bool found = false;
                for (int x : likely_absent) {
                    if (x == i) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    likely_absent.push_back(i);
                }
            }
        }
        
        // Make our guesses
        cout << "! " << likely_absent[0] << endl;
        cout.flush();
        int result1;
        cin >> result1;
        
        if (result1 == 0) {
            cout << "! " << likely_absent[1] << endl;
            cout.flush();
            int result2;
            cin >> result2;
        }
        
        cout << "#" << endl;
        cout.flush();
    }
    
    return 0;
}