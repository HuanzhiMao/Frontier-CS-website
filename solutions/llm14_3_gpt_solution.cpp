#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

static const u64 MAX_X = 1000000000ULL;
static const u64 QUERY_LIMIT = 200000;

mt19937_64 rng((u64)chrono::high_resolution_clock::now().time_since_epoch().count());

u64 used_queries = 0;

inline void flush_out() {
    cout.flush();
}

bool read_label(long long &lab) {
    if (!(cin >> lab)) return false;
    return true;
}

bool check_limit() {
    return used_queries < QUERY_LIMIT;
}

bool issue_walk(u64 x, long long &label) {
    if (!check_limit()) return false;
    cout << "walk " << x << '\n';
    flush_out();
    used_queries++;
    return read_label(label);
}

u64 mulmod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 powmod(u64 a, u64 d, u64 mod) {
    u64 r = 1 % mod;
    while (d) {
        if (d & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        d >>= 1;
    }
    return r;
}

bool isPrime(u64 n) {
    if (n < 2) return false;
    static const u64 testPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (u64 p : testPrimes) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    auto trial = [&](u64 a) -> bool {
        if (a % n == 0) return true;
        u64 x = powmod(a, d, n);
        if (x == 1 || x == n - 1) return true;
        for (u64 i = 1; i < s; i++) {
            x = mulmod(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    for (u64 a : testPrimes) {
        if (!trial(a)) return false;
    }
    return true;
}

u64 rng_u64(u64 l, u64 r) {
    uniform_int_distribution<u64> dist(l, r);
    return dist(rng);
}

u64 pollard(u64 n) {
    if (n % 2ULL == 0) return 2ULL;
    if (n % 3ULL == 0) return 3ULL;
    u64 c = rng_u64(1, n - 1);
    u64 x = rng_u64(0, n - 1);
    u64 y = x;
    u64 d = 1;
    auto f = [&](u64 v) {
        return (mulmod(v, v, n) + c) % n;
    };
    while (d == 1) {
        x = f(x);
        y = f(f(y));
        u64 diff = x > y ? x - y : y - x;
        d = std::gcd(diff, n);
        if (d == n) {
            c = rng_u64(1, n - 1);
            x = rng_u64(0, n - 1);
            y = x;
            d = 1;
        }
    }
    return d;
}

void factor_rec(u64 n, vector<u64> &fac) {
    if (n == 1) return;
    if (isPrime(n)) {
        fac.push_back(n);
        return;
    }
    u64 d = pollard(n);
    factor_rec(d, fac);
    factor_rec(n / d, fac);
}

vector<pair<u64,int>> factorize(u64 n) {
    vector<u64> fac;
    factor_rec(n, fac);
    sort(fac.begin(), fac.end());
    vector<pair<u64,int>> res;
    for (size_t i = 0; i < fac.size(); ) {
        size_t j = i;
        while (j < fac.size() && fac[j] == fac[i]) j++;
        res.push_back({fac[i], (int)(j - i)});
        i = j;
    }
    return res;
}

bool walk_distance_and_check_equal(u64 dist) {
    // Get current label
    long long before;
    if (!issue_walk(0, before)) return false;

    // Walk 'dist' in chunks (<= 1e9)
    if (dist == 0) {
        // Already got label after walk 0, so trivially equal
        return true;
    }
    long long after = -1;
    while (dist >= MAX_X) {
        if (!issue_walk(MAX_X, after)) return false;
        dist -= MAX_X;
    }
    if (dist > 0) {
        if (!issue_walk(dist, after)) return false;
    }
    return after == before;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initial read current label
    long long label0;
    if (!issue_walk(0, label0)) {
        // If interaction failed, try to end gracefully
        cout << "guess " << 1 << '\n';
        flush_out();
        return 0;
    }

    unordered_map<long long, u64> seen;
    seen.reserve(1 << 17);
    seen.max_load_factor(0.7f);
    seen[label0] = 0;

    u64 T = 0;
    u64 g = 0;

    // Try to find first collision using random small steps
    const u64 STEP_MAX = 1000000ULL; // 1e6
    const u64 SEARCH_BUDGET = 150000; // keep room for factorization and checks
    u64 attempts = 0;

    while (g == 0 && attempts < SEARCH_BUDGET && check_limit()) {
        u64 x = rng_u64(1, STEP_MAX);
        long long y;
        if (!issue_walk(x, y)) {
            cout << "guess " << 1 << '\n';
            flush_out();
            return 0;
        }
        T += x;
        auto it = seen.find(y);
        if (it == seen.end()) {
            seen[y] = T;
        } else {
            u64 delta = T - it->second;
            if (delta > 0) g = delta;
        }
        attempts++;
    }

    // If still no collision, try a few more with larger steps to diversify
    u64 EXTRA_TRIES = 40000;
    while (g == 0 && EXTRA_TRIES-- && check_limit()) {
        u64 x = rng_u64(1, MAX_X);
        long long y;
        if (!issue_walk(x, y)) {
            cout << "guess " << 1 << '\n';
            flush_out();
            return 0;
        }
        T += x;
        auto it = seen.find(y);
        if (it == seen.end()) {
            seen[y] = T;
        } else {
            u64 delta = T - it->second;
            if (delta > 0) g = delta;
        }
    }

    if (g == 0) {
        // As a fallback, guess the maximum label seen (poor but ensures termination)
        long long mx = 1;
        for (auto &p : seen) mx = max(mx, p.first);
        cout << "guess " << mx << '\n';
        flush_out();
        return 0;
    }

    // Now g is a multiple of n. Reduce g to n by testing divisors via prime factorization.
    auto pf = factorize(g);
    // Greedy division by prime powers
    for (auto &pe : pf) {
        u64 p = pe.first;
        int e = pe.second;
        for (int i = 0; i < e; i++) {
            if (g % p != 0) break;
            u64 candidate = g / p;
            if (!check_limit()) break;
            bool ok = walk_distance_and_check_equal(candidate);
            if (ok) g = candidate;
            else break;
        }
    }

    // Final answer
    cout << "guess " << g << '\n';
    flush_out();
    return 0;
}