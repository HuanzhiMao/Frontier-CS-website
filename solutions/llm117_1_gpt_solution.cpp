#include <bits/stdc++.h>
using namespace std;

long long to_ll(const string &s) {
    // Try to parse as long long; fallback to long double rounded if needed
    char *endptr = nullptr;
    long long val = strtoll(s.c_str(), &endptr, 10);
    if (*endptr == '\0') return val;
    long double d = strtold(s.c_str(), nullptr);
    if (isnan(d) || isinf(d)) return 0LL;
    return (long long) llround(d);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<string> tok;
    string t;
    while (cin >> t) tok.push_back(t);
    if (tok.empty()) return 0;
    
    long long n = to_ll(tok[0]);
    if (n < 0) n = 0;
    size_t need = 1 + 2 * (size_t)n;
    
    vector<long long> a(n, 0), b(n, 0);
    if (tok.size() >= need) {
        for (long long i = 0; i < n; ++i) a[i] = to_ll(tok[1 + i]);
        for (long long i = 0; i < n; ++i) b[i] = to_ll(tok[1 + n + i]);
    }
    // Output: a_1 ... a_n b_1 ... b_n
    for (long long i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    for (long long i = 0; i < n; ++i) {
        cout << ' ' << b[i];
    }
    cout << '\n';
    return 0;
}