#include <bits/stdc++.h>
using namespace std;

static const int H = 8, W = 14, N = H * W;

struct Bits {
    uint64_t lo, hi;
    Bits(uint64_t a=0, uint64_t b=0): lo(a), hi(b) {}
    inline void clear() { lo = hi = 0; }
    inline void set(int i) {
        if (i < 64) lo |= (1ULL << i);
        else hi |= (1ULL << (i - 64));
    }
    inline void reset(int i) {
        if (i < 64) lo &= ~(1ULL << i);
        else hi &= ~(1ULL << (i - 64));
    }
    inline bool any() const { return (lo | hi) != 0; }
    inline bool none() const { return (lo | hi) == 0; }
    inline Bits operator|(const Bits& o) const { return Bits(lo | o.lo, hi | o.hi); }
    inline Bits operator&(const Bits& o) const { return Bits(lo & o.lo, hi & o.hi); }
    inline Bits& operator|=(const Bits& o) { lo |= o.lo; hi |= o.hi; return *this; }
    inline Bits& operator&=(const Bits& o) { lo &= o.lo; hi &= o.hi; return *this; }
};

static inline void for_each_set_bit(const Bits& b, const function<void(int)>& f) {
    uint64_t x = b.lo;
    while (x) {
        int i = __builtin_ctzll(x);
        f(i);
        x &= x - 1;
    }
    x = b.hi;
    while (x) {
        int i = __builtin_ctzll(x);
        f(i + 64);
        x &= x - 1;
    }
}

struct Evaluator {
    vector<Bits> adj;              // adjacency mask per node
    vector<vector<uint8_t>> digs;  // digits of numbers 1..maxPre
    int maxPre;

