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
        
        vector<int> remaining(n, -1);
        
        // Test each pen to find how much ink it has
        for (int i = 0; i < n; i++) {
            int count = 0;
            while (true) {
                cout << "0 " << i << endl;
                cout.flush();
                int response;
                cin >> response;
                
                if (response == 0) {
                    remaining[i] = count;
                    break;
                }
                count++;
            }
        }
        
        // Find two pens with total remaining ink >= n
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (remaining[i] + remaining[j] >= n) {
                    cout << "1 " << i << " " << j << endl;
                    cout.flush();
                    goto next_case;
                }
            }
        }
        
        next_case:;
    }
    
    return 0;
}