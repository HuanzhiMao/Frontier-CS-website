#include <bits/stdc++.h>
using namespace std;

struct XorShift {
    uint32_t x;
    XorShift(uint32_t seed = 123456789u) : x(seed) {}
    inline uint32_t next() {
        uint32_t y = x;
        y ^= y << 13;
        y ^= y >> 17;
        y ^= y << 5;
        x = y;
        return x;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    // Linear sieve to get spf and primes
    vector<int> spf(n + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            long long v = 1LL * p * i;
            if (v > n || p > spf[i]) break;
            spf[v] = p;
        }
    }

    // Initialize s[p] for primes (random ±1), f values, and prefix sums S
    vector<int> s(n + 1, 1);
    XorShift rng(123456789u);
    for (int p : primes) {
        s[p] = (rng.next() & 1) ? 1 : -1;
    }

    vector<int> f(n + 1, 1);
    f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        int p = spf[i];
        f[i] = f[i / p] * s[p];
    }

    vector<int> S(n + 1, 0);
    int M = 0;
    for (int i = 1; i <= n; ++i) {
        S[i] = S[i - 1] + f[i];
        int a = S[i] >= 0 ? S[i] : -S[i];
        if (a > M) M = a;
    }

    // Choose small primes to optimize
    vector<int> chosen;
    const int Pmax = 200; // consider primes up to 200
    for (int p : primes) {
        if (p > Pmax) break;
        chosen.push_back(p);
    }
    // Limit number of chosen primes to keep time bounded
    const int Kmax = 64;
    if ((int)chosen.size() > Kmax) chosen.resize(Kmax);

    vector<char> par(n + 1, 0);

    // One pass of coordinate descent on chosen small primes
    for (int p : chosen) {
        // Build parity array for exponent of p in factorization of i
        par[1] = 0;
        for (int i = 2; i <= n; ++i) {
            if (i % p == 0) par[i] = par[i / p] ^ 1;
            else par[i] = 0;
        }

        long long tot = 0;
        int Mprime = 0;
        for (int k = 1; k <= n; ++k) {
            if (par[k]) tot += f[k];
            int val = (int)((long long)S[k] - 2LL * tot);
            int ab = val >= 0 ? val : -val;
            if (ab > Mprime) Mprime = ab;
        }

        if (Mprime < M) {
            // Accept flip
            s[p] = -s[p];
            M = Mprime;
            tot = 0;
            for (int k = 1; k <= n; ++k) {
                if (par[k]) {
                    tot += f[k];
                    f[k] = -f[k];
                }
                S[k] = (int)((long long)S[k] - 2LL * tot);
            }
        }
    }

    // Output the sequence f(1..n)
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << f[i];
    }
    cout << '\n';
    return 0;
}