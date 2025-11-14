#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    vector<long long> X(T);
    for (int i = 0; i < T; ++i) cin >> X[i];

    const int LIM = 100000;
    vector<int> primes;
    vector<bool> is_prime(LIM + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIM; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIM; j += i) is_prime[j] = false;
        }
    }
    for (int i = 2; i <= LIM; ++i) if (is_prime[i]) primes.push_back(i);

    for (int i = 0; i < T; ++i) {
        long long n = X[i];
        if (n <= 1) {
            cout << 1 << '\n';
            continue;
        }
        long long ans = 1;
        for (int p : primes) {
            if (1LL * p * p > n) break;
            if (n % p == 0) {
                int cnt = 0;
                while (n % p == 0) {
                    n /= p;
                    ++cnt;
                }
                ans *= (cnt + 1);
            }
        }
        if (n > 1) ans *= 2;
        cout << ans << '\n';
    }
    return 0;
}