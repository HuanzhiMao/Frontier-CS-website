#include <bits/stdc++.h>
using namespace std;

vector<int> primes;

void sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * 1LL * i <= n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) is_prime[j] = false;
        }
    }
    for (int i = 2; i <= n; ++i) if (is_prime[i]) primes.push_back(i);
}

long long divisors_count(long long n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long ans = 1;
    for (long long p : primes) {
        if (p * p > n) break;
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
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    vector<long long> X(T);
    for (int i = 0; i < T; ++i) {
        if (!(cin >> X[i])) X[i] = 1;
    }

    // sqrt(1e9) < 1e5, so sieve up to 1e5 is enough
    sieve(100000);

    for (int i = 0; i < T; ++i) {
        cout << divisors_count(X[i]) << "\n";
    }
    return 0;
}