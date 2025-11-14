#include <bits/stdc++.h>
using namespace std;

struct Cand {
    unsigned long long sum;
    unsigned long long lo;
    unsigned long long hi;
};

static inline unsigned long long absdiff(unsigned long long a, unsigned long long b){
    return (a >= b) ? (a - b) : (b - a);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    unsigned long long T;
    if (!(cin >> n >> T)) return 0;
    vector<unsigned long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // Quick edge checks
    unsigned long long sumAll = 0;
    for (auto v : a) {
        sumAll += v;
    }
    // If best is clearly all or none
    if (T == 0) {
        for (int i = 0; i < n; ++i) cout << '0';
        cout << '\n';
        return 0;
    }
    if (T >= sumAll) {
        for (int i = 0; i < n; ++i) cout << '1';
        cout << '\n';
        return 0;
    }

    auto build_string = [&](unsigned long long lo, unsigned long long hi){
        string s(n, '0');
        for (int i = 0; i < n; ++i){
            if (i < 64){
                if (lo & (1ULL << i)) s[i] = '1';
            } else {
                if (hi & (1ULL << (i - 64))) s[i] = '1';
            }
        }
        return s;
    };

    auto local_improve = [&](Cand &best) {
        // Greedy 1-flip local search
        bool improved = true;
        int iters = 0;
        while (improved && iters < 5) {
            improved = false;
            unsigned long long curDiff = absdiff(best.sum, T);
            int bestIdx = -1;
            unsigned long long bestNewSum = best.sum;
            for (int j = 0; j < n; ++j) {
                bool has = (j < 64) ? (best.lo >> j) & 1ULL : (best.hi >> (j - 64)) & 1ULL;
                unsigned long long ns = has ? (best.sum - a[j]) : (best.sum + a[j]);
                unsigned long long nd = absdiff(ns, T);
                if (nd < curDiff) {
                    curDiff = nd;
                    bestIdx = j;
                    bestNewSum = ns;
                }
            }
            if (bestIdx != -1) {
                // flip
                if (bestIdx < 64) best.lo ^= (1ULL << bestIdx);
                else best.hi ^= (1ULL << (bestIdx - 64));
                best.sum = bestNewSum;
                improved = true;
            }
            ++iters;
        }
    };

    auto update_best = [&](Cand &best, const Cand &c){
        if (absdiff(c.sum, T) < absdiff(best.sum, T) ||
            (absdiff(c.sum, T) == absdiff(best.sum, T) && c.sum < best.sum)) {
            best = c;
        }
    };

    // Beam search runner
    auto run_beam = [&](const vector<int>& order, int W, Cand &globalBest){
        vector<Cand> v;
        v.reserve(W);
        v.push_back({0ULL, 0ULL, 0ULL});
        Cand localBest = v[0];
        update_best(globalBest, localBest);

        vector<Cand> all;
        all.reserve(min(2*W, 1<<20));

        for (int idx : order) {
            all.clear();
            all.reserve(min((int)all.capacity(), (int)min((size_t)2*W, v.size()*2)));
            // expand
            for (const Cand &c : v) {
                // without
                all.push_back(c);
                // with
                Cand c2 = c;
                c2.sum = c.sum + a[idx];
                if (idx < 64) c2.lo |= (1ULL << idx);
                else c2.hi |= (1ULL << (idx - 64));
                all.push_back(c2);
            }

            // Keep top W by closeness to T
            if ((int)all.size() > W) {
                nth_element(all.begin(), all.begin() + W, all.end(), [&](const Cand& x, const Cand& y){
                    unsigned long long dx = absdiff(x.sum, T);
                    unsigned long long dy = absdiff(y.sum, T);
                    if (dx != dy) return dx < dy;
                    return x.sum < y.sum;
                });
                all.resize(W);
            } else {
                sort(all.begin(), all.end(), [&](const Cand& x, const Cand& y){
                    unsigned long long dx = absdiff(x.sum, T);
                    unsigned long long dy = absdiff(y.sum, T);
                    if (dx != dy) return dx < dy;
                    return x.sum < y.sum;
                });
            }

            // Deduplicate by sum to keep diversity
            vector<Cand> nv;
            nv.reserve(W);
            unordered_set<unsigned long long> seen;
            seen.reserve(all.size() * 2);
            for (const auto &c : all) {
                if (seen.insert(c.sum).second) {
                    nv.push_back(c);
                    if ((int)nv.size() == W) break;
                }
            }
            v.swap(nv);

            // Track best
            if (!v.empty()) {
                // v is not guaranteed sorted now; find best element
                for (const auto &c : v) {
                    update_best(localBest, c);
                }
                update_best(globalBest, localBest);
            }
        }

        // Local improvement from best found in this run
        local_improve(localBest);
        update_best(globalBest, localBest);
    };

    // Prepare multiple orders
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);

    vector<int> order_orig = idx;

    vector<int> order_desc = idx;
    sort(order_desc.begin(), order_desc.end(), [&](int i, int j){
        if (a[i] != a[j]) return a[i] > a[j];
        return i < j;
    });

    vector<int> order_asc = idx;
    sort(order_asc.begin(), order_asc.end(), [&](int i, int j){
        if (a[i] != a[j]) return a[i] < a[j];
        return i < j;
    });

    // Order by closeness to T/n heuristic (process items around T/n first)
    long double targetUnit = (long double)T / max(1, n);
    vector<int> order_tunit = idx;
    sort(order_tunit.begin(), order_tunit.end(), [&](int i, int j){
        long double di = fabsl((long double)a[i] - targetUnit);
        long double dj = fabsl((long double)a[j] - targetUnit);
        if (di != dj) return di < dj;
        return i < j;
    });

    // Prepare randomness
    std::mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    auto make_random_order = [&](int seedinc){
        vector<int> o = idx;
        // shuffle with RNG
        shuffle(o.begin(), o.end(), rng);
        return o;
    };

    // Global best initialization: try trivial greedy improvement from 0 and from all
    Cand globalBest{0ULL, 0ULL, 0ULL};
    // Greedy by descending: add if improves distance
    {
        unsigned long long sum = 0;
        unsigned long long lo = 0, hi = 0;
        for (int i : order_desc) {
            unsigned long long ns = sum + a[i];
            if (absdiff(ns, T) <= absdiff(sum, T)) {
                sum = ns;
                if (i < 64) lo |= (1ULL << i);
                else hi |= (1ULL << (i - 64));
            }
        }
        Cand c{sum, lo, hi};
        if (absdiff(c.sum, T) < absdiff(globalBest.sum, T) || (absdiff(c.sum, T) == absdiff(globalBest.sum, T) && c.sum < globalBest.sum)) {
            globalBest = c;
        }
    }
    // Greedy by ascending
    {
        unsigned long long sum = 0;
        unsigned long long lo = 0, hi = 0;
        for (int i : order_asc) {
            unsigned long long ns = sum + a[i];
            if (absdiff(ns, T) <= absdiff(sum, T)) {
                sum = ns;
                if (i < 64) lo |= (1ULL << i);
                else hi |= (1ULL << (i - 64));
            }
        }
        Cand c{sum, lo, hi};
        if (absdiff(c.sum, T) < absdiff(globalBest.sum, T) || (absdiff(c.sum, T) == absdiff(globalBest.sum, T) && c.sum < globalBest.sum)) {
            globalBest = c;
        }
    }

    // Beam parameters
    int W = 8192; // beam width
    if (n <= 40) W = 16384;
    if (n <= 25) W = 20000;
    W = min(W, 30000);

    // Time budget
    auto start = chrono::steady_clock::now();
    const double TIME_LIMIT = 1.8; // seconds
    auto elapsed_sec = [&](){
        return chrono::duration<double>(chrono::steady_clock::now() - start).count();
    };

    // Run several orders within time
    vector<vector<int>> orders;
    orders.push_back(order_desc);
    orders.push_back(order_asc);
    orders.push_back(order_orig);
    orders.push_back(order_tunit);

    int random_runs = 10;
    for (int i = 0; i < random_runs; ++i) {
        orders.push_back(make_random_order(i));
    }

    for (auto &ord : orders) {
        if (elapsed_sec() > TIME_LIMIT) break;
        run_beam(ord, W, globalBest);
    }

    // Final local improvement
    auto final_best = globalBest;
    // Try from complement as a starting point if closer (only if beneficial)
    {
        unsigned long long lo = 0, hi = 0;
        // complement of current selection
        lo = ~globalBest.lo;
        hi = ~globalBest.hi;
        // mask only n bits
        if (n < 64) {
            if (n < 64) {
                unsigned long long mask = (n == 64 ? ~0ULL : ((1ULL << n) - 1));
                lo &= mask;
                hi = 0;
            }
        } else {
            unsigned long long mask_lo = ~0ULL;
            unsigned long long mask_hi = (n == 64 ? 1ULL : ((1ULL << (n - 64)) - 1));
            hi &= mask_hi;
            lo &= mask_lo;
        }
        // compute sum of complement
        unsigned long long s = sumAll - globalBest.sum;
        Cand comp{s, lo, hi};
        if (absdiff(comp.sum, T) < absdiff(final_best.sum, T)) {
            final_best = comp;
        }
    }
    // local improve both
    local_improve(final_best);
    if (absdiff(final_best.sum, T) < absdiff(globalBest.sum, T)) globalBest = final_best;

    // Output bitstring
    cout << build_string(globalBest.lo, globalBest.hi) << '\n';
    return 0;
}