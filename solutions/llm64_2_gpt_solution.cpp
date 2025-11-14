#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct RNG {
    std::mt19937_64 rng;
    RNG() : rng((uint64_t)chrono::steady_clock::now().time_since_epoch().count()) {}
    int randint(int l, int r) { // inclusive
        std::uniform_int_distribution<int> dist(l, r);
        return dist(rng);
    }
    uint64_t rand64() {
        return rng();
    }
};

struct PairLLMask {
    long long sum;
    uint32_t mask;
};

static inline long long llabs64(long long x) {
    return x >= 0 ? x : -x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long T;
    if (!(cin >> n >> T)) {
        return 0;
    }
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    RNG R;
    auto start_time = chrono::steady_clock::now();
    auto time_passed_ms = [&]() -> double {
        auto now = chrono::steady_clock::now();
        return chrono::duration<double, std::milli>(now - start_time).count();
    };
    double TIME_LIMIT_MS = 950.0; // conservative time budget

    // Helper to compute string from bits
    auto bits_to_string = [&](const vector<uint8_t>& bits) -> string {
        string s;
        s.reserve(n);
        for (int i = 0; i < n; ++i) s.push_back(bits[i] ? '1' : '0');
        return s;
    };

    // Exact meet-in-the-middle for small n
    auto exact_mitm = [&](const vector<long long>& a, long long T) -> pair<vector<uint8_t>, bool> {
        int N = (int)a.size();
        int n1 = N / 2;
        int n2 = N - n1;
        // Left half sums
        vector<long long> leftS(1 << n1, 0);
        for (int m = 1; m < (1 << n1); ++m) {
            int b = __builtin_ctz(m);
            leftS[m] = leftS[m ^ (1 << b)] + a[b];
        }
        // Right half sums with masks
        vector<PairLLMask> rightList;
        rightList.reserve(1 << n2);
        rightList.push_back({0, 0});
        for (int i = 0; i < n2; ++i) {
            int sz = (int)rightList.size();
            long long val = a[n1 + i];
            for (int j = 0; j < sz; ++j) {
                PairLLMask cur = rightList[j];
                rightList.push_back({cur.sum + val, cur.mask | (1u << i)});
            }
        }
        sort(rightList.begin(), rightList.end(), [](const PairLLMask& x, const PairLLMask& y){
            return x.sum < y.sum;
        });
        vector<long long> rightSums;
        rightSums.reserve(rightList.size());
        for (auto &p : rightList) rightSums.push_back(p.sum);

        long long bestErr = LLONG_MAX;
        uint32_t bestLM = 0, bestRM = 0;
        long long bestSum = 0;

        for (int lm = 0; lm < (1 << n1); ++lm) {
            long long left = leftS[lm];
            long long need = T - left;
            auto it = lower_bound(rightSums.begin(), rightSums.end(), need);
            if (it != rightSums.end()) {
                long long rs = *it;
                long long err = llabs64((left + rs) - T);
                if (err < bestErr) {
                    bestErr = err;
                    bestLM = lm;
                    bestRM = rightList[it - rightSums.begin()].mask;
                    bestSum = left + rs;
                    if (bestErr == 0) break;
                }
            }
            if (it != rightSums.begin()) {
                --it;
                long long rs = *it;
                long long err = llabs64((left + rs) - T);
                if (err < bestErr) {
                    bestErr = err;
                    bestLM = lm;
                    bestRM = rightList[it - rightSums.begin()].mask;
                    bestSum = left + rs;
                    if (bestErr == 0) break;
                }
            }
        }

        vector<uint8_t> bits(N, 0);
        for (int i = 0; i < n1; ++i) if ((bestLM >> i) & 1) bits[i] = 1;
        for (int i = 0; i < n2; ++i) if ((bestRM >> i) & 1) bits[n1 + i] = 1;
        return {bits, bestErr == 0};
    };

    if (n <= 36) {
        auto [bits_small, exact] = exact_mitm(a, T);
        cout << bits_to_string(bits_small) << "\n";
        return 0;
    }

    // Heuristic for larger n
    // Initial solutions
    vector<uint8_t> best_bits(n, 0);
    long long best_S = 0;

    auto eval_and_update = [&](const vector<uint8_t>& bits, long long S, vector<uint8_t>& curBestBits, long long& curBestS) {
        if (llabs64(S - T) < llabs64(curBestS - T)) {
            curBestS = S;
            curBestBits = bits;
        }
    };

    // Greedy under T (descending by a)
    {
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int i, int j){ return a[i] > a[j]; });
        vector<uint8_t> bits(n, 0);
        long long S = 0;
        for (int i : idx) {
            if (S + a[i] <= T) {
                S += a[i];
                bits[i] = 1;
            }
        }
        eval_and_update(bits, S, best_bits, best_S);
    }
    // Greedy closeness, descending
    {
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int i, int j){ return a[i] > a[j]; });
        vector<uint8_t> bits(n, 0);
        long long S = 0;
        for (int i : idx) {
            long long add = S + a[i];
            if (llabs64(add - T) < llabs64(S - T)) {
                S = add;
                bits[i] = 1;
            }
        }
        eval_and_update(bits, S, best_bits, best_S);
    }
    // Greedy closeness, ascending
    {
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int i, int j){ return a[i] < a[j]; });
        vector<uint8_t> bits(n, 0);
        long long S = 0;
        for (int i : idx) {
            long long add = S + a[i];
            if (llabs64(add - T) < llabs64(S - T)) {
                S = add;
                bits[i] = 1;
            }
        }
        eval_and_update(bits, S, best_bits, best_S);
    }
    // A few random initializations
    for (int it = 0; it < 3; ++it) {
        vector<uint8_t> bits(n, 0);
        long long S = 0;
        for (int i = 0; i < n; ++i) {
            if (R.rand64() & 1) {
                bits[i] = 1;
                S += a[i];
            }
        }
        eval_and_update(bits, S, best_bits, best_S);
    }

    // Local improvement: random block meet-in-the-middle toggling
    auto block_improve = [&](const vector<int>& block, vector<uint8_t>& bits, long long& S, const vector<long long>& a, long long T) -> bool {
        int k = (int)block.size();
        if (k == 0) return false;
        int k1 = k / 2;
        int k2 = k - k1;

        static thread_local vector<long long> deltaLeft, deltaRight;
        deltaLeft.resize(k1);
        deltaRight.resize(k2);

        for (int i = 0; i < k1; ++i) {
            int idx = block[i];
            deltaLeft[i] = bits[idx] ? -a[idx] : a[idx];
        }
        for (int i = 0; i < k2; ++i) {
            int idx = block[k1 + i];
            deltaRight[i] = bits[idx] ? -a[idx] : a[idx];
        }

        int Lsize = 1 << k1;
        int Rsize = 1 << k2;

        static thread_local vector<PairLLMask> leftList, rightList;
        leftList.resize(Lsize);
        rightList.resize(Rsize);

        leftList[0] = {0, 0};
        for (int m = 1; m < Lsize; ++m) {
            int b = __builtin_ctz(m);
            leftList[m].sum = leftList[m ^ (1 << b)].sum + deltaLeft[b];
            leftList[m].mask = (uint32_t)m;
        }

        rightList[0] = {0, 0};
        for (int m = 1; m < Rsize; ++m) {
            int b = __builtin_ctz(m);
            rightList[m].sum = rightList[m ^ (1 << b)].sum + deltaRight[b];
            rightList[m].mask = (uint32_t)m;
        }

        sort(rightList.begin(), rightList.end(), [](const PairLLMask& x, const PairLLMask& y){
            return x.sum < y.sum;
        });
        static thread_local vector<long long> rightSums;
        rightSums.resize(Rsize);
        for (int i = 0; i < Rsize; ++i) rightSums[i] = rightList[i].sum;

        long long d = T - S;
        long long curErr = llabs64(d);
        long long bestErr = curErr;
        uint32_t bestLM = 0, bestRM = 0;
        long long bestDelta = 0;

        for (int i = 0; i < Lsize; ++i) {
            long long left = leftList[i].sum;
            long long need = d - left;
            auto it = lower_bound(rightSums.begin(), rightSums.end(), need);
            if (it != rightSums.end()) {
                long long val = *it;
                long long newErr = llabs64(need - val);
                if (newErr < bestErr) {
                    bestErr = newErr;
                    bestLM = leftList[i].mask;
                    bestRM = rightList[it - rightSums.begin()].mask;
                    bestDelta = left + val;
                    if (bestErr == 0) break;
                }
            }
            if (it != rightSums.begin()) {
                --it;
                long long val = *it;
                long long newErr = llabs64(need - val);
                if (newErr < bestErr) {
                    bestErr = newErr;
                    bestLM = leftList[i].mask;
                    bestRM = rightList[it - rightSums.begin()].mask;
                    bestDelta = left + val;
                    if (bestErr == 0) break;
                }
            }
        }

        if (bestErr < curErr) {
            // Apply flips
            for (int i = 0; i < k1; ++i) if ((bestLM >> i) & 1) {
                int idx = block[i];
                bits[idx] ^= 1;
            }
            for (int i = 0; i < k2; ++i) if ((bestRM >> i) & 1) {
                int idx = block[k1 + i];
                bits[idx] ^= 1;
            }
            S += bestDelta;
            return true;
        }
        return false;
    };

    // Prepare permutation for picking blocks
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), R.rng);
    int pos = 0;
    vector<uint8_t> cur_bits = best_bits;
    long long cur_S = best_S;

    long long bestErr = llabs64(best_S - T);

    // Set of block sizes to try (balanced for speed/quality)
    vector<int> blockSizes = {24, 26, 22, 28, 30};

    int no_improve_iters = 0;

    while (time_passed_ms() < TIME_LIMIT_MS) {
        for (int bs : blockSizes) {
            if (time_passed_ms() >= TIME_LIMIT_MS) break;
            int k = min(bs, n);
            if (pos + k > n) {
                shuffle(perm.begin(), perm.end(), R.rng);
                pos = 0;
            }
            vector<int> block(k);
            for (int i = 0; i < k; ++i) block[i] = perm[pos + i];
            pos += k;

            bool improved = block_improve(block, cur_bits, cur_S, a, T);
            long long err = llabs64(cur_S - T);
            if (err < bestErr) {
                bestErr = err;
                best_bits = cur_bits;
                best_S = cur_S;
                no_improve_iters = 0;
                if (bestErr == 0) break;
            } else {
                no_improve_iters++;
            }
        }
        if (bestErr == 0) break;

        // Occasional shake: try single bit flip that helps
        if (no_improve_iters > 20) {
            int trials = min(n, 50);
            long long d = T - cur_S;
            long long curErr = llabs64(d);
            int bestIdx = -1;
            long long bestLocalErr = curErr;
            for (int t = 0; t < trials; ++t) {
                int j = R.randint(0, n - 1);
                long long delta = cur_bits[j] ? -a[j] : a[j];
                long long newErr = llabs64(d - delta);
                if (newErr < bestLocalErr) {
                    bestLocalErr = newErr;
                    bestIdx = j;
                }
            }
            if (bestIdx != -1) {
                long long delta = cur_bits[bestIdx] ? -a[bestIdx] : a[bestIdx];
                cur_bits[bestIdx] ^= 1;
                cur_S += delta;
                long long err = llabs64(cur_S - T);
                if (err < bestErr) {
                    bestErr = err;
                    best_bits = cur_bits;
                    best_S = cur_S;
                }
                no_improve_iters = 0;
            } else {
                no_improve_iters = 0;
            }
        }
    }

    cout << bits_to_string(best_bits) << "\n";
    return 0;
}