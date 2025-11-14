#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    int A = n + 1; // Ensure A > max b_i
    string prefix(A, 'O');
    for (int i = 1; i <= n; ++i) {
        string s = prefix + string(i, 'X'); // O^A X^i
        cout << s << '\n';
    }
    cout.flush();

    int q;
    if (!(cin >> q)) return 0;
    while (q--) {
        long long p;
        cin >> p;

        long long Al = A;
        long long Q = (p - Al) / (Al + 1); // Q = max(bu,bv) + A * bu * (1 + bv)
        long long T = Q / Al;              // T = bu * (1 + bv)
        long long r = Q % Al;              // r = max(bu, bv)

        int u = 1, v = 1;
        bool found = false;
        for (int bu = 1; bu <= n; ++bu) {
            if (T % bu != 0) continue;
            long long bv = T / bu - 1;
            if (bv >= 1 && bv <= n && max<long long>(bu, bv) == r) {
                u = bu;
                v = (int)bv;
                found = true;
                break;
            }
        }
        if (!found) {
            // Fallback (should not happen if inputs are consistent with our words)
            u = 1; v = 1;
        }
        cout << u << ' ' << v << '\n';
        cout.flush();
    }

    return 0;
}