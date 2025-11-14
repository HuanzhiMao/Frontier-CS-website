#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    vector<int> f(n + 1, 0);
    vector<int> primes;
    f[1] = 1;

    for (int i = 2; i <= n; ++i) {
        if (f[i] == 0) { // i is prime
            f[i] = -1; 
            primes.push_back(i);
        }
        for (int p : primes) {
            long long t = 1LL * i * p;
            if (t > n) break;
            f[(int)t] = -f[i]; // Liouville: multiplying by any prime toggles sign
            if (i % p == 0) break;
        }
    }

    string out;
    out.reserve(n * 3 + 2);
    for (int i = 1; i <= n; ++i) {
        if (f[i] == 1) out += "1";
        else out += "-1";
        if (i < n) out += ' ';
    }
    out += '\n';
    cout << out;
    return 0;
}