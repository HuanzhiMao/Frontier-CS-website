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
            
            // Create query with this number at position pos
            vector<int> query(n + 1);
            for (int i = 1; i <= n; i++) {
                if (i < pos) {
                    query[i] = result[i];
                } else if (i == pos) {
                    query[i] = num;
                } else {
                    // Use a number that hasn't been used and isn't num
                    for (int j = 1; j <= n; j++) {
                        if (!used[j] && j != num) {
                            query[i] = j;
                            break;
                        }
                    }
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
            int expected_prev = pos - 1;
            
            if (response == expected_prev + 1) {
                // This number is correct for this position
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