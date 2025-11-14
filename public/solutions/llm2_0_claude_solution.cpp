#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> perm(n + 1);
    vector<bool> used(n + 1, false);
    
    // For each position, try each number from 1 to n
    for (int pos = 1; pos <= n; pos++) {
        for (int num = 1; num <= n; num++) {
            if (used[num]) continue;
            
            // Create query with current number at position pos
            vector<int> query(n + 1);
            for (int i = 1; i <= n; i++) {
                if (i == pos) {
                    query[i] = num;
                } else if (perm[i] != 0) {
                    query[i] = perm[i];
                } else {
                    // Use a number that hasn't been used yet, preferably one we haven't tried
                    for (int j = 1; j <= n; j++) {
                        if (!used[j] && j != num) {
                            query[i] = j;
                            break;
                        }
                    }
                    if (query[i] == 0) query[i] = 1; // fallback
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
            
            // Create another query with different number at position pos to compare
            bool found = false;
            for (int num2 = 1; num2 <= n; num2++) {
                if (used[num2] || num2 == num) continue;
                
                vector<int> query2(n + 1);
                for (int i = 1; i <= n; i++) {
                    if (i == pos) {
                        query2[i] = num2;
                    } else {
                        query2[i] = query[i];
                    }
                }
                
                cout << "0";
                for (int i = 1; i <= n; i++) {
                    cout << " " << query2[i];
                }
                cout << endl;
                cout.flush();
                
                int response2;
                cin >> response2;
                
                if (response > response2) {
                    perm[pos] = num;
                    used[num] = true;
                    found = true;
                    break;
                } else if (response2 > response) {
                    perm[pos] = num2;
                    used[num2] = true;
                    found = true;
                    break;
                }
            }
            
            if (found) break;
        }
    }
    
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << perm[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}