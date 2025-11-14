#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    long long m = 1LL * n * (n - 1) / 2;
    
    vector<int> vals;
    vals.reserve(m);
    int x;
    while ((long long)vals.size() < m && (cin >> x)) {
        vals.push_back(x & 1);
    }
    
    vector<vector<unsigned char>> S(n + 2, vector<unsigned char>(n + 2, 0));
    int idx = 0;
    for (int l = 1; l <= n; ++l) {
        for (int r = l + 1; r <= n; ++r) {
            if (idx < (int)vals.size()) S[l][r] = (unsigned char)vals[idx++];
            else S[l][r] = 0;
        }
    }
    
    vector<vector<unsigned char>> a(n + 2, vector<unsigned char>(n + 2, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            unsigned char val = S[i][j] ^ S[i + 1][j] ^ S[i][j - 1] ^ S[i + 1][j - 1];
            a[i][j] = val & 1;
        }
    }
    
    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int cnt = 0;
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            if (i < j) cnt += a[i][j];
            else cnt += 1 - a[j][i];
        }
        p[i] = cnt + 1;
    }
    
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << p[i];
    }
    cout << '\n';
    
    return 0;
}