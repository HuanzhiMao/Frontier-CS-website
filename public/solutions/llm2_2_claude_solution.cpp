#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> result(n + 1);
    vector<bool> used(n + 1, false);
    
    // For each position, find what number goes there
    for (int pos = 1; pos <= n; pos++) {
        // Try each unused number at this position
        for (int num = 1; num <= n; num++) {
            if (used[num]) continue;
            
            // Create query with num at position pos, and 1s elsewhere (but avoid used numbers)
            vector<int> query(n + 1);
            query[pos] = num;
            
            int filler = 1;
            while (used[filler] || filler == num) filler++;
            
            for (int i = 1; i <= n; i++) {
                if (i != pos) {
                    query[i] = filler;
                }
            }
            
            cout << "0";
            for (int i = 1; i <= n; i++) {
                cout << " " << query[i];
            }
            cout << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            // Calculate expected matches from previous positions
            int expected = 0;
            for (int i = 1; i < pos; i++) {
                if (query[i] == result[i]) expected++;
            }
            
            // If we got one more match than expected, this number belongs here
            if (response == expected + 1) {
                result[pos] = num;
                used[num] = true;
                break;
            }
        }
    }
    
    // Output the final answer
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}