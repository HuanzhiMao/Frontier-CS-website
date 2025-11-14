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
        
        vector<int> ink_used(n, 0);
        
        // We need to find two pens with total remaining ink >= n
        // Since it's a permutation of 0 to n-1, total ink is n*(n-1)/2
        // We need pens with high initial ink values
        
        // Strategy: Test pens to find ones with high ink
        vector<pair<int, int>> pen_info; // (remaining_ink_estimate, pen_id)
        
        for (int pen = 0; pen < n; pen++) {
            int uses = 0;
            while (true) {
                cout << "0 " << pen << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == 0) {
                    break;
                }
                uses++;
                
                // Don't use too many times on one pen
                if (uses >= n - 1) break;
            }
            pen_info.push_back({uses, pen});
        }
        
        // Sort by remaining ink estimate (descending)
        sort(pen_info.rbegin(), pen_info.rend());
        
        // Take the two pens with highest remaining ink
        int pen1 = pen_info[0].second;
        int pen2 = pen_info[1].second;
        
        cout << "1 " << pen1 << " " << pen2 << endl;
        cout.flush();
    }
    
    return 0;
}