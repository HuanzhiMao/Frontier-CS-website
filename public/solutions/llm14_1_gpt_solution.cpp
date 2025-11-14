#include <bits/stdc++.h>
using namespace std;

static const int64_t LIMIT_X = 1000000000LL;
static const int MAX_WALKS = 200000;

struct RNG {
    uint64_t state;
    RNG() {
        uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        state = seed ^ (seed << 13) ^ (seed >> 7) ^ (seed << 17);
        if (state == 0) state = 0x9e3779b97f4a7c15ULL;
    }
    uint64_t next() {
        // xorshift64*
        state ^= state << 7;
        state ^= state >> 9;
        state ^= state << 8;
        return state;
    }
    uint32_t uniform(uint32_t l, uint32_t r) {
        return l + (uint32_t)(next() % (uint64_t)(r - l + 1));
    }
};

struct Interaction {
    long long total_steps = 0;
    long long cur_label = -1;
    int qcount = 0;

    long long walk(long long x) {
        if (x < 0) x = 0;
        if (x > LIMIT_X) x = LIMIT_X;
        cout << "walk " << x << endl;
        cout.flush();
        long long res;
        if (!(cin >> res)) {
            exit(0);
        }
        total_steps += x;
        cur_label = res;
        ++qcount;
        return res;
    }

    void guess(long long g) {
        cout << "guess " << g << endl;
        cout.flush();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Interaction it;
    RNG rng;

    // Get initial label at starting vertex
    it.walk(0);

    // Parameters for random walk to find a short-period collision
    // We'll use a sliding window of size W and step sizes in [1, B] such that W * B <= 1e9
    // Start with a strong setting to get high probability of collision quickly
    int64_t W = 80000; // window size
    int64_t B = LIMIT_X / W; // max step so that W*B <= 1e9
    if (B < 1) B = 1;

    deque<pair<int, long long>> window; // (label, total_steps)
    window.emplace_back((int)it.cur_label, it.total_steps);

    unordered_map<int, long long> last_pos;
    last_pos.reserve((size_t)W * 2);
    last_pos.max_load_factor(0.7f);
    last_pos[(int)it.cur_label] = it.total_steps;

    long long baseG = 0;

    auto try_find_collision = [&](int64_t &W_target, int64_t &B_target, int max_additional_steps) {
        // Continue adding steps until we find a collision or we exhaust the allowed additional steps
        for (int cnt = 0; cnt < max_additional_steps; ++cnt) {
            if (it.qcount >= MAX_WALKS - 50) break; // keep some budget for factor checks
            int64_t step = rng.uniform(1, (uint32_t)B_target);
            it.walk(step);
            int lbl = (int)it.cur_label;

            auto itmp = last_pos.find(lbl);
            if (itmp != last_pos.end()) {
                long long D = it.total_steps - itmp->second;
                if (D > 0 && D <= LIMIT_X) {
                    baseG = D; // Found a collision with difference <= 1e9
                    return;
                }
                // If D > LIMIT_X, we still can't use it for fast checks, so ignore this and continue.
                // Update to latest occurrence anyway to try for a shorter future difference.
            }

            window.emplace_back(lbl, it.total_steps);
            last_pos[lbl] = it.total_steps;

            if ((int64_t)window.size() > W_target) {
                auto pr = window.front();
                window.pop_front();
                auto f = last_pos.find(pr.first);
                if (f != last_pos.end() && f->second == pr.second) {
                    last_pos.erase(f);
                }
            }
        }
    };

    // First attempt
    try_find_collision(W, B, 100000);

    // If not found, enlarge the window to increase collision probability while keeping B small enough
    if (baseG == 0 && it.qcount < MAX_WALKS - 50) {
        int64_t W2 = 160000;
        int64_t B2 = LIMIT_X / W2;
        if (B2 < 1) B2 = 1;
        W = W2; B = B2;
        // Reserve more space for the map if needed
        last_pos.reserve((size_t)W * 2);
        try_find_collision(W, B, MAX_WALKS - 50 - it.qcount);
    }

    // If still not found, as a fallback, just guess 1 (worst-case fallback, unlikely to happen in practice)
    if (baseG == 0) {
        it.guess(1);
        return 0;
    }

    // Factor baseG to get its prime factors (unique)
    long long G = baseG;
    vector<long long> primes;
    {
        long long x = G;
        for (long long p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                primes.push_back(p);
                while (x % p == 0) x /= p;
            }
        }
        if (x > 1) primes.push_back(x);
    }

    auto test_period = [&](long long d) -> bool {
        // Test if d is a multiple of n by checking if the label repeats after d steps
        if (it.qcount >= MAX_WALKS) return false;
        long long before = it.cur_label;
        it.walk(d);
        return it.cur_label == before;
    };

    // Reduce G by testing its prime factors
    for (long long p : primes) {
        while (G % p == 0 && it.qcount < MAX_WALKS) {
            long long d = G / p;
            if (d <= 0) break;
            if (test_period(d)) {
                G = d;
            } else {
                break;
            }
        }
    }

    it.guess(G);
    return 0;
}