    Evaluator(int maxPrecompute = 5000) : adj(N), digs(maxPrecompute + 1), maxPre(maxPrecompute) {
        // Build 8-neighborhood adjacency
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                int u = r * W + c;
                Bits m;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (nr < 0 || nr >= H || nc < 0 || nc >= W) continue;
                        int v = nr * W + nc;
                        m.set(v);
                    }
                }
                adj[u] = m;
            }
        }
        // Precompute digits for 1..maxPre
        for (int x = 1; x <= maxPre; ++x) {
            int t = x;
            string s = to_string(t);
            digs[x].reserve(s.size());
            for (char ch : s) digs[x].push_back(uint8_t(ch - '0'));
        }
    }

    inline bool can_read(const array<int, N>& grid, const vector<uint8_t>& dvec) const {
        // nodes_by_digit
        Bits nodesByDigit[10];
        for (int i = 0; i < N; ++i) {
            nodesByDigit[ grid[i] ].set(i);
        }
        Bits S = nodesByDigit[dvec[0]];
        if (S.none()) return false;
        for (size_t i = 1; i < dvec.size(); ++i) {
            // Union neighbors of all current positions
            Bits expand(0,0);
            // iterate set bits of S
            uint64_t x = S.lo;
            while (x) {
                int idx = __builtin_ctzll(x);
                expand |= adj[idx];
                x &= x - 1;
            }
            x = S.hi;
            while (x) {
                int idx = __builtin_ctzll(x) + 64;
                expand |= adj[idx];
                x &= x - 1;
            }
            S = expand & nodesByDigit[dvec[i]];
            if (S.none()) return false;
        }
        return true;
    }

    int score(const array<int, N>& grid, int limit) const {
        int lim = min(limit, maxPre);
        // Precompute nodes_by_digit once for speed
        Bits nodesByDigit[10];
        for (int d = 0; d < 10; ++d) nodesByDigit[d].clear();
        for (int i = 0; i < N; ++i) nodesByDigit[ grid[i] ].set(i);

        for (int x = 1; x <= lim; ++x) {
            const auto& dvec = digs[x];
            Bits S = nodesByDigit[dvec[0]];
            if (S.none()) return x - 1;
            for (size_t i = 1; i < dvec.size(); ++i) {
                Bits expand(0,0);
                uint64_t a = S.lo;
                while (a) {
                    int idx = __builtin_ctzll(a);
                    expand |= adj[idx];
                    a &= a - 1;
                }
                a = S.hi;
                while (a) {
                    int idx = __builtin_ctzll(a) + 64;
                    expand |= adj[idx];
                    a &= a - 1;
                }
                S = expand & nodesByDigit[dvec[i]];
                if (S.none()) return x - 1;
            }
        }
        return lim;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // RNG
    uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);

    Evaluator E(5000);

    // Build a snake Hamiltonian path order
    vector<int> order;
    order.reserve(N);
    for (int r = 0; r < H; ++r) {
        if (r % 2 == 0) {
            for (int c = 0; c < W; ++c) order.push_back(r * W + c);
        } else {
            for (int c = W - 1; c >= 0; --c) order.push_back(r * W + c);
        }
    }

    // Create initial grid: place concatenation of 1..K along the path where total digits <= N
    string S;
    for (int k = 1; ; ++k) {
        string t = to_string(k);
        if ((int)S.size() + (int)t.size() > N) break;
        S += t;
    }
    array<int, N> grid{};
    // Initialize with a periodic pattern to ensure all digits present
    for (int i = 0; i < N; ++i) grid[i] = i % 10;
    // Overlay the path with the string S
    for (int i = 0; i < (int)S.size(); ++i) {
        grid[order[i]] = S[i] - '0';
    }
    // Fill remaining cells randomly to diversify
    for (int i = (int)S.size(); i < N; ++i) {
        grid[order[i]] = int(rng() % 10);
    }

    // Hill climbing / simulated annealing
    array<int, N> cur = grid, best = grid;
    int bestScore = E.score(cur, 1000);
    int curScore = bestScore;

    auto start = chrono::high_resolution_clock::now();
    const long long timeBudgetMs = 1800; // keep it small and safe
    uniform_int_distribution<int> posDist(0, N - 1);
    uniform_int_distribution<int> digitDist(0, 9);
    uniform_real_distribution<double> real01(0.0, 1.0);

    int evalLimit = 1000;

    while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < timeBudgetMs) {
        bool doSwap = (rng() & 3ULL) == 0ULL; // 25% swap, 75% single change
        if (doSwap) {
            int i = posDist(rng), j = posDist(rng);
            if (i == j) continue;
            if (cur[i] == cur[j]) continue;
            swap(cur[i], cur[j]);
            int limit = min(evalLimit, bestScore + 200);
            int newScore = E.score(cur, limit);
            double elapsed = chrono::duration<double>(chrono::high_resolution_clock::now() - start).count();
            double progress = min(1.0, elapsed / (timeBudgetMs / 1000.0));
            double temp = 0.5 * (1.0 - progress) + 0.02 * progress;
            bool accept = (newScore >= curScore) || (real01(rng) < exp(double(newScore - curScore) / max(1e-9, temp)));
            if (accept) {
                curScore = newScore;
                if (newScore > bestScore) {
                    bestScore = newScore;
                    best = cur;
                    if (bestScore >= evalLimit - 5 && evalLimit < 5000) {
                        evalLimit = min(5000, evalLimit * 2);
                        // re-evaluate best with higher limit
                        bestScore = E.score(best, evalLimit);
                        curScore = E.score(cur, evalLimit);
                    }
                }
            } else {
                swap(cur[i], cur[j]); // revert
            }
        } else {
            int i = posDist(rng);
            int old = cur[i];
            int nd = digitDist(rng);
            if (nd == old) continue;
            cur[i] = nd;
            int limit = min(evalLimit, bestScore + 200);
            int newScore = E.score(cur, limit);
            double elapsed = chrono::duration<double>(chrono::high_resolution_clock::now() - start).count();
            double progress = min(1.0, elapsed / (timeBudgetMs / 1000.0));
            double temp = 0.5 * (1.0 - progress) + 0.02 * progress;
            bool accept = (newScore >= curScore) || (real01(rng) < exp(double(newScore - curScore) / max(1e-9, temp)));
            if (accept) {
                curScore = newScore;
                if (newScore > bestScore) {
                    bestScore = newScore;
                    best = cur;
                    if (bestScore >= evalLimit - 5 && evalLimit < 5000) {
                        evalLimit = min(5000, evalLimit * 2);
                        bestScore = E.score(best, evalLimit);
                        curScore = E.score(cur, evalLimit);
                    }
                }
            } else {
                cur[i] = old; // revert
            }
        }
    }

    // Output the best grid
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            cout << char('0' + best[r * W + c]);
        }
        cout << '\n';
    }
    return 0;
}