#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) cin >> p[i];
        int i1 = -1, i2 = -1;
        for (int i = 0; i < n; ++i) {
            if (i1 == -1 || p[i] > p[i1]) {
                i2 = i1;
                i1 = i;
            } else if (i2 == -1 || p[i] > p[i2]) {
                i2 = i;
            }
        }
        cout << i1 << " " << i2 << "\n";
    }
    return 0;
}