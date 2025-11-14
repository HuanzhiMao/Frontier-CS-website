#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> orValues(n + 1, vector<int>(n + 1, -1));
    
    // Query all pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << "\n";
            cout.flush();
            int result;
            cin >> result;
            if (result == -1) return 0;
            orValues[i][j] = orValues[j][i] = result;
        }
    }
    
    // Find the element that is 0
    int zeroPos = -1;
    for (int i = 1; i <= n; i++) {
        bool isZero = true;
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            // Check if orValues[i][j] could be p[j] (meaning p[i] = 0)
            bool found = false;
            for (int k = 1; k <= n; k++) {
                if (k == i || k == j) continue;
                if ((orValues[i][j] | orValues[j][k]) == orValues[i][k]) {
                    found = true;
                    break;
                }
            }
            if (!found && n > 3) {
                isZero = false;
                break;
            }
        }
        if (isZero) {
            zeroPos = i;
            break;
        }
    }
    
    if (zeroPos == -1) {
        // Try a different approach - find zero by checking which position
        // when OR'd with others gives the smallest values
        int minSum = INT_MAX;
        for (int i = 1; i <= n; i++) {
            int sum = 0;
            for (int j = 1; j <= n; j++) {
                if (i != j) sum += orValues[i][j];
            }
            if (sum < minSum) {
                minSum = sum;
                zeroPos = i;
            }
        }
    }
    
    p[zeroPos] = 0;
    
    // Now we know position zeroPos has value 0
    // So orValues[zeroPos][j] = p[j] for all j != zeroPos
    vector<bool> used(n, false);
    used[0] = true;
    
    for (int j = 1; j <= n; j++) {
        if (j != zeroPos) {
            p[j] = orValues[zeroPos][j];
            used[p[j]] = true;
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << "\n";
    cout.flush();
    
    return 0;
}