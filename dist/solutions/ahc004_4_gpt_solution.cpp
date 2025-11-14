#include <bits/stdc++.h>
using namespace std;

static inline uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

struct Solver {
    int N, M;
    vector<string> S;

    mt19937_64 rng;
    vector<vector<char>> best_grid;
    int best_c = -1;

    Solver(int N, int M, const vector<string>& S, uint64_t seed) : N(N), M(M), S(S), rng(seed) {}

    inline int score_match(const vector<vector<char>>& grid, const string& s) {
        int k = (int)s.size();
        int c = 0;
        // Check rows
        for (int i = 0; i < N; ++i) {
            for (int start = 0; start < N; ++start) {
                bool ok = true;
                int col = start;
                for (int p = 0; p < k; ++p) {
                    if (grid[i][col] != s[p]) { ok = false; break; }
                    col++; if (col == N) col = 0;
                }
                if (ok) return 1;
            }
        }
        // Check cols
        for (int j = 0; j < N; ++j) {
            for (int start = 0; start < N; ++start) {
                bool ok = true;
                int row = start;
                for (int p = 0; p < k; ++p) {
                    if (grid[row][j] != s[p]) { ok = false; break; }
                    row++; if (row == N) row = 0;
                }
                if (ok) return 1;
            }
        }
        return 0;
    }

    inline int compute_c(const vector<vector<char>>& grid) {
        int cnt = 0;
        for (int i = 0; i < M; ++i) {
            cnt += score_match(grid, S[i]);
        }
        return cnt;
    }

    vector<vector<char>> build_grid_once(uint64_t local_seed, bool desc_len) {
        mt19937_64 lrng(local_seed);
        vector<vector<char>> grid(N, vector<char>(N, 0));
        vector<int> rowCnt(N, 0), colCnt(N, 0);

        vector<int> idx(M);
        iota(idx.begin(), idx.end(), 0);
        shuffle(idx.begin(), idx.end(), lrng);
        stable_sort(idx.begin(), idx.end(), [&](int a, int b){
            if (desc_len) return (int)S[a].size() > (int)S[b].size();
            else return (int)S[a].size() < (int)S[b].size();
        });

        for (int id : idx) {
            const string& s = S[id];
            int k = (int)s.size();
            long long best_score = LLONG_MIN;
            int bi = 0, bj = 0, bdir = 0; // dir 0=H, 1=V

            // Horizontal
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    int matched = 0;
                    int required = 0;
                    bool conflict = false;
                    int r = i, c = j;
                    for (int p = 0; p < k; ++p) {
                        char g = grid[r][c];
                        if (g != 0) {
                            if (g == s[p]) matched++;
                            else { conflict = true; break; }
                        } else required++;
                        c++; if (c == N) c = 0;
                    }
                    if (conflict) continue;
                    long long sc = (long long)matched * 1000 - (long long)required + rowCnt[i] + (long long)(lrng() % 7);
                    if (sc > best_score) {
                        best_score = sc; bi = i; bj = j; bdir = 0;
                    }
                }
            }
            // Vertical
            for (int j = 0; j < N; ++j) {
                for (int i = 0; i < N; ++i) {
                    int matched = 0;
                    int required = 0;
                    bool conflict = false;
                    int r = i, c = j;
                    for (int p = 0; p < k; ++p) {
                        char g = grid[r][c];
                        if (g != 0) {
                            if (g == s[p]) matched++;
                            else { conflict = true; break; }
                        } else required++;
                        r++; if (r == N) r = 0;
                    }
                    if (conflict) continue;
                    long long sc = (long long)matched * 1000 - (long long)required + colCnt[j] + (long long)(lrng() % 7);
                    if (sc > best_score) {
                        best_score = sc; bi = i; bj = j; bdir = 1;
                    }
                }
            }

            // Commit
            if (bdir == 0) {
                int r = bi, c = bj;
                for (int p = 0; p < k; ++p) {
                    if (grid[r][c] == 0) {
                        grid[r][c] = s[p];
                        rowCnt[r]++; colCnt[c]++;
                    }
                    c++; if (c == N) c = 0;
                }
            } else {
                int r = bi, c = bj;
                for (int p = 0; p < k; ++p) {
                    if (grid[r][c] == 0) {
                        grid[r][c] = s[p];
                        rowCnt[r]++; colCnt[c]++;
                    }
                    r++; if (r == N) r = 0;
                }
            }
        }

        // Fill remaining unknowns randomly to letters A..H
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (grid[i][j] == 0) {
                    grid[i][j] = char('A' + (lrng() % 8));
                }
            }
        }
        return grid;
    }

    void run() {
        auto start = chrono::steady_clock::now();
        const double TIME_LIMIT = 1.85; // seconds
        int tries = 0;

        // Do a few deterministic variations plus random ones within time
        while (true) {
            double elapsed = chrono::duration<double>(chrono::steady_clock::now() - start).count();
            if (elapsed > TIME_LIMIT) break;

            bool desc = (tries % 2 == 0);
            uint64_t local_seed = splitmix64(rng());
            auto grid = build_grid_once(local_seed, desc);
            int c = compute_c(grid);
            if (c > best_c) {
                best_c = c;
                best_grid = grid;
            }
            tries++;
            if (tries >= 10) break; // safety cap
        }

        if (best_c < 0) { // fallback
            best_grid.assign(N, vector<char>(N, 'A'));
        }
    }

    void output() {
        for (int i = 0; i < N; ++i) {
            string line;
            line.reserve(N);
            for (int j = 0; j < N; ++j) line.push_back(best_grid[i][j]);
            cout << line << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<string> S(M);
    for (int i = 0; i < M; ++i) cin >> S[i];

    uint64_t seed = 0xC0FFEE1234567890ULL ^ (uint64_t)N * 1315423911ULL ^ (uint64_t)M * 2654435761ULL;
    for (int i = 0; i < M; ++i) {
        uint64_t h = 0;
        for (char ch : S[i]) {
            h = (h * 1000003ULL) ^ (unsigned char)ch ^ 0x9e3779b97f4a7c15ULL;
        }
        seed ^= splitmix64(h + i * 0x9e3779b97f4a7c15ULL);
    }

    Solver solver(N, M, S, seed);
    solver.run();
    solver.output();
    return 0;
}