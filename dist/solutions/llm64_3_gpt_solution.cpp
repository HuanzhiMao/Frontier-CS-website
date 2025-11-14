#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using i64 = long long;
using u128 = __uint128_t;

static inline u64 absdiff_u64(u64 a, u64 b){ return a >= b ? a - b : b - a; }
static inline u128 absdiff_u128(u128 a, u128 b){ return (a >= b) ? (a-b) : (b-a); }

struct Candidate {
    u64 sum;
    u64 maskL;
    u64 maskR;
    u64 diff;
    bool operator<(const Candidate& other) const { return diff < other.diff; }
};

struct BitsetDP {
    size_t M; // maximum bit index
    size_t W; // number of 64-bit words
    vector<vector<u64>> hist; // history per step
    vector<u64> dp;
    u64 lastMask;

    BitsetDP(size_t M_=0): M(M_) {
        W = (M + 64) >> 6;
        dp.assign(W, 0);
        if (M % 64 == 63) {
            lastMask = ~0ULL;
        } else {
            int lastbits = (int)((M + 1) & 63);
            if (lastbits == 0) lastMask = ~0ULL;
            else lastMask = ( (1ULL << lastbits) - 1ULL );
        }
    }
    void init(size_t M_) {
        M = M_;
        W = (M + 64) >> 6;
        dp.assign(W, 0);
        if (M % 64 == 63) {
            lastMask = ~0ULL;
        } else {
            int lastbits = (int)((M + 1) & 63);
            if (lastbits == 0) lastMask = ~0ULL;
            else lastMask = ( (1ULL << lastbits) - 1ULL );
        }
        if (W) dp[0] = 1ULL; // sum 0 reachable
        hist.clear();
        hist.reserve(128);
        hist.push_back(dp);
    }
    inline void orShiftInplace(u64 shift) {
        if (shift == 0) {
            // dp |= dp, no change
            return;
        }
        if (shift > M) {
            // shift exceeds range, contributes nothing
            return;
        }
        size_t sw = (size_t)(shift >> 6);
        int sb = (int)(shift & 63);
        if (sb == 0) {
            for (size_t j = W; j-- > 0;) {
                u64 v = 0;
                if (j >= sw) v = dp[j - sw];
                dp[j] |= v;
            }
        } else {
            for (size_t j = W; j-- > 0;) {
                u64 v = 0;
                if (j >= sw + 1) v |= (dp[j - sw - 1] >> (64 - sb));
                if (j >= sw)     v |= (dp[j - sw] << sb);
                dp[j] |= v;
            }
        }
        dp[W-1] &= lastMask;
    }
    void addWeight(u64 w) {
        orShiftInplace(w);
        hist.push_back(dp);
    }
    inline bool testBit(const vector<u64>& arr, size_t pos) const {
        return (arr[pos >> 6] >> (pos & 63)) & 1ULL;
    }
    // Find nearest set bit to target in [0..M]. Returns index or -1 if none.
    int findNearest(size_t target) const {
        if (target > M) target = M;
        if (testBit(dp, target)) return (int)target;
        size_t w = target >> 6;
        int b = (int)(target & 63);
        // search right
        int right = -1;
        {
            u64 word = dp[w] & (~((1ULL << (b+1)) - 1ULL));
            if (b == 63) word = 0;
            if (word) {
                int t = __builtin_ctzll(word);
                right = (int)((w << 6) + t);
            } else {
                for (size_t j = w + 1; j < W; ++j) {
                    if (dp[j]) {
                        int t = __builtin_ctzll(dp[j]);
                        right = (int)((j << 6) + t);
                        break;
                    }
                }
            }
        }
        // search left
        int left = -1;
        {
            u64 word = dp[w] & ((b == 0) ? 0ULL : ((1ULL << b) - 1ULL));
            if (word) {
                int t = 63 - __builtin_clzll(word);
                left = (int)((w << 6) + t);
            } else {
                for (size_t j = w; j-- > 0;) {
                    if (dp[j]) {
                        int t = 63 - __builtin_clzll(dp[j]);
                        left = (int)((j << 6) + t);
                        break;
                    }
                }
            }
        }
        if (left == -1 && right == -1) return -1;
        if (left == -1) return right;
        if (right == -1) return left;
        size_t dl = target - (size_t)left;
        size_t dr = (size_t)right - target;
        return (dl <= dr) ? left : right;
    }
    // Reconstruct subset indices (relative to input ordering passed externally) for achieving sum 's' (scaled).
    // weights 'w' length m correspond to the steps in hist [1..m]; returns selected indices (positions 0..m-1)
    vector<int> reconstruct(const vector<u64>& w, int s) const {
        vector<int> chosen;
        int m = (int)w.size();
        int cur = s;
        for (int i = m; i >= 1; --i) {
            u64 wi = w[i-1];
            if (wi <= (u64)cur && testBit(hist[i-1], (size_t)(cur - wi))) {
                chosen.push_back(i-1);
                cur -= (int)wi;
            } else {
                // not taken
            }
        }
        reverse(chosen.begin(), chosen.end());
        return chosen;
    }
};

