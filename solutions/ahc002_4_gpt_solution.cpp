#include <bits/stdc++.h>
using namespace std;

struct XorShift {
    uint64_t x;
    XorShift(uint64_t seed = 88172645463393265ull) { x = seed; }
    inline uint32_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return (uint32_t)x;
    }
    inline int nextInt(int mod) { return (int)(next() % mod); }
};

struct Solver {
    static constexpr int H = 50, W = 50;
    int si, sj;
    int t[H][W];
    int p[H][W];
    int M = 0;
    vector<uint8_t> vis; // visited tiles
    string ans;
    XorShift rng;

    static constexpr int dr[4] = {-1, 1, 0, 0};
    static constexpr int dc[4] = {0, 0, -1, 1};
    static constexpr char dch[4] = {'U', 'D', 'L', 'R'};

    inline bool in(int r, int c) const { return (0 <= r && r < H && 0 <= c && c < W); }

    inline int degreeCell(int r, int c) {
        int cnt = 0;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!in(nr, nc)) continue;
            int tid = t[nr][nc];
            if (!vis[tid]) cnt++;
        }
        return cnt;
    }

    struct Move {
        short nr, nc;
        unsigned char dir;
        int tid;
        int pri;
    };

    inline int genMoves(int r, int c, int prevDir, Move out[4]) {
        int m = 0;
        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir], nc = c + dc[dir];
            if (!in(nr, nc)) continue;
            int tid2 = t[nr][nc];
            if (vis[tid2]) continue;
            int deg = 0, max2 = 0;
            for (int d2 = 0; d2 < 4; ++d2) {
                int rr = nr + dr[d2], cc = nc + dc[d2];
                if (!in(rr, cc)) continue;
                int tid3 = t[rr][cc];
                if (!vis[tid3]) {
                    deg++;
                    int val = p[rr][cc];
                    if (val > max2) max2 = val;
                }
            }
            int heur = p[nr][nc] * 100 + deg * 25 + max2 * 5;
            if (dir == prevDir) heur += 3;
            heur += (rng.nextInt(3)); // small noise
            out[m++] = Move{(short)nr, (short)nc, (unsigned char)dir, tid2, heur};
        }
        // sort by priority descending (m <= 4)
        for (int i = 0; i < m; ++i) {
            int best = i;
            for (int j = i + 1; j < m; ++j) {
                if (out[j].pri > out[best].pri) best = j;
            }
            if (best != i) swap(out[i], out[best]);
        }
        return m;
    }

    inline int KlimitForDepth(int depth) {
        if (depth >= 6) return 2;
        return 3;
    }

    int dfsScore(int r, int c, int depth, int prevDir) {
        int base = p[r][c];
        if (depth <= 0) return base;
        Move ms[4];
        int m = genMoves(r, c, prevDir, ms);
        if (m == 0) return base;
        int k = m;
        int kbound = KlimitForDepth(depth);
        if (k > kbound) k = kbound;
        int bestChild = 0;
        for (int i = 0; i < k; ++i) {
            vis[ms[i].tid] = 1;
            int sc = dfsScore(ms[i].nr, ms[i].nc, depth - 1, ms[i].dir);
            vis[ms[i].tid] = 0;
            if (sc > bestChild) bestChild = sc;
        }
        return base + bestChild;
    }

    void solve() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        cin >> si >> sj;
        int maxT = 0;
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                cin >> t[i][j];
                if (t[i][j] > maxT) maxT = t[i][j];
            }
        }
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                cin >> p[i][j];
            }
        }
        M = maxT + 1;
        vis.assign(M, 0);

        int r = si, c = sj;
        vis[t[r][c]] = 1;

        int prevDir = 255; // no previous direction

        while (true) {
            Move ms[4];
            int m = genMoves(r, c, prevDir == 255 ? -1 : prevDir, ms);
            if (m == 0) break;

            int baseDepth = 7;
            if (m <= 2) baseDepth += 2;
            else if (m == 3) baseDepth += 1;
            if (baseDepth > 12) baseDepth = 12;

            // Evaluate candidates using depth-limited DFS
            int bestIdx = 0;
            int bestScore = -1;
            int consider = m;
            // Optionally restrict consider to top 3 by heuristic
            if (consider > 3) consider = 3;
            for (int i = 0; i < consider; ++i) {
                vis[ms[i].tid] = 1;
                int sc = dfsScore(ms[i].nr, ms[i].nc, baseDepth - 1, ms[i].dir);
                vis[ms[i].tid] = 0;
                if (sc > bestScore) {
                    bestScore = sc;
                    bestIdx = i;
                }
            }

            // move
            vis[ms[bestIdx].tid] = 1;
            ans.push_back(dch[ms[bestIdx].dir]);
            r = ms[bestIdx].nr;
            c = ms[bestIdx].nc;
            prevDir = ms[bestIdx].dir;
        }

        cout << ans << '\n';
    }
};

int main() {
    Solver s;
    s.solve();
    return 0;
}