#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto gcdll = [](long long a, long long b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) {
            long long t = a % b;
            a = b;
            b = t;
        }
        return a;
    };

    mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count() ^ (uint64_t)(uintptr_t)new int);

    auto rand_x = [&]() -> long long {
        return (long long)(rng() % 1000000000ULL) + 1; // [1, 1e9]
    };

    long long T = 0;
    long long g = 0;
    int q = 0;

    unordered_map<int, long long> first_seen;
    first_seen.reserve(300000);
    first_seen.max_load_factor(0.7f);

    // Initial query to get starting label
    cout << "walk " << 0 << endl;
    cout.flush();
    int label;
    if (!(cin >> label)) return 0;
    q++;
    first_seen.emplace(label, T);

    int unchanged_repeats = 0;
    const int TARGET_UNCHANGED_REPEATS = 25;
    const int MAX_QUERIES = 180000;

    while (q < MAX_QUERIES) {
        long long x = rand_x();
        cout << "walk " << x << endl;
        cout.flush();
        int v;
        if (!(cin >> v)) return 0;
        q++;
        T += x;

        auto it = first_seen.find(v);
        if (it == first_seen.end()) {
            first_seen.emplace(v, T);
        } else {
            long long diff = T - it->second;
            if (diff < 0) diff = -diff;
            long long new_g = (g == 0) ? diff : gcdll(g, diff);
            if (new_g == g) {
                unchanged_repeats++;
            } else {
                g = new_g;
                unchanged_repeats = 0;
            }
            // Keep earliest occurrence to diversify differences
            // Do not update it->second

            if (g == 1) break;
            if (unchanged_repeats >= TARGET_UNCHANGED_REPEATS && g > 0 && g <= 1000000000LL) break;
        }
    }

    if (g == 0) g = 1;
    if (g > 1000000000LL) {
        // Try a few more repeats to reduce g if possible
        int extra_repeats = 0;
        while (q < 200000 && extra_repeats < 50 && g > 1000000000LL) {
            long long x = rand_x();
            cout << "walk " << x << endl;
            cout.flush();
            int v;
            if (!(cin >> v)) break;
            q++;
            T += x;
            auto it = first_seen.find(v);
            if (it == first_seen.end()) {
                first_seen.emplace(v, T);
            } else {
                long long diff = T - it->second;
                if (diff < 0) diff = -diff;
                long long new_g = gcdll(g, diff);
                if (new_g != g) {
                    g = new_g;
                } else {
                    extra_repeats++;
                }
            }
        }
        if (g == 0) g = 1;
        if (g > 1000000000LL) g = (g % 1000000000LL == 0 ? 1000000000LL : (long long)(min(g, 1000000000LL)));
    }

    cout << "guess " << g << endl;
    cout.flush();
    return 0;
}