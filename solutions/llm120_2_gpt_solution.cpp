#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = 100;
    vector<vector<long long>> S(n + 1, vector<long long>(n + 1, 0));
    vector<long long> T(n + 1, 0);
    long long A = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            for (int k = j + 1; k <= n; ++k) {
                cout << "? " << i << " " << j << " " << k << endl;
                cout.flush();
                int ans;
                if (!(cin >> ans)) return 0;
                if (ans < 0) return 0;
                A += ans;
                T[i] += ans;
                T[j] += ans;
                T[k] += ans;
                S[i][j] += ans;
                S[i][k] += ans;
                S[j][k] += ans;
            }
        }
    }

    long long M_total = A / (n - 2);
    vector<long long> deg(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        deg[i] = (T[i] - M_total) / (n - 3);
    }

    vector<vector<int>> adj(n + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            long long val = (S[i][j] - deg[i] - deg[j]) / (n - 4);
            int e = (int)val;
            if (e < 0) e = 0;
            if (e > 1) e = 1;
            adj[i][j] = adj[j][i] = e;
        }
    }

    cout << "!" << endl;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i == j) cout << '0';
            else cout << (adj[i][j] ? '1' : '0');
        }
        cout << endl;
    }
    cout.flush();
    return 0;
}