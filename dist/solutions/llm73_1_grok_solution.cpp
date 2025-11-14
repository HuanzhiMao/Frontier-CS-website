#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> parity(n + 1, vector<int>(n + 1, -1));
    vector<vector<int>> inv(n + 1, vector<int>(n + 1, 0));
    for (int l = 1; l < n; l++) {
        cout << "0 " << l << " " << (l + 1) << endl;
        cout.flush();
        int res;
        cin >> res;
        parity[l][l + 1] = res;
        inv[l][l + 1] = res;
    }
    for (int i = n - 1; i >= 1; i--) {
        vector<int> cum(n + 2, 0);
        cum[i] = 0;
        cum[i + 1] = cum[i] ^ inv[i][i + 1];
        for (int r = i + 2; r <= n; r++) {
            cout << "0 " << i << " " << r << endl;
            cout.flush();
            int res;
            cin >> res;
            parity[i][r] = res;
            int temp = parity[i][r] ^ parity[i + 1][r];
            inv[i][r] = temp ^ cum[r - 1];
            cum[r] = cum[r - 1] ^ inv[i][r];
        }
    }
    vector<int> p(n + 1);
    for (int k = 1; k <= n; k++) {
        int count = 0;
        for (int j = 1; j <= n; j++) {
            if (j == k) continue;
            if (j < k) {
                if (inv[j][k] == 0) count++;
            } else {
                if (inv[k][j] == 1) count++;
            }
        }
        p[k] = count + 1;
    }
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    return 0;
}