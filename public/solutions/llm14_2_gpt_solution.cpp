#include <bits/stdc++.h>
using namespace std;

static const long long MAX_X = 1000000000LL;
static const int MAX_QUERIES = 200000;

// Global state
long long query_count = 0;
long long current_label = -1;

// Send a walk command with x steps and read the returned label
long long walk_and_get(long long x) {
    cout << "walk " << x << endl;
    cout.flush();
    long long y;
    if (!(cin >> y)) exit(0); // In case of judge error/EOF
    current_label = y;
    query_count++;
    return y;
}

// Perform E steps possibly in chunks (since each x must be <= 1e9)
// Return true if after total E steps we returned to the starting label; otherwise false.
bool is_multiple(long long E) {
    if (E == 0) return true;
    long long start_label = current_label;
    while (E > 0) {
        long long step = min(E, MAX_X);
        walk_and_get(step);
        E -= step;
    }
    return (current_label == start_label);
}

// Simple sieve to generate primes up to limit
vector<int> sieve_primes(int limit) {
    vector<int> primes;
    if (limit < 2) return primes;
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (long long j = 1LL * i * i; j <= limit; j += i) {
                isPrime[(int)j] = false;
            }
        }
    }
    for (int i = 2; i <= limit; ++i) if (isPrime[i]) primes.push_back(i);
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Get starting label
    walk_and_get(0);

    // Random engine
    std::mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());

    // Parameters for random walking
    const long long B = 1000000LL; // max random step per walk (keeps D manageable)
    std::uniform_int_distribution<long long> dist(1, B);

    // Record labels to detect first collision
    unordered_map<long long, long long> first_pos;
    first_pos.reserve(200000);
    first_pos.max_load_factor(0.7f);

    long long sum_steps = 0;
    first_pos[current_label] = 0;

    long long D = -1;
    // Reserve some budget for factorization queries. We'll try up to 195000 random walks.
    int max_random_walks = 195000;

    for (int i = 0; i < max_random_walks; ++i) {
        long long step = dist(rng);
        long long lbl = walk_and_get(step);
        sum_steps += step;

        auto it = first_pos.find(lbl);
        if (it != first_pos.end()) {
            D = sum_steps - it->second;
            break;
        } else {
            first_pos[lbl] = sum_steps;
        }
    }

    if (D <= 0) {
        // Fallback: If collision not found (extremely unlikely), try deterministic small extra steps
        // to force collision within remaining budget.
        // We'll just walk small random steps until collision or close to limit.
        while (query_count + 1000 < MAX_QUERIES && D <= 0) {
            long long step = dist(rng);
            long long lbl = walk_and_get(step);
            sum_steps += step;
            auto it = first_pos.find(lbl);
            if (it != first_pos.end()) {
                D = sum_steps - it->second;
                break;
            } else {
                first_pos[lbl] = sum_steps;
            }
        }
        if (D <= 0) {
            // As an absolute last resort, guess 1 (will be wrong, but prevents undefined behavior)
            cout << "guess " << 1 << endl;
            cout.flush();
            return 0;
        }
    }

    // Factorize D using trial division
    long long D_init = D;
    long long ntemp = D_init;
    int lim = (int)(sqrtl((long double)D_init) + 2);
    vector<int> primes = sieve_primes(lim);
    vector<pair<long long, int>> factors;

    for (int p : primes) {
        if (1LL * p * p > ntemp) break;
        if (ntemp % p == 0) {
            int cnt = 0;
            while (ntemp % p == 0) {
                ntemp /= p;
                cnt++;
            }
            factors.push_back({p, cnt});
        }
    }
    if (ntemp > 1) {
        factors.push_back({ntemp, 1});
    }

    // Reduce D_init to n by testing divisibility using walk queries (chunked if needed)
    long long T = D_init;
    for (auto &fc : factors) {
        long long p = fc.first;
        int cnt = fc.second;
        for (int i = 0; i < cnt; ++i) {
            long long candidate = T / p;
            if (is_multiple(candidate)) {
                T = candidate;
            } else {
                break;
            }
        }
    }

    // Output the guess
    cout << "guess " << T << endl;
    cout.flush();
    return 0;
}