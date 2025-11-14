#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    vector<unsigned long long> xs;
    xs.reserve(T);
    unsigned long long x;
    while (xs.size() < (size_t)T && (cin >> x)) xs.push_back(x);
    if (xs.size() < (size_t)T) {
        // If fewer numbers were read than T, try to read remaining (in case of unusual formatting)
        while (xs.size() < (size_t)T && cin >> x) xs.push_back(x);
    }
    if (xs.empty()) return 0;

    const int LIM = 100000; // enough since sqrt(1e9) ~ 31623
    vector<int> primes;
    vector<bool> is_comp(LIM + 1, false);
    for (int i = 2; i <= LIM; ++i) {
        if (!is_comp[i]) {
            primes.push_back(i);
            if ((long long)i * i <= LIM)
                for (int j = i * i; j <= LIM; j += i) is_comp[j] = true;
        }
    }

    auto tau = [&](unsigned long long n) -> unsigned long long {
        if (n == 0) return 0;
        if (n == 1) return 1;
        unsigned long long ans = 1;
        for (int p : primes) {
            if (1ULL * p * p > n) break;
            if (n % p == 0) {
                int e = 0;
                while (n % p == 0) {
                    n /= p;
                    ++e;
                }
                ans *= (e + 1);
            }
        }
        if (n > 1) ans *= 2;
        return ans;
    };

    for (size_t i = 0; i < xs.size(); ++i) {
        cout << tau(xs[i]) << "\n";
    }

    return 0;
}