// Meet-in-the-middle exact or near for up to ~42 items
struct MITM {
    vector<u64> vals;
    vector<int> idxs; // original indices
    int n;
    MITM(const vector<u64>& a, const vector<int>& indices): vals(a), idxs(indices) {
        n = (int)vals.size();
    }
    // Returns best mask and sum for these items near target T
    // For n up to ~42, we can handle.
    pair<vector<char>, u64> solve(u64 T, int topC, vector<Candidate>* outCands = nullptr) {
        int nL = n / 2;
        int nR = n - nL;
        // Left sums
        int szL = 1 << nL;
        vector<u64> sumL(szL);
        sumL[0] = 0;
        for (int m = 1; m < szL; ++m) {
            int b = __builtin_ctz(m);
            sumL[m] = sumL[m & (m-1)] + vals[b];
        }
        vector<pair<u64,u64>> L; L.reserve(szL);
        for (int m = 0; m < szL; ++m) {
            L.emplace_back(sumL[m], (u64)m);
        }
        sort(L.begin(), L.end(), [](const auto& x, const auto& y){ return x.first < y.first; });
        vector<u64> Lsums; Lsums.reserve(L.size());
        for (auto &p: L) Lsums.push_back(p.first);

        int szR = 1 << nR;
        vector<u64> sumR(szR);
        sumR[0] = 0;
        for (int m = 1; m < szR; ++m) {
            int b = __builtin_ctz(m);
            sumR[m] = sumR[m & (m-1)] + vals[nL + b];
        }

        u64 bestSum = 0;
        u64 bestDiff = std::numeric_limits<u64>::max();
        u64 bestMaskL = 0, bestMaskR = 0;

        int Cmax = max(1, topC);
        auto cmpHeap = [](const Candidate& a, const Candidate& b){
            if (a.diff != b.diff) return a.diff < b.diff;
            return a.sum < b.sum;
        };
        // max-heap by diff; but C++ priority_queue is max-heap by comparator "less"; We'll invert: store with greater comparator (diff largest on top)
        struct HeapCmp {
            bool operator()(const Candidate& a, const Candidate& b) const {
                if (a.diff != b.diff) return a.diff < b.diff; // for max-heap, we want larger diff to be "less"?
                return a.sum < b.sum;
            }
        };
        // We'll implement manually: store as priority_queue with comparator for "greater" diff (largest on top)
        struct MaxCmp {
            bool operator()(const Candidate& a, const Candidate& b) const {
                if (a.diff != b.diff) return a.diff < b.diff; // True if a has smaller diff -> a below; So b with larger diff goes top
                return a.sum < b.sum;
            }
        };
        priority_queue<Candidate, vector<Candidate>, MaxCmp> pq;

        for (int rm = 0; rm < szR; ++rm) {
            u64 sr = sumR[rm];
            if (sr > T && Lsums[0] > T) {
                // might still be candidate, but we'll binary search anyway
            }
            u64 need = (sr >= T) ? 0ULL : (T - sr);
            auto it = lower_bound(Lsums.begin(), Lsums.end(), need);
            if (it != Lsums.end()) {
                int pos = (int)(it - Lsums.begin());
                u64 s = sr + Lsums[pos];
                u64 diff = absdiff_u64(s, T);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    bestSum = s;
                    bestMaskL = L[pos].second;
                    bestMaskR = (u64)rm;
                }
                Candidate c{ s, L[pos].second, (u64)rm, diff };
                if ((int)pq.size() < Cmax) pq.push(c);
                else if (pq.top().diff > diff) { pq.pop(); pq.push(c); }
            }
            if (it != Lsums.begin()) {
                int pos = (int)((it - Lsums.begin()) - 1);
                u64 s = sr + Lsums[pos];
                u64 diff = absdiff_u64(s, T);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    bestSum = s;
                    bestMaskL = L[pos].second;
                    bestMaskR = (u64)rm;
                }
                Candidate c{ s, L[pos].second, (u64)rm, diff };
                if ((int)pq.size() < Cmax) pq.push(c);
                else if (pq.top().diff > diff) { pq.pop(); pq.push(c); }
            }
        }

        vector<Candidate> topCands;
        while (!pq.empty()) { topCands.push_back(pq.top()); pq.pop(); }
        sort(topCands.begin(), topCands.end(), [](const Candidate& a, const Candidate& b){
            if (a.diff != b.diff) return a.diff < b.diff;
            return a.sum < b.sum;
        });
        if (outCands) *outCands = topCands;

        vector<char> sel(n, 0);
        // fill selection from best masks
        for (int i = 0; i < nL; ++i) {
            if ((bestMaskL >> i) & 1ULL) sel[i] = 1;
        }
        for (int j = 0; j < nR; ++j) {
            if ((bestMaskR >> j) & 1ULL) sel[nL + j] = 1;
        }
        return { sel, bestSum };
    }
};

