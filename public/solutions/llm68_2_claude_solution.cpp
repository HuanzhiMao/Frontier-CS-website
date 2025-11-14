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
        
        vector<int> ink_count(n, 0);
        
        // Try each pen until we find enough information
        for (int pen = 0; pen < n; pen++) {
            while (true) {
                cout << "0 " << pen << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == 1) {
                    ink_count[pen]++;
                } else {
                    break; // pen is empty
                }
            }
        }
        
        // Find two pens with total ink >= n
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (ink_count[i] + ink_count[j] >= n) {
                    cout << "1 " << i << " " << j << endl;
                    cout.flush();
                    goto next_test;
                }
            }
        }
        
        next_test:;
    }
    
    return 0;
}