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
        
        while (right - left > 3) {
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
        
        // Now we have narrowed down to a small range
        for (int i = left; i <= right; i++) {
            suspects.push_back(i);
        }
        
        // If we still have more than 2 suspects, query individual students
        while (suspects.size() > 2) {
            int candidate = suspects.back();
            suspects.pop_back();
            
            cout << "? " << candidate << " " << candidate << endl;
            cout.flush();
            int response;
            cin >> response;
            
            if (response == 0) {
                // This student is definitely absent
                suspects.clear();
                suspects.push_back(candidate);
                break;
            }
        }
        
        // Try our best guesses
        if (suspects.size() >= 1) {
            cout << "! " << suspects[0] << endl;
            cout.flush();
            int result;
            cin >> result;
            
            if (result == 1) {
                cout << "#" << endl;
                cout.flush();
                continue;
            }
        }
        
        if (suspects.size() >= 2) {
            cout << "! " << suspects[1] << endl;
            cout.flush();
            int result;
            cin >> result;
        }
        
        cout << "#" << endl;
        cout.flush();
    }
    
    return 0;
}