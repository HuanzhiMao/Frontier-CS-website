#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;

    auto query_prefix_with_ban = [&](int x, int m, int ban)->int {
        int cnt = m - ((ban != -1 && ban <= m) ? 1 : 0);
        cout << "? " << x << " " << cnt;
        for (int i = 1; i <= m; ++i) {
            if (i == ban) continue;
            cout << " " << i;
        }
        cout << endl;
        cout.flush();
        int ret;
        if (!(cin >> ret)) exit(0);
        if (ret == -1) exit(0);
        return ret;
    };

    auto query_all_except = [&](int x, int L, int ban)->int {
        int cnt = L - 1;
        cout << "? " << x << " " << cnt;
        for (int i = 1; i <= L; ++i) {
            if (i == ban) continue;
            cout << " " << i;
        }
        cout << endl;
        cout.flush();
        int ret;
        if (!(cin >> ret)) exit(0);
        if (ret == -1) exit(0);
        return ret;
    };

    for (int _tc = 0; _tc < t; ++_tc) {
        int n;
        if (!(cin >> n)) return 0;
        if (n == -1) return 0;
        int L = 2 * n - 1;

        bool found = false;
        for (int x = 1; x <= n; ++x) {
            // Find first occurrence position p1 of value x
            int l = 1, r = L;
            while (l < r) {
                int mid = (l + r) >> 1;
                int ans = query_prefix_with_ban(x, mid, -1);
                if (ans == 1) r = mid;
                else l = mid + 1;
            }
            int p1 = l;
            // Check if there is a second occurrence
            int hasSecond = query_all_except(x, L, p1);
            if (hasSecond == 0) {
                cout << "! " << x << endl;
                cout.flush();
                found = true;
                break;
            }
        }
        if (!found) {
            // Fallback (should not happen if the problem constraints hold)
            cout << "! " << 1 << endl;
            cout.flush();
        }
    }

    return 0;
}