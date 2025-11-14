#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<long long> X(N), Y(N);
    for (int i = 0; i < N; ++i) cin >> X[i] >> Y[i];

    // Construct initial route: odd indices ascending, then even indices descending
    vector<int> route;
    route.reserve(N + 1);
    route.push_back(0);
    for (int i = 1; i < N; i += 2) route.push_back(i);
    int s = ((N - 1) % 2 == 0) ? (N - 1) : (N - 2);
    for (int i = s; i >= 2; i -= 2) route.push_back(i);
    route.push_back(0);

    // Fallback to baseline if for any reason size mismatch (safety)
    if ((int)route.size() != N + 1) {
        route.clear();
        route.reserve(N + 1);
        route.push_back(0);
        for (int i = 1; i < N; ++i) route.push_back(i);
        route.push_back(0);
    }

    // Sieve primes up to N-1 (city IDs)
    vector<char> isPrime(N, true);
    if (N > 0) isPrime[0] = false;
    if (N > 1) isPrime[1] = false;
    for (int p = 2; 1LL * p * p < N; ++p) {
        if (isPrime[p]) {
            for (long long q = 1LL * p * p; q < N; q += p) isPrime[(int)q] = false;
        }
    }

    // Prepare set of positions (1..N-1) where route[pos] is prime and not locked
    vector<char> locked(N + 1, false);
    set<int> primePos;
    for (int pos = 1; pos <= N - 1; ++pos) {
        if (isPrime[route[pos]]) primePos.insert(pos);
    }

    // Ensure every 10th step starts from a prime city if possible
    for (int t = 10; t <= N; t += 10) {
        int idx = t - 1;
        if (idx < 1 || idx > N - 1) continue;
        if (locked[idx]) continue;
        if (isPrime[route[idx]]) {
            locked[idx] = true;
            primePos.erase(idx);
            continue;
        }
        if (primePos.empty()) continue;
        auto it = primePos.lower_bound(idx);
        int j;
        if (it == primePos.begin()) {
            j = *it;
        } else if (it == primePos.end()) {
            j = *prev(it);
        } else {
            int r = *it;
            int l = *prev(it);
            j = (abs(r - idx) < abs(idx - l)) ? r : l;
        }
        if (j >= 1 && j <= N - 1) {
            swap(route[idx], route[j]);
            locked[idx] = true;
            primePos.erase(j);
        }
    }

    cout << (N + 1) << '\n';
    for (int i = 0; i <= N; ++i) {
        cout << route[i] << '\n';
    }

    return 0;
}