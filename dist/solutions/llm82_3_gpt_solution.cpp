#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if(!(cin >> n)) return 0;
    vector<vector<int>> a(n + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            int v;
            cin >> v;
            a[i][j] = v;
            a[j][i] = v;
        }
    }
    
    vector<int> mn(n + 1, INT_MAX);
    for (int i = 1; i <= n; ++i) {
        int mval = INT_MAX;
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            mval = min(mval, a[i][j]);
        }
        mn[i] = mval;
    }
    
    int zero_idx = -1;
    for (int i = 1; i <= n; ++i) {
        bool ok = true;
        for (int k = 1; k <= n; ++k) {
            if (k == i) continue;
            if (a[i][k] != mn[k]) { ok = false; break; }
        }
        if (ok) { zero_idx = i; break; }
    }
    
    // Fallback (shouldn't be needed)
    if (zero_idx == -1) {
        vector<int> ones;
        for (int i = 1; i <= n; ++i) if (mn[i] == 1) ones.push_back(i);
        for (int cand : ones) {
            bool ok = true;
            for (int k = 1; k <= n; ++k) {
                if (k == cand) continue;
                if (a[cand][k] != mn[k]) { ok = false; break; }
            }
            if (ok) { zero_idx = cand; break; }
        }
        if (zero_idx == -1 && !ones.empty()) zero_idx = ones[0];
        if (zero_idx == -1) zero_idx = 1; // ultimate fallback
    }
    
    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        if (i == zero_idx) p[i] = 0;
        else p[i] = a[i][zero_idx];
    }
    
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << p[i];
    }
    cout << '\n';
    
    return 0;
}