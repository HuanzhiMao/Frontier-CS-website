#include <bits/stdc++.h>
using namespace std;

struct BestPlacement {
    int d = -1; // 0: horizontal, 1: vertical
    int i = -1;
    int j = -1;
    int cost = INT_MAX;
    bool valid = false;
};

static inline BestPlacement findBest(const vector<char>& grid, int N, const string& s, int allowed, std::mt19937_64& rng) {
    const int k = (int)s.size();
    BestPlacement best;
    int bestCost = INT_MAX;
    // To reduce branch mispredictions, compute loop bounds locally
    for (int d = 0; d < 2; ++d) {
        if (bestCost == 0) break; // cannot improve further
        if (d == 0) {
            // horizontal
            for (int i = 0; i < N; ++i) {
                if (bestCost == 0) break;
                for (int j = 0; j < N; ++j) {
                    int cost = 0;
                    bool conflict = false;
                    int cc = j;
                    for (int p = 0; p < k; ++p) {
                        int idx = i * N + cc;
                        char g = grid[idx];
                        char ch = s[p];
                        if (g != '.' && g != ch) { conflict = true; break; }
                        if (g == '.') ++cost;
                        ++cc;
                        if (cc >= N) cc -= N;
                        if (cost >= bestCost) { // prune
                            // still need to advance cc to keep loop variables consistent? We don't use cc after break, so fine.
                            break;
                        }
                    }
                    if (!conflict) {
                        if (cost < bestCost) {
                            bestCost = cost;
                            best = {0, i, j, cost, true};
                            if (bestCost == 0) break;
                        } else if (cost == bestCost) {
                            // random tie-break to diversify
                            if ((rng() & 1) == 0) {
                                best = {0, i, j, cost, true};
                            }
                        }
                    }
                }
            }
        } else {
            // vertical
            for (int j = 0; j < N; ++j) {
                if (bestCost == 0) break;
                for (int i = 0; i < N; ++i) {
                    int cost = 0;
                    bool conflict = false;
                    int rr = i;
                    for (int p = 0; p < k; ++p) {
                        int idx = rr * N + j;
                        char g = grid[idx];
                        char ch = s[p];
                        if (g != '.' && g != ch) { conflict = true; break; }
                        if (g == '.') ++cost;
                        ++rr;
                        if (rr >= N) rr -= N;
                        if (cost >= bestCost) { // prune
                            break;
                        }
                    }
                    if (!conflict) {
                        if (cost < bestCost) {
                            bestCost = cost;
                            best = {1, i, j, cost, true};
                            if (bestCost == 0) break;
                        } else if (cost == bestCost) {
                            if ((rng() & 1) == 0) {
                                best = {1, i, j, cost, true};
                            }
                        }
                    }
                }
            }
        }
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) {
        return 0;
    }
    vector<string> s(M);
    for (int i = 0; i < M; ++i) cin >> s[i];

    // Seed RNG deterministically from input
    uint64_t seed = 146527 + N * 10007 + M * 998244353ULL;
    for (int i = 0; i < M; ++i) {
        for (char c : s[i]) {
            seed ^= (seed << 7) ^ (seed >> 3) ^ (uint64_t)(unsigned char)c * 1000003ULL + i * 911382323ULL;
        }
        seed ^= (uint64_t)s[i].size() * 1234567ULL + (uint64_t)(i + 1) * 19260817ULL;
    }
    std::mt19937_64 rng(seed);

    const int NN = N * N;
    vector<char> grid(NN, '.');
    vector<char> best_grid = grid;
    vector<char> tmp_grid;

    vector<bool> placed(M, false);
    int maxLen = 0;
    for (auto &t : s) maxLen = max(maxLen, (int)t.size());

    auto start = chrono::steady_clock::now();
    const double TIME_LIMIT = 1.95; // seconds
    auto timeNow = [&](){
        return chrono::duration<double>(chrono::steady_clock::now() - start).count();
    };

    // Multi-stage greedy: increase allowed new assignments gradually
    for (int allowed = 0; allowed <= maxLen; ++allowed) {
        bool progressed_outer = true;
        int safeguard_iter = 0;
        while (progressed_outer) {
            if (timeNow() > TIME_LIMIT * 0.8) break; // reserve time for fill
            progressed_outer = false;
            ++safeguard_iter;
            if (safeguard_iter > 5) break; // avoid too many inner loops per allowed
            for (int i = 0; i < M; ++i) {
                if (placed[i]) continue;
                if (timeNow() > TIME_LIMIT * 0.8) break;
                BestPlacement bp = findBest(grid, N, s[i], allowed, rng);
                if (bp.valid && bp.cost <= allowed) {
                    // Apply placement
                    const string &st = s[i];
                    int k = (int)st.size();
                    if (bp.d == 0) {
                        int r = bp.i;
                        int c = bp.j;
                        for (int p = 0; p < k; ++p) {
                            int cc = c + p; if (cc >= N) cc -= N;
                            int idx = r * N + cc;
                            if (grid[idx] == '.') grid[idx] = st[p];
                        }
                    } else {
                        int r = bp.i;
                        int c = bp.j;
                        for (int p = 0; p < k; ++p) {
                            int rr = r + p; if (rr >= N) rr -= N;
                            int idx = rr * N + c;
                            if (grid[idx] == '.') grid[idx] = st[p];
                        }
                    }
                    placed[i] = true;
                    progressed_outer = true;
                }
            }
        }
        if (timeNow() > TIME_LIMIT * 0.8) break;
    }

    // Fill remaining '.' with random letters to potentially match more strings
    for (int idx = 0; idx < NN; ++idx) {
        if (grid[idx] == '.') {
            grid[idx] = char('A' + (rng() & 7ULL));
        }
    }

    // Output the grid
    for (int i = 0; i < N; ++i) {
        cout.write(&grid[i * N], N);
        cout << '\n';
    }

    return 0;
}