// Greedy heuristic: iterate in given order, include item if it gets closer to T
static vector<char> greedy_pick(const vector<u64>& a, const vector<int>& order, u64 T) {
    int n = (int)a.size();
    vector<char> sel(n, 0);
    u128 cur = 0;
    for (int idx: order) {
        u128 with = cur + (u128)a[idx];
        u128 d0 = absdiff_u128(cur, (u128)T);
        u128 d1 = absdiff_u128(with, (u128)T);
        if (d1 <= d0) {
            sel[idx] = 1;
            cur = with;
        }
    }
    return sel;
}

static u128 compute_sum_u128(const vector<u64>& a, const vector<char>& sel) {
    u128 s = 0;
    for (size_t i = 0; i < a.size(); ++i) if (sel[i]) s += (u128)a[i];
    return s;
}

// Local improvement: 1-opt and 2-opt
static void local_improve(const vector<u64>& a, u64 T, vector<char>& sel) {
    int n = (int)a.size();
    u128 cur = compute_sum_u128(a, sel);
    u128 T128 = (u128)T;
    auto diff = [&](){ return absdiff_u128(cur, T128); };
    bool improved = true;
    int iter = 0;
    while (improved && iter < 5) {
        improved = false;
        // 1-opt
        for (int i = 0; i < n; ++i) {
            u128 newSum = cur + (sel[i] ? (u128)(- (i128)a[i]) : (u128)a[i]);
            u128 d0 = absdiff_u128(cur, T128);
            u128 d1 = absdiff_u128(newSum, T128);
            if (d1 < d0) {
                sel[i] = !sel[i];
                cur = newSum;
                improved = true;
            }
        }
        // 2-opt: try pair toggles
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                u128 delta = (sel[i] ? (u128)(- (i128)a[i]) : (u128)a[i]) +
                             (sel[j] ? (u128)(- (i128)a[j]) : (u128)a[j]);
                u128 newSum = cur + delta;
                u128 d0 = absdiff_u128(cur, T128);
                u128 d1 = absdiff_u128(newSum, T128);
                if (d1 < d0) {
                    sel[i] = !sel[i];
                    sel[j] = !sel[j];
                    cur = newSum;
                    improved = true;
                }
            }
        }
        ++iter;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    u64 T;
    if (!(cin >> n >> T)) {
        return 0;
    }
    vector<u64> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // Edge handling
    u128 S128 = 0;
    for (int i = 0; i < n; ++i) S128 += (u128)a[i];
    if ((u128)T >= S128) {
        string ans(n, '1');
        cout << ans;
        return 0;
    }
    if (T == 0) {
        string ans(n, '0');
        cout << ans;
        return 0;
    }

    // Best solution tracking
    vector<char> bestSel(n, 0);
    u128 bestDiff = (u128)(~(u64)0ULL);
    auto consider = [&](const vector<char>& sel){
        u128 sum = compute_sum_u128(a, sel);
        u128 d = absdiff_u128(sum, (u128)T);
        if (d < bestDiff) {
            bestDiff = d;
            bestSel = sel;
        }
    };

    // If n small, do exact MITM
    if (n <= 42) {
        vector<int> idxs(n);
        iota(idxs.begin(), idxs.end(), 0);
        MITM mitm(a, idxs);
        auto res = mitm.solve(T, 1, nullptr);
        vector<char> sel = res.first;
        consider(sel);
        local_improve(a, T, bestSel);
        string out(n, '0');
        for (int i = 0; i < n; ++i) out[i] = bestSel[i] ? '1' : '0';
        cout << out;
        return 0;
    }

    // Greedy attempts
    {
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        // Descending
        sort(order.begin(), order.end(), [&](int i, int j){ return a[i] > a[j]; });
        consider(greedy_pick(a, order, T));
        // Ascending
        sort(order.begin(), order.end(), [&](int i, int j){ return a[i] < a[j]; });
        consider(greedy_pick(a, order, T));
        // Original
        iota(order.begin(), order.end(), 0);
        consider(greedy_pick(a, order, T));
    }

    // MITM on top K largest values + DP on rest
    int K = min(n, 38);
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j){ return a[i] > a[j]; });
    vector<int> idxBig(order.begin(), order.begin() + K);
    vector<int> idxSmall(order.begin() + K, order.end());

    vector<u64> valsBig(K), valsSmall(n - K);
    for (int i = 0; i < K; ++i) valsBig[i] = a[idxBig[i]];
    for (int i = 0; i < n - K; ++i) valsSmall[i] = a[idxSmall[i]];

    MITM mitmBig(valsBig, idxBig);
    vector<Candidate> topCands;
    auto mitmRes = mitmBig.solve(T, 300, &topCands);
    vector<char> selBig = mitmRes.first;
    u64 sumBig = mitmRes.second;

    // Build selection vector in original indices for big part
    vector<char> selBase(n, 0);
    for (int i = 0; i < K; ++i) {
        if (selBig[i]) selBase[idxBig[i]] = 1;
    }
    consider(selBase);

    // Evaluate top candidates with DP on rest
    u64 maxTR = 0;
    for (const auto& c: topCands) {
        if (c.sum <= T) {
            maxTR = max(maxTR, T - c.sum);
        }
    }
    if (maxTR > 0 && !idxSmall.empty()) {
        // Scale so that cap ~ 2*maxTR fits in ~ MAX_BITS
        const u64 MAX_BITS = 4000000ULL; // bit capacity
        u128 twice = (u128)maxTR * 2u;
        u64 d = (u64)((twice + (u128)MAX_BITS - 1u) / (u128)MAX_BITS);
        if (d == 0) d = 1;
        u64 cap = (u64)(twice / d);
        if (cap == 0) cap = 1;
        // Weights scaled
        vector<u64> wSmall(valsSmall.size());
        for (size_t i = 0; i < valsSmall.size(); ++i) wSmall[i] = (u64)(valsSmall[i] / d);
        // Ensure at least some non-zero weights; if all zero, try d=1
        bool anyNonZero = false;
        for (auto w: wSmall) if (w) { anyNonZero = true; break; }
        if (!anyNonZero) {
            d = 1;
            for (size_t i = 0; i < valsSmall.size(); ++i) wSmall[i] = (u64)(valsSmall[i] / d);
            // If still all zero, DP is useless; skip
            anyNonZero = false;
            for (auto w: wSmall) if (w) { anyNonZero = true; break; }
        }
        if (anyNonZero) {
            BitsetDP dp(cap);
            dp.init(cap);
            for (size_t i = 0; i < wSmall.size(); ++i) {
                u64 w = wSmall[i];
                dp.addWeight(w);
            }

            // Choose best candidate by approximate nearest sum from dp
            struct Combo { int candIdx; int pos; u64 approxErr; };
            vector<Combo> combos;
            for (int ci = 0; ci < (int)topCands.size(); ++ci) {
                const auto& c = topCands[ci];
                if (c.sum > T) continue; // residual negative, skip
                u64 TR = T - c.sum;
                size_t target = (size_t)(TR / d);
                if (target > dp.M) target = dp.M;
                int pos = dp.findNearest(target);
                if (pos < 0) continue;
                u64 approx = absdiff_u64((u64)TR, (u64)pos * d);
                combos.push_back({ci, pos, approx});
            }
            sort(combos.begin(), combos.end(), [](const Combo& A, const Combo& B){
                if (A.approxErr != B.approxErr) return A.approxErr < B.approxErr;
                return A.candIdx < B.candIdx;
            });
            int take = min((int)combos.size(), 5); // reconstruct only a few best
            for (int k = 0; k < take; ++k) {
                int ci = combos[k].candIdx;
                int pos = combos[k].pos;
                // reconstruct
                vector<int> idxChosenSmall = dp.reconstruct(wSmall, pos);
                // Build selection
                vector<char> sel(n, 0);
                // big part from this candidate
                const auto& c = topCands[ci];
                // decode masks
                int nL = K / 2;
                int nR = K - nL;
                // However MITM stored masks for its own split.
                // We need to reconstruct actual big selection indices (idxBig[])
                // We'll recompute left/right splits as in MITM:
                // left are first nL of valsBig, right are remaining
                for (int i = 0; i < nL; ++i) {
                    if ((c.maskL >> i) & 1ULL) sel[idxBig[i]] = 1;
                }
                for (int j = 0; j < nR; ++j) {
                    if ((c.maskR >> j) & 1ULL) sel[idxBig[nL + j]] = 1;
                }
                // small additions
                u128 sum = 0;
                for (int i = 0; i < K; ++i) if (sel[idxBig[i]]) sum += (u128)a[idxBig[i]];
                for (int t = 0; t < (int)idxChosenSmall.size(); ++t) {
                    int si = idxChosenSmall[t];
                    sel[idxSmall[si]] = 1;
                    sum += (u128)a[idxSmall[si]];
                }
                u128 dcur = absdiff_u128(sum, (u128)T);
                if (dcur < bestDiff) {
                    bestDiff = dcur;
                    bestSel = sel;
                }
            }
        } else {
            // Can't use DP effectively; just consider top candidate big solutions
            for (const auto& c : topCands) {
                vector<char> sel(n, 0);
                int nL = K / 2;
                int nR = K - nL;
                for (int i = 0; i < nL; ++i) if ((c.maskL >> i) & 1ULL) sel[idxBig[i]] = 1;
                for (int j = 0; j < nR; ++j) if ((c.maskR >> j) & 1ULL) sel[idxBig[nL + j]] = 1;
                consider(sel);
            }
        }
    } else {
        // No positive residual available; consider top big candidates
        for (const auto& c : topCands) {
            vector<char> sel(n, 0);
            int nL = K / 2;
            int nR = K - nL;
            for (int i = 0; i < nL; ++i) if ((c.maskL >> i) & 1ULL) sel[idxBig[i]] = 1;
            for (int j = 0; j < nR; ++j) if ((c.maskR >> j) & 1ULL) sel[idxBig[nL + j]] = 1;
            consider(sel);
        }
    }

    // Local improvement from best so far
    local_improve(a, T, bestSel);

    // Output
    string out(n, '0');
    for (int i = 0; i < n; ++i) out[i] = bestSel[i] ? '1' : '0';
    cout << out;
    return 0;
}