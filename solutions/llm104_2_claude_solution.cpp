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
        int left = 1, right = n;
        
        while (right - left > 2) {
            int mid = (left + right) / 2;
            
            cout << "? " << left << " " << mid << endl;
            cout.flush();
            int response1;
            cin >> response1;
            
            int expected = mid - left + 1;
            bool absent_in_first_half = (response1 == expected - 1);
            
            if (absent_in_first_half) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        if (right == left) {
            suspects.push_back(left);
        } else if (right == left + 1) {
            suspects.push_back(left);
            suspects.push_back(right);
        } else {
            suspects.push_back(left);
            suspects.push_back(left + 1);
            suspects.push_back(right);
        }
        
        for (int i = 0; i < min(2, (int)suspects.size()); i++) {
            cout << "! " << suspects[i] << endl;
            cout.flush();
            int response;
            cin >> response;
            if (response == 1) {
                break;
            }
        }
        
        cout << "#" << endl;
        cout.flush();
    }
    
    return 0;
}