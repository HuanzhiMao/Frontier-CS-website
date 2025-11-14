#include <bits/stdc++.h>
using namespace std;

struct City {
    long long x, y;
};

static inline double dist(const City& a, const City& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return sqrt((double)dx * dx + (double)dy * dy);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<City> cities(N);
    for (int i = 0; i < N; ++i) {
        cin >> cities[i].x >> cities[i].y;
    }

    // Sieve for primes up to N-1 (city IDs)
    vector<char> isPrime(N, true);
    if (N > 0) isPrime[0] = false;
    if (N > 1) isPrime[1] = false;
    for (int i = 2; (long long)i * i < N; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j < N; j += i) isPrime[j] = false;
        }
    }

    // Baseline path: 0 -> 1 -> 2 -> ... -> N-1 -> 0
    vector<int> P(N + 1);
    P[0] = 0;
    for (int i = 1; i <= N - 1; ++i) P[i] = i;
    P[N] = 0;

    // Improve carrot constraint locally within non-overlapping 10-size blocks:
    // For each step t=10,20,...,N (if applicable), enforce P[t-1] to be prime by swapping within indices [t-10, t-1]
    if (N >= 10) {
        int maxK = N / 10; // number of penalty steps
        for (int k = 1; k <= maxK; ++k) {
            int pos = 10 * k - 1; // index in P that should be prime (1..N-1)
            if (pos < 1 || pos > N - 1) continue;
            if (isPrime[P[pos]]) continue; // already prime

            int start = pos - 9;
            int end = pos;
            // Search for a prime within [start, end], preferring nearest to pos
            int bestIdx = -1;
            for (int d = 0; d <= 9; ++d) {
                int j1 = pos - d;
                if (j1 >= start && isPrime[P[j1]]) { bestIdx = j1; break; }
                int j2 = pos + d;
                if (j2 <= end && isPrime[P[j2]]) { bestIdx = j2; break; }
            }
            if (bestIdx != -1 && bestIdx != pos) {
                swap(P[pos], P[bestIdx]);
            }
        }
    }

    // Output
    cout << (N + 1) << '\n';
    for (int i = 0; i <= N; ++i) {
        cout << P[i] << '\n';
    }
    return 0;
}