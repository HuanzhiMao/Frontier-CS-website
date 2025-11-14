#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const long long m = 1000000;
    cout << 0 << " " << m;
    for (long long i = 1; i <= m; ++i) {
        cout << " " << i;
    }
    cout << "\n";
    cout.flush();

    long long y;
    if (!(cin >> y)) {
        return 0;
    }

    long long answer = -1;

    if (y > 0) {
        for (long long q = 1; q <= m / 2; ++q) {
            i128 mq = (i128)m * q;
            i128 num = 2 * (mq - (i128)y);
            i128 denom = (i128)q * (q + 1);
            if (num <= 0) continue;
            if (num % denom != 0) continue;
            i128 n128 = num / denom;
            if (n128 < 2 || n128 > 1000000000LL) continue;
            long long n = (long long)n128;
            long long r = m - q * n;
            if (r < 0 || r >= n) continue;
            i128 ycalc = (i128)n * q * (q - 1) / 2 + (i128)r * q;
            if ((long long)ycalc == y) {
                answer = n;
                break;
            }
        }
    }

    if (answer == -1) {
        // Fallback guess if not determined (e.g., y == 0)
        answer = 1000000000LL;
    }

    cout << 1 << " " << answer << "\n";
    cout.flush();
    return 0;
}