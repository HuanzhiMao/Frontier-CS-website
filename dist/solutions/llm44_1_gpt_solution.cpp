#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<long long> X(N), Y(N);
    for (int i = 0; i < N; ++i) cin >> X[i] >> Y[i];

    // Sieve primes up to N-1 (city IDs)
    vector<char> isPrime(N, false);
    if (N > 2) {
        vector<char> prime(N, true);
        prime[0] = false;
        if (N > 1) prime[1] = false;
        for (int p = 2; 1LL * p * p <= N - 1; ++p) {
            if (prime[p]) {
                for (int q = p * p; q <= N - 1; q += p) prime[q] = false;
            }
        }
        isPrime = move(prime);
    }

    // Build serpentine order by grouping consecutive IDs (x-sorted) into stripes and sorting by y within each stripe
    vector<int> order;
    order.reserve(max(0, N - 1));

    int groupSize = max(1, (int)floor(sqrt(max(1, N - 1))));
    int S = (N - 1 + groupSize - 1) / groupSize;
    for (int g = 0; g < S; ++g) {
        int start = g * groupSize + 1;
        int end = min(N - 1, (g + 1) * groupSize);
        if (start > end) continue;
        vector<int> ids;
        ids.reserve(end - start + 1);
        for (int id = start; id <= end; ++id) ids.push_back(id);
        if ((g & 1) == 0) {
            stable_sort(ids.begin(), ids.end(), [&](int a, int b) {
                if (Y[a] == Y[b]) return X[a] < X[b];
                return Y[a] < Y[b];
            });
        } else {
            stable_sort(ids.begin(), ids.end(), [&](int a, int b) {
                if (Y[a] == Y[b]) return X[a] < X[b];
                return Y[a] > Y[b];
            });
        }
        for (int id : ids) order.push_back(id);
    }

    // Build final path using a small lookahead buffer to place primes at positions i%10==9 (0-based i on P[1..N-1])
    const int W = 30; // lookahead window size
    vector<int> buffer;
    buffer.reserve(W + 5);
    vector<int> primePos; // indices of primes within buffer
    primePos.reserve(W + 5);

    int nextIdx = 0;
    int orderLen = (int)order.size();
    while ((int)buffer.size() < W && nextIdx < orderLen) {
        int id = order[nextIdx++];
        buffer.push_back(id);
        if (isPrime[id]) primePos.push_back((int)buffer.size() - 1);
    }

    vector<int> path(N + 1, 0);
    path[0] = 0;
    for (int pos = 1; pos <= N - 1; ++pos) {
        int chosen;
        if (pos % 10 == 9 && !primePos.empty()) {
            int j = primePos.front();
            chosen = buffer[j];
            buffer.erase(buffer.begin() + j);
            primePos.erase(primePos.begin());
            for (int &idx : primePos) if (idx > j) --idx;
        } else {
            chosen = buffer.front();
            bool frontIsPrime = isPrime[chosen];
            buffer.erase(buffer.begin());
            for (int &idx : primePos) --idx;
            if (frontIsPrime && !primePos.empty() && primePos.front() == -1) {
                primePos.erase(primePos.begin());
            }
        }
        path[pos] = chosen;
        if (nextIdx < orderLen) {
            int id = order[nextIdx++];
            buffer.push_back(id);
            if (isPrime[id]) primePos.push_back((int)buffer.size() - 1);
        }
    }
    path[N] = 0;

    // Output
    cout << (N + 1) << '\n';
    for (int i = 0; i <= N; ++i) {
        cout << path[i] << '\n';
    }
    return 0;
}