#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

u64 mul_mod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 pow_mod(u64 a, u64 d, u64 mod) {
    u64 r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

bool isPrime(u64 n) {
    if (n < 2) return false;
    static u64 testPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (u64 p : testPrimes) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (u64 a : testPrimes) {
        if (a % n == 0) continue;
        u64 x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (u64 r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

u64 brent(u64 n) {
    if ((n & 1ULL) == 0) return 2;
    uniform_int_distribution<u64> dist_c(1, n - 1);
    uniform_int_distribution<u64> dist_x(0, n - 1);
    u64 y = dist_x(rng);
    u64 c = dist_c(rng);
    u64 m = 1ULL << (rng() % 6 + 6); // between 64 and 2048
    u64 g = 1, r = 1, q = 1, x, ys;

    while (g == 1) {
        x = y;
        for (u64 i = 0; i < r; ++i) y = (mul_mod(y, y, n) + c) % n;
        u64 k = 0;
        while (k < r && g == 1) {
            ys = y;
            u64 lim = min(m, r - k);
            for (u64 i = 0; i < lim; ++i) {
                y = (mul_mod(y, y, n) + c) % n;
                u64 diff = x > y ? x - y : y - x;
                if (diff == 0) diff = n;
                q = mul_mod(q, diff, n);
            }
            g = std::gcd(q, n);
            k += lim;
        }
        r <<= 1;
    }
    if (g == n) {
        do {
            ys = (mul_mod(ys, ys, n) + c) % n;
            u64 diff = x > ys ? x - ys : ys - x;
            g = std::gcd(diff, n);
        } while (g == 1);
    }
    return g;
}

void factor_rec(u64 n, map<u64, int>& mp) {
    if (n == 1) return;
    if (isPrime(n)) { mp[n]++; return; }
    u64 d = n;
    while (d == n) d = brent(n);
    factor_rec(d, mp);
    factor_rec(n / d, mp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        u64 X;
        if (!(cin >> X)) X = 1;
        if (X == 1) {
            cout << 1 << '\n';
            continue;
        }
        map<u64, int> mp;
        factor_rec(X, mp);
        unsigned long long ans = 1;
        for (auto &kv : mp) {
            ans *= (unsigned long long)(kv.second + 1);
        }
        cout << ans << '\n';
    }
    return 0;
}