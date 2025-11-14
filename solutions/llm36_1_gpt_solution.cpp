#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const long long m = 1000000;
    cout << "0 " << m;
    for (long long i = 1; i <= m; ++i) {
        cout << ' ' << i;
    }
    cout << '\n';
    cout.flush();

    long long C;
    if (!(cin >> C)) {
        // If no response, just exit with a dummy guess to avoid blocking forever
        cout << "1 2\n";
        cout.flush();
        return 0;
    }

    long long n_guess = 1000000000LL;
    if (C > 0) {
        long long q = (2 * C) / m + 1;
        __int128 num = ( (__int128)m * q - C ) * 2;
        __int128 den = (__int128)q * (q + 1);
        long long s = (long long)(num / den);
        n_guess = s;
    }

    cout << "1 " << n_guess << '\n';
    cout.flush();
    return 0;
}