#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        int n;
        if (!(cin >> n)) break;
        vector<int> a(n);
        bool ok = true;
        for (int i = 0; i < n; ++i) {
            if (!(cin >> a[i])) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            // Not enough numbers provided; default to identity permutation
            for (int i = 0; i < n; ++i) a[i] = i + 1;
        }
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << a[i];
        }
        cout << '\n';
        if (!ok) break;
    }
    return 0;
}