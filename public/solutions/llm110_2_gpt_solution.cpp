#include <bits/stdc++.h>
using namespace std;

static const int H = 8;
static const int W = 14;
static const int N = H * W;
using BS = bitset<128>;

struct GridSolver {
    array<BS, 10> D;       // Positions of each digit
    vector<BS> NB;         // Neighbors bitset for each cell
    int capQuick = 1500;   // quick evaluation cap
    int capFull  = 6000;   // full evaluation cap

    GridSolver() {
        NB.assign(N, BS());
        // Precompute 8-neighborhood for each cell (no stay)
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                int idx = r * W + c;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (0 <= nr && nr < H && 0 <= nc && nc < W) {
                            NB[idx].set(nr * W + nc);
                        }
                    }
                }
            }
        }
    }

    inline void buildDigitBitsets(const vector<int>& grid) {
        for (int d = 0; d < 10; ++d) D[d].reset();
        for (int i = 0; i < N; ++i) {
            int d = grid[i];
            D[d].set(i);
        }
    }

    inline bool readableNumber(int x) const {
        // Get digits of x
        int digs[12];
        int len = 0;
        while (x > 0) { digs[len++] = x % 10; x /= 10; }

        BS cur = D[digs[len - 1]];
        if (cur.none()) return false;

        for (int p = len - 2; p >= 0; --p) {
            BS neighUnion; neighUnion.reset();
            // union of neighbors of all positions in cur
            for (int i = 0; i < N; ++i) {
                if (cur.test(i)) neighUnion |= NB[i];
            }
            cur = neighUnion & D[digs[p]];
            if (cur.none()) return false;
        }
        return true;
    }

    int computeScoreCap(const vector<int>& grid, int cap, const chrono::steady_clock::time_point& deadline) {
        buildDigitBitsets(grid);
        int i = 1;
        for (;; ++i) {
            if (i > cap) return cap;
            if (chrono::steady_clock::now() > deadline) return i - 1;
            if (!readableNumber(i)) return i - 1;
        }
    }

    int computeScore(const vector<int>& grid, const chrono::steady_clock::time_point& deadline) {
        // Quick pass
        int s1 = computeScoreCap(grid, capQuick, deadline);
        if (s1 < capQuick) return s1;
        // Deeper pass
        int s2 = computeScoreCap(grid, capFull, deadline);
        return s2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    GridSolver solver;

    // Timing: use a modest time budget well under 1 minute
    auto start = chrono::steady_clock::now();
    // Use around 1.5 seconds to search (well within 1 minute limit)
    auto deadline = start + chrono::milliseconds(1500);

    // Random generator
    uint64_t seed = chrono::steady_clock::now().time_since_epoch().count();
    seed ^= (uint64_t)(uintptr_t)&seed;
    mt19937_64 rng(seed);
    uniform_int_distribution<int> digitDist(0, 9);

    vector<int> bestGrid(N, 0);
    int bestScore = -1;

    // Generate and evaluate random grids
    // Strategy: predominantly random, occasionally tweak some cells
    int attempts = 0;
    while (chrono::steady_clock::now() < deadline) {
        vector<int> grid(N);
        for (int i = 0; i < N; ++i) grid[i] = digitDist(rng);

        int s = solver.computeScore(grid, deadline);
        if (s > bestScore) {
            bestScore = s;
            bestGrid = grid;
        }
        ++attempts;

        // Small local search: mutate a few cells and test quickly
        for (int k = 0; k < 3; ++k) {
            if (chrono::steady_clock::now() >= deadline) break;
            vector<int> g2 = bestGrid;
            int changes = 6 + (rng() % 7); // 6..12 random changes
            for (int t = 0; t < changes; ++t) {
                int idx = rng() % N;
                g2[idx] = digitDist(rng);
            }
            int s2 = solver.computeScore(g2, deadline);
            if (s2 > bestScore) {
                bestScore = s2;
                bestGrid = g2;
            }
        }
    }

    // Print the best grid found
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            int idx = r * W + c;
            char ch = char('0' + bestGrid[idx]);
            cout << ch;
        }
        cout << '\n';
    }
    return 0;
}