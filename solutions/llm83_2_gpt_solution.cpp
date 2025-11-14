#include <bits/stdc++.h>
using namespace std;

struct SplitMix64 {
    uint64_t x;
    SplitMix64(uint64_t s = 0) : x(s) {}
    uint64_t next() {
        uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    vector<int> spf(n + 1, 0);
    vector<int> primes;
    primes.reserve(n / 10);
    vector<signed char> f(n + 1, 0);

    f[1] = 1;
    SplitMix64 rng(0x9e3779b97f4a7c15ULL ^ (uint64_t)n ^ 0x123456789abcdef0ULL);

    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            f[i] = (rng.next() & 1) ? 1 : -1; // assign random sign to prime
        }
        for (int p : primes) {
            long long v = 1LL * i * p;
            if (v > n) break;
            spf[v] = p;
            f[v] = (signed char)(f[i] * f[p]); // completely multiplicative
            if (p == spf[i]) break;
        }
    }

    string out;
    out.reserve((size_t)n * 3 + 10);
    for (int i = 1; i <= n; ++i) {
        if (f[i] == 1) {
            out.push_back('1');
        } else {
            out.push_back('-');
            out.push_back('1');
        }
        if (i < n) out.push_back(' ');
    }
    out.push_back('\n');
    cout << out;
    return 0;
}