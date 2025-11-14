#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    bool firstOutput = true;
    while (cin >> n >> m) {
        string s;
        for (int i = 0; i < m; ++i) {
            cin >> s; // read and ignore participants' predictions
        }
        string t;
        for (int i = 0; i < m; ++i) {
            cin >> t; // read actual outcome
            if (!firstOutput) cout << '\n';
            firstOutput = false;
            cout << (t.empty() ? '0' : t[0]);
        }
    }
    return 0;
}