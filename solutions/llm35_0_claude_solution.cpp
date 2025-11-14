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
        
        vector<bool> found(n + 1, false);
        int answer = -1;
        
        for (int x = 1; x <= n && answer == -1; x++) {
            if (found[x]) continue;
            
            // Query first half
            vector<int> first_half;
            for (int i = 1; i <= (2*n-1)/2; i++) {
                first_half.push_back(i);
            }
            
            cout << "? " << x << " " << first_half.size();
            for (int pos : first_half) {
                cout << " " << pos;
            }
            cout << endl;
            cout.flush();
            
            int response1;
            cin >> response1;
            if (response1 == -1) return 0;
            
            // Query second half
            vector<int> second_half;
            for (int i = (2*n-1)/2 + 1; i <= 2*n-1; i++) {
                second_half.push_back(i);
            }
            
            cout << "? " << x << " " << second_half.size();
            for (int pos : second_half) {
                cout << " " << pos;
            }
            cout << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            if (response2 == -1) return 0;
            
            int total_count = response1 + response2;
            
            if (total_count == 1) {
                answer = x;
                break;
            } else if (total_count == 2) {
                found[x] = true;
            }
        }
        
        cout << "! " << answer << endl;
        cout.flush();
    }
    
    return 0;
}