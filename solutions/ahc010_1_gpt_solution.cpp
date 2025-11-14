#include <bits/stdc++.h>
using namespace std;

static const int H = 30;
static const int W = 30;
static const int NSTATE = H * W * 4;

// Directions: 0=left,1=up,2=right,3=down
static const int di[4] = {0, -1, 0, 1};
static const int dj[4] = {-1, 0, 1, 0};

// to[t][d] from problem statement
static const int TO_BASE[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1},
};

inline int idx(int i, int j, int d) {
    return ((i * W) + j) * 4 + d;
}

struct Timer {
    chrono::high_resolution_clock::time_point st;
    Timer() { st = chrono::high_resolution_clock::now(); }
    double elapsedMs() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double, std::milli>(now - st).count();
    }
};

struct XorShift {
    uint64_t x=88172645463393265ull;
    inline uint32_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return (uint32_t)x;
    }
    inline uint32_t next(uint32_t mod) { return next() % mod; }
    inline double nextDouble() { return (next() + 0.5) * (1.0 / 4294967296.0); }
} rng;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read input
    vector<string> s(H);
    for (int i = 0; i < H; i++) {
        cin >> s[i];
    }
    static int base[H][W];
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            base[i][j] = s[i][j] - '0';
        }
    }

    // Precompute masks per tile type (connectivity on sides)
    int connectMask[8];
    for (int t = 0; t < 8; t++) {
        int m = 0;
        for (int d = 0; d < 4; d++) {
            if (TO_BASE[t][d] != -1) m |= (1 << d);
        }
        connectMask[t] = m;
    }

    auto rotate_type = [](int t, int r) {
        r &= 3;
        if (t >= 0 && t <= 3) {
            return (t + r) & 3;
        } else if (t == 4 || t == 5) {
            // toggle on parity
            int v = (t - 4) ^ (r & 1);
            return 4 + v;
        } else {
            // 6 or 7
            int v = (t - 6) ^ (r & 1);
            return 6 + v;
        }
    };

    // Precompute maskFor[t][r]
    int maskFor[8][4];
    for (int t = 0; t < 8; t++) {
        for (int r = 0; r < 4; r++) {
            int tt = rotate_type(t, r);
            maskFor[t][r] = connectMask[tt];
        }
    }

    // Current rotations
    static int rot[H][W];
    for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) rot[i][j] = 0;

    // Current oriented type and mask
    static int curType[H][W];
    static int curMask[H][W];
    for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) {
        curType[i][j] = rotate_type(base[i][j], rot[i][j]);
        curMask[i][j] = connectMask[curType[i][j]];
    }

    auto local_match = [&](int i, int j, int mm) -> int {
        int cnt = 0;
        if (j > 0) {
            if ((mm & (1 << 0)) && (curMask[i][j - 1] & (1 << 2))) cnt++;
        }
        if (i > 0) {
            if ((mm & (1 << 1)) && (curMask[i - 1][j] & (1 << 3))) cnt++;
        }
        if (j + 1 < W) {
            if ((mm & (1 << 2)) && (curMask[i][j + 1] & (1 << 0))) cnt++;
        }
        if (i + 1 < H) {
            if ((mm & (1 << 3)) && (curMask[i + 1][j] & (1 << 1))) cnt++;
        }
        return cnt;
    };

    auto compute_total_matches = [&](){
        int res = 0;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j + 1 < W; j++) {
                if ((curMask[i][j] & (1 << 2)) && (curMask[i][j + 1] & (1 << 0))) res++;
            }
        }
        for (int i = 0; i + 1 < H; i++) {
            for (int j = 0; j < W; j++) {
                if ((curMask[i][j] & (1 << 3)) && (curMask[i + 1][j] & (1 << 1))) res++;
            }
        }
        return res;
    };

    int totalMatches = compute_total_matches();

    Timer timer;
    const double TIME_LIMIT_MS = 1950.0;

    // Greedy coordinate descent (exclude type 4/5 because mask unaffected)
    for (int pass = 0; pass < 3; pass++) {
        bool changed = false;
        for (int i = 0; i < H; i++) {
            if (timer.elapsedMs() > TIME_LIMIT_MS * 0.15) break;
            for (int j = 0; j < W; j++) {
                int t = base[i][j];
                if (t == 4 || t == 5) continue;
                int bestR = rot[i][j];
                int bestLocal = local_match(i, j, curMask[i][j]);
                for (int r = 0; r < 4; r++) {
                    if (r == rot[i][j]) continue;
                    int mm = maskFor[t][r];
                    int v = local_match(i, j, mm);
                    if (v > bestLocal) {
                        bestLocal = v;
                        bestR = r;
                    }
                }
                if (bestR != rot[i][j]) {
                    int oldLocal = local_match(i, j, curMask[i][j]);
                    rot[i][j] = bestR;
                    curType[i][j] = rotate_type(t, rot[i][j]);
                    int mmNew = connectMask[curType[i][j]];
                    curMask[i][j] = mmNew;
                    int newLocal = local_match(i, j, curMask[i][j]);
                    totalMatches += (newLocal - oldLocal);
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }

    // Simulated annealing on tiles except 4/5
    double T0 = 1.5, T1 = 0.01;
    int iter = 0;
    while (true) {
        double elapsed = timer.elapsedMs();
        if (elapsed > TIME_LIMIT_MS * 0.78) break;
        iter++;
        int i = rng.next(H);
        int j = rng.next(W);
        int t = base[i][j];
        if (t == 4 || t == 5) continue;
        int oldR = rot[i][j];
        int newR;
        if (t == 6 || t == 7) {
            newR = oldR ^ 1;
        } else {
            int delta = 1 + rng.next(3);
            newR = (oldR + delta) & 3;
        }
        if (newR == oldR) continue;
        int oldLocal = local_match(i, j, curMask[i][j]);
        int mmNew = maskFor[t][newR];
        int newLocal = local_match(i, j, mmNew);
        int diff = newLocal - oldLocal;

        double progress = elapsed / (TIME_LIMIT_MS * 0.78);
        double T = T0 + (T1 - T0) * progress;
        if (diff >= 0 || rng.nextDouble() < exp(diff / T)) {
            rot[i][j] = newR;
            curType[i][j] = rotate_type(t, rot[i][j]);
            curMask[i][j] = mmNew;
            totalMatches += diff;
        }
    }

    // Prepare 'next state' array for loop computations
    static int nextState[NSTATE];
    auto rebuildNextAll = [&](){
        for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) {
            // ensure curType reflects rot
            curType[i][j] = rotate_type(base[i][j], rot[i][j]);
        }
        for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) for (int d = 0; d < 4; d++) {
            int t = curType[i][j];
            int d2 = TO_BASE[t][d];
            int s = idx(i, j, d);
            if (d2 == -1) {
                nextState[s] = -1;
            } else {
                int ni = i + di[d2];
                int nj = j + dj[d2];
                if (ni < 0 || ni >= H || nj < 0 || nj >= W) {
                    nextState[s] = -1;
                } else {
                    int nd = (d2 + 2) & 3;
                    nextState[s] = idx(ni, nj, nd);
                }
            }
        }
    };
    rebuildNextAll();

    auto computeTop2Product = [&](){
        static unsigned char visited[NSTATE];
        static int seenMark[NSTATE];
        static int posIdx[NSTATE];
        memset(visited, 0, sizeof(visited));
        int curMark = 1;
        int top1 = 0, top2 = 0;

        for (int s0 = 0; s0 < NSTATE; s0++) {
            if (visited[s0]) continue;

            int s = s0;
            int len = 0;
            int cycleLen = 0;

            // path traversal with stamping
            vector<int> path;
            path.reserve(64);
            while (true) {
                if (s == -1 || visited[s]) {
                    break;
                }
                if (seenMark[s] == curMark) {
                    // found cycle
                    cycleLen = (int)path.size() - posIdx[s];
                    break;
                }
                seenMark[s] = curMark;
                posIdx[s] = (int)path.size();
                path.push_back(s);
                s = nextState[s];
            }
            // mark visited
            for (int v : path) visited[v] = 1;

            if (cycleLen > 0) {
                // update top 2
                if (cycleLen >= top1) {
                    top2 = top1;
                    top1 = cycleLen;
                } else if (cycleLen > top2) {
                    top2 = cycleLen;
                }
            }
            curMark++;
        }
        long long prod = 1LL * top1 * top2;
        return prod;
    };

    // Try flipping tiles of type 4/5 to improve L1*L2
    auto updateNextForTile = [&](int i, int j){
        int t = curType[i][j];
        for (int d = 0; d < 4; d++) {
            int d2 = TO_BASE[t][d];
            int s = idx(i, j, d);
            if (d2 == -1) {
                nextState[s] = -1;
            } else {
                int ni = i + di[d2];
                int nj = j + dj[d2];
                if (ni < 0 || ni >= H || nj < 0 || nj >= W) {
                    nextState[s] = -1;
                } else {
                    int nd = (d2 + 2) & 3;
                    nextState[s] = idx(ni, nj, nd);
                }
            }
        }
    };

    long long bestProd = computeTop2Product();

    // Create list of 4/5 positions
    vector<pair<int,int>> fourFive;
    fourFive.reserve(H*W/2);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (base[i][j] == 4 || base[i][j] == 5) fourFive.emplace_back(i, j);

    // Optionally shuffle for diversity
    for (int k = 0; k < (int)fourFive.size(); k++) {
        int t = rng.next((uint32_t)fourFive.size());
        swap(fourFive[k], fourFive[t]);
    }

    // Greedy single pass to flip 4/5 tiles to improve product
    for (auto [i, j] : fourFive) {
        if (timer.elapsedMs() > TIME_LIMIT_MS * 0.97) break;

        // Flip rotation parity
        rot[i][j] ^= 1;
        curType[i][j] = rotate_type(base[i][j], rot[i][j]);
        updateNextForTile(i, j);

        long long prod = computeTop2Product();
        if (prod >= bestProd) {
            bestProd = prod;
            // keep
        } else {
            // revert
            rot[i][j] ^= 1;
            curType[i][j] = rotate_type(base[i][j], rot[i][j]);
            updateNextForTile(i, j);
        }
    }

    // Output rotations as 900 characters
    string out;
    out.reserve(H * W);
    for (int i = 0; i < H; i++) for (int j = 0; j < W; j++) {
        out.push_back(char('0' + (rot[i][j] & 3)));
    }
    cout << out << '\n';
    return 0;
}