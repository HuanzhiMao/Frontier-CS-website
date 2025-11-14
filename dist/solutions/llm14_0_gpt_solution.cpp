#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static const int64 MAX_STEP = 1000000000LL;
static const int MAX_QUERIES = 200000;

mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());

int64 total_steps = 0;
int queries_used = 0;
unordered_map<int, int64> last_time;
int64 ans_gcd = 0;
int collisions = 0;
int last_label_val = -1;

int64 gcdll(int64 a, int64 b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (a == 0) return b;
    if (b == 0) return a;
    return std::gcd(a, b);
}

int read_label() {
    int y;
    if (!(cin >> y)) {
        exit(0);
    }
    return y;
}

int do_walk(int64 x) {
    cout << "walk " << x << endl;
    cout.flush();
    int y = read_label();
    total_steps += x;
    queries_used++;

    auto it = last_time.find(y);
    if (it == last_time.end()) {
        last_time.emplace(y, total_steps);
    } else {
        int64 diff = total_steps - it->second;
        ans_gcd = gcdll(ans_gcd, diff);
        collisions++;
        it->second = total_steps;
    }
    last_label_val = y;
    return y;
}

bool can_spend_steps_for(int64 X, int budget_left) {
    if (X <= 0) return true;
    int64 segments = (X + MAX_STEP - 1) / MAX_STEP;
    return (int64)budget_left >= segments;
}

bool test_divisible_by_n(int64 X) {
    int start_label = last_label_val;
    while (X > 0) {
        int64 step = (X > MAX_STEP ? MAX_STEP : X);
        do_walk(step);
        X -= step;
        // collisions handled inside do_walk
    }
    return last_label_val == start_label;
}

vector<int> small_primes_up_to_100() {
    vector<int> primes;
    vector<bool> is_prime(101, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= 100; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * 2; j <= 100; j += i) is_prime[j] = false;
        }
    }
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initial query to read starting label
    do_walk(0);

    // Sampling phase
    int target_collisions = 16;
    int sample_limit = 170000; // leave room for refinement
    uniform_int_distribution<int64> dist(1, MAX_STEP);

    while (queries_used < sample_limit) {
        int64 x = dist(rng);
        do_walk(x);
        if (ans_gcd > 0 && ans_gcd <= 1000000000LL && collisions >= target_collisions) break;
    }

    // Refinement: try to remove small prime factors from ans_gcd
    if (ans_gcd == 0) {
        // Ensure at least one collision; continue sampling more if needed
        while (queries_used < 180000 && ans_gcd == 0) {
            int64 x = dist(rng);
            do_walk(x);
        }
        if (ans_gcd == 0) {
            // As a fallback, guess 1 (should not happen often)
            cout << "guess " << 1 << endl;
            cout.flush();
            return 0;
        }
    }

    vector<int> primes = small_primes_up_to_100();

    // Attempt to divide out small primes
    for (int p : primes) {
        while (ans_gcd % p == 0) {
            int64 X = ans_gcd / p;
            int budget_left = MAX_QUERIES - queries_used - 1; // leave one for final guess
            if (!can_spend_steps_for(X, budget_left)) break;
            bool ok = test_divisible_by_n(X);
            if (ok) {
                ans_gcd /= p;
            } else {
                break;
            }
            if (ans_gcd <= 1000000000LL) break;
        }
        if (ans_gcd <= 1000000000LL) break;
    }

    // If still too large, spend remaining budget on more sampling to try to reduce gcd
    while (queries_used < MAX_QUERIES - 50 && ans_gcd > 1000000000LL) {
        int64 x = dist(rng);
        do_walk(x);
    }

    // Another quick refinement with small primes if room and still large
    if (ans_gcd > 1000000000LL) {
        for (int p : primes) {
            while (ans_gcd % p == 0) {
                int64 X = ans_gcd / p;
                int budget_left = MAX_QUERIES - queries_used - 1;
                if (!can_spend_steps_for(X, budget_left)) break;
                bool ok = test_divisible_by_n(X);
                if (ok) {
                    ans_gcd /= p;
                } else {
                    break;
                }
                if (ans_gcd <= 1000000000LL) break;
            }
            if (ans_gcd <= 1000000000LL) break;
        }
    }

    if (ans_gcd <= 0) ans_gcd = 1;
    if (ans_gcd > 1000000000LL) {
        // As a last resort, try more sampling until limit
        while (queries_used < MAX_QUERIES - 1 && ans_gcd > 1000000000LL) {
            int64 x = dist(rng);
            do_walk(x);
        }
        if (ans_gcd > 1000000000LL) {
            // If still large, clamp (unlikely). Guessing 1 to stay within bounds.
            ans_gcd = 1;
        }
    }

    cout << "guess " << ans_gcd << endl;
    cout.flush();
    return 0;
}