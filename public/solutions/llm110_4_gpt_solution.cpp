#include <bits/stdc++.h>
using namespace std;

static const int R = 8, C = 14, N = R * C;

struct Bits {
    unsigned long long lo, hi;
    Bits(): lo(0), hi(0) {}
    inline void reset() { lo = 0; hi = 0; }
    inline bool any() const { return (lo | hi) != 0ULL; }
    inline void set(int idx) {
        if (idx < 64) lo |= (1ULL << idx);
        else hi |= (1ULL << (idx - 64));
    }
    inline void clear_bit(int idx) {
        if (idx < 64) lo &= ~(1ULL << idx);
        else hi &= ~(1ULL << (idx - 64));
    }
    inline Bits operator|(const Bits &o) const { Bits r; r.lo = lo | o.lo; r.hi = hi | o.hi; return r; }
    inline Bits operator&(const Bits &o) const { Bits r; r.lo = lo & o.lo; r.hi = hi & o.hi; return r; }
    inline Bits& operator|=(const Bits &o) { lo |= o.lo; hi |= o.hi; return *this; }
    inline Bits& operator&=(const Bits &o) { lo &= o.lo; hi &= o.hi; return *this; }
};

static vector<Bits> ADJ;

inline Bits neighbor_of(const Bits &cur) {
    Bits res;
    unsigned long long t = cur.lo;
    while (t) {
        int i = __builtin_ctzll(t);
        res |= ADJ[i];
        t &= t - 1;
    }
    t = cur.hi;
    while (t) {
        int i = __builtin_ctzll(t);
        res |= ADJ[64 + i];
        t &= t - 1;
    }
    return res;
}

struct State {
    array<int, N> g{};
    array<Bits, 10> mask{};
};

inline void rebuildMask(State &st) {
    for (int d = 0; d < 10; ++d) { st.mask[d].reset(); }
    for (int i = 0; i < N; ++i) st.mask[st.g[i]].set(i);
}

inline void setDigit(State &st, int pos, int nd, int &od) {
    od = st.g[pos];
    if (od == nd) return;
    st.mask[od].clear_bit(pos);
    st.g[pos] = nd;
    st.mask[nd].set(pos);
}

inline void swapDigits(State &st, int i, int j) {
    if (i == j) return;
    int di = st.g[i], dj = st.g[j];
    if (di == dj) return;
    st.mask[di].clear_bit(i);
    st.mask[dj].clear_bit(j);
    st.mask[di].set(j);
    st.mask[dj].set(i);
    swap(st.g[i], st.g[j]);
}

inline bool can_read(const State &st, const string &s) {
    Bits cur = st.mask[s[0] - '0'];
    if (!cur.any()) return false;
    for (size_t i = 1; i < s.size(); ++i) {
        Bits next = neighbor_of(cur);
        next &= st.mask[s[i] - '0'];
        if (!next.any()) return false;
        cur = next;
    }
    return true;
}

int evaluate(const State &st, const vector<string> &nums, int cap) {
    for (int k = 1; k <= cap; ++k) {
        if (!can_read(st, nums[k])) return k - 1;
    }
    return cap;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Build adjacency
    ADJ.assign(N, Bits());
    auto idx = [](int r, int c) { return r * C + c; };
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            int u = idx(r, c);
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (0 <= nr && nr < R && 0 <= nc && nc < C) {
                        int v = idx(nr, nc);
                        ADJ[u].set(v);
                    }
                }
            }
        }
    }

    // Precompute strings for numbers
    const int PRECOMP = 6000;
    vector<string> nums(PRECOMP + 1);
    for (int i = 0; i <= PRECOMP; ++i) nums[i] = to_string(i);

    // RNG
    uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937_64 rng(seed);
    uniform_int_distribution<int> dig(0, 9);
    uniform_int_distribution<int> posDist(0, N - 1);
    uniform_int_distribution<int> cellDist(0, N - 1);
    uniform_real_distribution<double> real01(0.0, 1.0);

    // Initial random state
    State cur;
    for (int i = 0; i < N; ++i) cur.g[i] = dig(rng);
    // Ensure each digit 0..9 appears at least once
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    for (int d = 0; d < 10; ++d) {
        cur.g[order[d]] = d;
    }
    rebuildMask(cur);

    const int EVAL_CAP_SEARCH = 1200;
    int curScore = evaluate(cur, nums, EVAL_CAP_SEARCH);
    State best = cur;
    int bestScore = curScore;

    auto tStart = chrono::steady_clock::now();
    const double timeLimitMs = 800.0; // ~0.8s search time
    const double T0 = 5.0, T1 = 0.1;

    // Local search
    while (true) {
        auto now = chrono::steady_clock::now();
        double ms = chrono::duration<double, milli>(now - tStart).count();
        if (ms > timeLimitMs) break;
        double t = ms / timeLimitMs;
        double Temp = T0 * pow(T1 / T0, t);

        bool doSwap = (real01(rng) < 0.5);
        if (doSwap) {
            int i = cellDist(rng);
            int j = cellDist(rng);
            if (i == j) continue;
            int di = cur.g[i], dj = cur.g[j];
            if (di == dj) continue;
            swapDigits(cur, i, j);
            int cand = evaluate(cur, nums, EVAL_CAP_SEARCH);
            int delta = cand - curScore;
            bool accept = false;
            if (delta >= 0) accept = true;
            else {
                double prob = exp(double(delta) / Temp);
                if (real01(rng) < prob) accept = true;
            }
            if (accept) {
                curScore = cand;
                if (cand > bestScore) { best = cur; bestScore = cand; }
            } else {
                // revert
                swapDigits(cur, i, j);
            }
        } else {
            int p = posDist(rng);
            int nd = dig(rng);
            if (nd == cur.g[p]) continue;
            int od;
            setDigit(cur, p, nd, od);
            int cand = evaluate(cur, nums, EVAL_CAP_SEARCH);
            int delta = cand - curScore;
            bool accept = false;
            if (delta >= 0) accept = true;
            else {
                double prob = exp(double(delta) / Temp);
                if (real01(rng) < prob) accept = true;
            }
            if (accept) {
                curScore = cand;
                if (cand > bestScore) { best = cur; bestScore = cand; }
            } else {
                // revert
                int dummy;
                setDigit(cur, p, od, dummy);
            }
        }
    }

    // Optional final refinement: small greedy tweaks around best
    cur = best;
    curScore = bestScore;
    // Try a few random mutations to possibly improve slightly
    for (int iter = 0; iter < 300; ++iter) {
        int p = posDist(rng);
        int nd = dig(rng);
        if (nd == cur.g[p]) continue;
        int od;
        setDigit(cur, p, nd, od);
        int cand = evaluate(cur, nums, EVAL_CAP_SEARCH);
        if (cand >= curScore) {
            curScore = cand;
            if (cand > bestScore) { best = cur; bestScore = cand; }
        } else {
            int dummy;
            setDigit(cur, p, od, dummy);
        }
    }

    // Print best grid
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            cout << char('0' + best.g[r * C + c]);
        }
        cout << '\n';
    }

    return 0;
}