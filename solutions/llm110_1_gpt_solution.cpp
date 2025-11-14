#include <bits/stdc++.h>
using namespace std;

static const int R = 8;
static const int C = 14;
static const int V = R * C;

struct Bits {
    uint64_t lo, hi;
    Bits(uint64_t a=0, uint64_t b=0): lo(a), hi(b) {}
    inline bool zero() const { return lo==0 && hi==0; }
    inline void set(int idx) {
        if (idx < 64) lo |= (1ull << idx);
        else hi |= (1ull << (idx - 64));
    }
    inline void reset() { lo = hi = 0; }
};

inline Bits b_or(const Bits &a, const Bits &b){ return Bits(a.lo | b.lo, a.hi | b.hi); }
inline Bits b_and(const Bits &a, const Bits &b){ return Bits(a.lo & b.lo, a.hi & b.hi); }

static Bits neigh[V];

inline bool inside(int r, int c){ return r>=0 && r<R && c>=0 && c<C; }

static inline int idx_of(int r, int c){ return r*C + c; }

void build_neighbors() {
    for (int r=0; r<R; ++r) {
        for (int c=0; c<C; ++c) {
            int id = idx_of(r,c);
            Bits b;
            for (int dr=-1; dr<=1; ++dr) {
                for (int dc=-1; dc<=1; ++dc) {
                    if (dr==0 && dc==0) continue;
                    int rr = r + dr, cc = c + dc;
                    if (inside(rr,cc)) {
                        int j = idx_of(rr,cc);
                        b.set(j);
                    }
                }
            }
            neigh[id] = b;
        }
    }
}

inline Bits digit_bits(const array<int,V> &grid, int d) {
    Bits b;
    for (int i=0;i<V;i++){
        if (grid[i]==d) b.set(i);
    }
    return b;
}

bool is_readable(const string &s, const Bits digitset[10]) {
    Bits cur = digitset[s[0]-'0'];
    if (cur.zero()) return false;

    for (size_t i=1; i<s.size(); ++i) {
        Bits reach;
        // iterate set bits of cur.lo
        uint64_t t = cur.lo;
        while (t) {
            int u = __builtin_ctzll(t);
            reach = b_or(reach, neigh[u]);
            t &= t - 1;
        }
        // iterate set bits of cur.hi
        t = cur.hi;
        while (t) {
            int u = __builtin_ctzll(t);
            reach = b_or(reach, neigh[64 + u]);
            t &= t - 1;
        }
        cur = b_and(reach, digitset[s[i]-'0']);
        if (cur.zero()) return false;
    }
    return true;
}

int score_grid(const array<int,V> &grid, int limitN) {
    Bits digitset[10];
    for (int d=0; d<10; ++d) digitset[d] = digit_bits(grid, d);
    for (int n=1; n<=limitN; ++n) {
        string s = to_string(n);
        if (!is_readable(s, digitset)) return n-1;
    }
    return limitN;
}

array<int,V> random_grid(mt19937_64 &rng) {
    uniform_int_distribution<int> D(0,9);
    array<int,V> g;
    for (int i=0;i<V;i++) g[i] = D(rng);
    // Ensure all digits 0..9 appear at least once
    array<int,10> pos;
    iota(pos.begin(), pos.end(), 0);
    vector<int> idx(V);
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), rng);
    for (int d=0; d<10 && d<(int)idx.size(); ++d) {
        g[idx[d]] = d;
    }
    return g;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    build_neighbors();

    uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    seed ^= (uint64_t)(uintptr_t)&seed;
    seed ^= 0x9e3779b97f4a7c15ull;
    mt19937_64 rng(seed);

    const double timeBudgetSec = 0.8; // keep well under 1 minute; fast search
    auto start = chrono::steady_clock::now();

    array<int,V> bestGrid{};
    int bestScore = -1;

    // Try many random grids, keep best.
    int limitEval = 500; // cap for evaluation per candidate
    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - start).count();
        if (elapsed > timeBudgetSec) break;

        auto g = random_grid(rng);
        int sc = score_grid(g, limitEval);
        if (sc > bestScore) {
            bestScore = sc;
            bestGrid = g;
        }
    }

    // Tiny local improvement phase on best grid
    {
        auto g = bestGrid;
        int curScore = bestScore;
        uniform_int_distribution<int> posDist(0, V-1);
        uniform_int_distribution<int> digDist(0, 9);

        while (true) {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(now - start).count();
            if (elapsed > timeBudgetSec) break;

            int idx = posDist(rng);
            int old = g[idx];
            int nd = digDist(rng);
            if (nd == old) continue;
            g[idx] = nd;
            int sc = score_grid(g, max(curScore+5, 120)); // quick check
            if (sc >= curScore) {
                curScore = sc;
                bestGrid = g;
                bestScore = sc;
            } else {
                g[idx] = old;
            }
        }
    }

    // Print the best grid found
    for (int r=0;r<R;r++){
        for (int c=0;c<C;c++){
            cout << char('0' + bestGrid[r*C + c]);
        }
        cout << '\n';
    }
    return 0;
}