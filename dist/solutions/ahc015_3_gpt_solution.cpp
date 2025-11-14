#include <bits/stdc++.h>
using namespace std;

static const int H = 10, W = 10;

using Grid = array<array<int, W>, H>;

struct Solver {
    array<int, 100> flv;
    Grid grid{};
    int pot[4][H][W]; // pot[color][r][c], color in {1..3}

    Solver() {
        // Targets: 1 -> top-left (0,0), 2 -> top-right (0,9), 3 -> bottom-right (9,9)
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                pot[1][r][c] = r + c;                 // to (0,0)
                pot[2][r][c] = r + (W - 1 - c);       // to (0,9)
                pot[3][r][c] = (H - 1 - r) + (W - 1 - c); // to (9,9)
            }
        }
    }

    pair<int,int> pthEmpty(int p) const {
        int cnt = 0;
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                if (grid[r][c] == 0) {
                    ++cnt;
                    if (cnt == p) return {r, c};
                }
            }
        }
        // Should not happen
        return {-1, -1};
    }

    Grid tilt(const Grid &g, char dir) const {
        Grid res{};
        if (dir == 'F') {
            for (int c = 0; c < W; ++c) {
                int pos = 0;
                for (int r = 0; r < H; ++r) {
                    if (g[r][c] != 0) {
                        res[pos][c] = g[r][c];
                        ++pos;
                    }
                }
            }
        } else if (dir == 'B') {
            for (int c = 0; c < W; ++c) {
                int pos = H - 1;
                for (int r = H - 1; r >= 0; --r) {
                    if (g[r][c] != 0) {
                        res[pos][c] = g[r][c];
                        --pos;
                    }
                }
            }
        } else if (dir == 'L') {
            for (int r = 0; r < H; ++r) {
                int pos = 0;
                for (int c = 0; c < W; ++c) {
                    if (g[r][c] != 0) {
                        res[r][pos] = g[r][c];
                        ++pos;
                    }
                }
            }
        } else if (dir == 'R') {
            for (int r = 0; r < H; ++r) {
                int pos = W - 1;
                for (int c = W - 1; c >= 0; --c) {
                    if (g[r][c] != 0) {
                        res[r][pos] = g[r][c];
                        --pos;
                    }
                }
            }
        }
        return res;
    }

    long long compScore(const Grid &g) const {
        static const int dr[4] = {-1, 1, 0, 0};
        static const int dc[4] = {0, 0, -1, 1};
        bool vis[H][W] = {};
        long long sum = 0;
        int qx[H*W], qy[H*W];

        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                if (g[r][c] == 0 || vis[r][c]) continue;
                int color = g[r][c];
                int qs = 0, qe = 0;
                qx[qe] = r; qy[qe] = c; ++qe;
                vis[r][c] = true;
                int sz = 0;
                while (qs < qe) {
                    int cr = qx[qs], cc = qy[qs]; ++qs;
                    ++sz;
                    for (int k = 0; k < 4; ++k) {
                        int nr = cr + dr[k], nc = cc + dc[k];
                        if (nr < 0 || nr >= H || nc < 0 || nc >= W) continue;
                        if (vis[nr][nc]) continue;
                        if (g[nr][nc] != color) continue;
                        vis[nr][nc] = true;
                        qx[qe] = nr; qy[qe] = nc; ++qe;
                    }
                }
                sum += 1LL * sz * sz;
            }
        }
        return sum;
    }

    long long potTotal(const Grid &g) const {
        long long sum = 0;
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                int color = g[r][c];
                if (color == 0) continue;
                sum += pot[color][r][c];
            }
        }
        return sum;
    }

    long long evalScore(const Grid &g) const {
        // Weighted combination: prioritize connectivity, break ties with potential
        long long cs = compScore(g);
        long long pt = potTotal(g);
        return cs * 100 - pt;
    }

    void run() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        for (int i = 0; i < 100; ++i) cin >> flv[i];

        // Tie-break preferences per color
        // 1 -> top-left: prefer F, L
        // 2 -> top-right: prefer F, R
        // 3 -> bottom-right: prefer B, R
        array<array<char,4>, 4> pref = {};
        pref[1] = {'F','L','R','B'};
        pref[2] = {'F','R','L','B'};
        pref[3] = {'B','R','L','F'};

        for (int t = 0; t < 100; ++t) {
            int p; cin >> p;
            auto [r, c] = pthEmpty(p);
            int color = flv[t];
            grid[r][c] = color;

            char bestDir = 'F';
            long long bestScore = LLONG_MIN;

            array<char,4> dirs = {'F','B','L','R'};
            // Map for tie-break ranks
            int rankDir[4][256]; // [color][char]
            for (int col = 1; col <= 3; ++col) {
                for (int ch = 0; ch < 256; ++ch) rankDir[col][ch] = 1000;
                for (int i = 0; i < 4; ++i) rankDir[col][(int)pref[col][i]] = i;
            }

            for (char d : dirs) {
                Grid cand = tilt(grid, d);
                long long sc = evalScore(cand);
                if (sc > bestScore) {
                    bestScore = sc;
                    bestDir = d;
                } else if (sc == bestScore) {
                    // tie-break by preference of the new candy's color
                    if (rankDir[color][(int)d] < rankDir[color][(int)bestDir]) {
                        bestDir = d;
                    }
                }
            }

            cout << bestDir << '\n' << flush;
            grid = tilt(grid, bestDir);
        }
    }
};

int main() {
    Solver solver;
    solver.run();
    return 0;
}