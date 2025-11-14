#include <bits/stdc++.h>
using namespace std;

struct XorShift {
    using ull = unsigned long long;
    ull x;
    XorShift(ull seed = 88172645463393265ull) { x = seed; }
    inline ull next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int randInt(int l, int r) { // inclusive
        return l + (int)(next() % (unsigned)(r - l + 1));
    }
    inline double randDouble() {
        return (next() >> 11) * (1.0 / (1ull << 53));
    }
};

struct Timer {
    chrono::high_resolution_clock::time_point st;
    double limit;
    Timer(double limit_sec) : st(chrono::high_resolution_clock::now()), limit(limit_sec) {}
    inline double elapsed() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double>(now - st).count();
    }
    inline double progress() const {
        double e = elapsed();
        if (e < 0) e = 0;
        if (e > limit) e = limit;
        return e / limit;
    }
    inline bool over() const { return elapsed() >= limit; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    vector<string> rows(N);
    for (int i = 0; i < N; i++) {
        string s;
        if (!(cin >> s)) return 0;
        rows[i] = s;
    }

    // Parse input
    vector<int> orig(N*N);
    for (int i = 0; i < N; i++) {
        if ((int)rows[i].size() < N) {
            // Fallback if spaces are used (not expected)
            int col = 0;
            for (char c : rows[i]) {
                if ('0' <= c && c <= '7') {
                    if (col < N) {
                        orig[i*N + col] = c - '0';
                        col++;
                    }
                }
            }
            for (; col < N; col++) {
                char c;
                cin >> c;
                orig[i*N + col] = c - '0';
            }
        } else {
            for (int j = 0; j < N; j++) {
                orig[i*N + j] = rows[i][j] - '0';
            }
        }
    }

    // to mapping as provided in the problem
    static const int to_map[8][4] = {
        {1, 0, -1, -1},
        {3, -1, -1, 0},
        {-1, -1, 3, 2},
        {-1, 2, 1, -1},
        {1, 0, 3, 2},
        {3, 2, 1, 0},
        {2, -1, 0, -1},
        {-1, 3, -1, 1},
    };
    static const int di[4] = {0, -1, 0, 1};
    static const int dj[4] = {-1, 0, 1, 0};
    static const int rot90[8] = {1,2,3,0,5,4,7,6};

    // Precompute rotation results
    int rotPow[8][4];
    for (int t = 0; t < 8; t++) {
        int cur = t;
        for (int r = 0; r < 4; r++) {
            rotPow[t][r] = cur;
            cur = rot90[cur];
        }
    }

    // Current rotations and types
    vector<int> rot(N*N, 0);
    vector<int> curType(N*N);
    for (int i = 0; i < N*N; i++) curType[i] = orig[i];

    auto inb = [&](int i, int j){ return (0 <= i && i < N && 0 <= j && j < N); };

    // Edge matching count
    auto edge_match_count = [&](const vector<int>& types) {
        int cnt = 0;
        // horizontal edges
        for (int i = 0; i < N; i++) {
            for (int j = 0; j + 1 < N; j++) {
                int a = types[i*N + j];
                int b = types[i*N + (j+1)];
                if (to_map[a][2] != -1 && to_map[b][0] != -1) cnt++;
            }
        }
        // vertical edges
        for (int i = 0; i + 1 < N; i++) {
            for (int j = 0; j < N; j++) {
                int a = types[i*N + j];
                int b = types[(i+1)*N + j];
                if (to_map[a][3] != -1 && to_map[b][1] != -1) cnt++;
            }
        }
        return cnt;
    };

    auto edge_pair_contrib = [&](const vector<int>& types, int i, int j, int d)->int {
        // d: 0 left, 1 up, 2 right, 3 down (edge relative to (i,j))
        int ni = i + di[d], nj = j + dj[d];
        if (!inb(ni, nj)) return 0;
        int a = types[i*N + j];
        int b = types[ni*N + nj];
        int sd = d;
        int od = (d + 2) & 3;
        return (to_map[a][sd] != -1 && to_map[b][od] != -1) ? 1 : 0;
    };

    // Initial greedy improving edge matches
    // We set rotations to maximize local edge matches
    {
        XorShift rng(1234567);
        vector<int> order(N*N);
        iota(order.begin(), order.end(), 0);
        int passes = 3; // a few passes
        for (int it = 0; it < passes; it++) {
            // shuffle order
            for (int k = N*N-1; k > 0; k--) {
                int p = rng.randInt(0, k);
                swap(order[k], order[p]);
            }
            for (int idx : order) {
                int i = idx / N, j = idx % N;
                int bestR = rot[idx];
                int bestSc = -1;
                // For types 4 and 5, open sides are always 4 regardless of rotation,
                // but we still evaluate for uniformity.
                for (int r = 0; r < 4; r++) {
                    int t = rotPow[orig[idx]][r];
                    int sc = 0;
                    // check 4 edges
                    // left
                    if (j-1 >= 0) {
                        int nb = curType[i*N + (j-1)];
                        if (to_map[t][0] != -1 && to_map[nb][2] != -1) sc++;
                    }
                    // up
                    if (i-1 >= 0) {
                        int nb = curType[(i-1)*N + j];
                        if (to_map[t][1] != -1 && to_map[nb][3] != -1) sc++;
                    }
                    // right
                    if (j+1 < N) {
                        int nb = curType[i*N + (j+1)];
                        if (to_map[t][2] != -1 && to_map[nb][0] != -1) sc++;
                    }
                    // down
                    if (i+1 < N) {
                        int nb = curType[(i+1)*N + j];
                        if (to_map[t][3] != -1 && to_map[nb][1] != -1) sc++;
                    }
                    if (sc > bestSc) {
                        bestSc = sc;
                        bestR = r;
                    }
                }
                rot[idx] = bestR;
                curType[idx] = rotPow[orig[idx]][bestR];
            }
        }
    }

    int curEdges = edge_match_count(curType);

    // Compute loops' top two product
    auto compute_top2_product = [&](const vector<int>& types)->long long {
        const int M = N * N * 4;
        static int next_id[30*30*4];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int t = types[i*N + j];
                int base = (i*N + j) * 4;
                for (int d = 0; d < 4; d++) {
                    int d2 = to_map[t][d];
                    if (d2 == -1) {
                        next_id[base + d] = -1;
                    } else {
                        int ni = i + di[d2], nj = j + dj[d2];
                        if (!inb(ni, nj)) {
                            next_id[base + d] = -1;
                        } else {
                            int nd = (d2 + 2) & 3; // entry direction to neighbor
                            next_id[base + d] = ((ni * N + nj) << 2) | nd;
                        }
                    }
                }
            }
        }
        static unsigned char state[30*30*4];
        static int pos[30*30*4];
        memset(state, 0, sizeof(state));
        long long L1 = 0, L2 = 0;
        vector<int> path;
        path.reserve(M);
        for (int id = 0; id < M; id++) {
            if (state[id]) continue;
            int v = id;
            path.clear();
            while (v != -1 && state[v] == 0) {
                state[v] = 1;
                pos[v] = (int)path.size();
                path.push_back(v);
                v = next_id[v];
            }
            if (v != -1 && state[v] == 1) {
                int clen = (int)path.size() - pos[v];
                if (clen > (int)L1) {
                    L2 = L1; L1 = clen;
                } else if (clen > (int)L2) {
                    L2 = clen;
                }
            }
            for (int u : path) state[u] = 2;
        }
        return L1 * L2;
    };

    long long curTop2Product = compute_top2_product(curType);

    // Simulated annealing
    Timer timer(1.95);
    XorShift rng(123456789);

    // Keep best solution
    vector<int> bestRot = rot;
    vector<int> bestType = curType;
    long long bestProd = curTop2Product;

    // Composite score to guide search (loops product dominates; edges tie-break)
    auto composite_score = [&](long long prod, int edge) -> long double {
        // scale product strongly to prioritize it
        return (long double)prod * 1000000.0L + (long double)edge;
    };

    long double curScore = composite_score(curTop2Product, curEdges);
    long double bestScore = curScore;

    // SA parameters
    const long double T0 = 3.0;
    const long double T1 = 0.01;

    // Precompute adjacency for quick delta edge updates
    auto local_edge_sum = [&](const vector<int>& types, int i, int j)->int {
        int s = 0;
        if (j-1 >= 0) s += ((to_map[types[i*N + j]][0] != -1) && (to_map[types[i*N + (j-1)]][2] != -1));
        if (i-1 >= 0) s += ((to_map[types[i*N + j]][1] != -1) && (to_map[types[(i-1)*N + j]][3] != -1));
        if (j+1 < N) s += ((to_map[types[i*N + j]][2] != -1) && (to_map[types[i*N + (j+1)]][0] != -1));
        if (i+1 < N) s += ((to_map[types[i*N + j]][3] != -1) && (to_map[types[(i+1)*N + j]][1] != -1));
        return s;
    };

    // Initial local edges sum per cell
    vector<int> localEdges(N*N, 0);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
        localEdges[i*N + j] = local_edge_sum(curType, i, j);
    }

    // SA loop
    while (!timer.over()) {
        double prog = timer.progress();
        long double T = T0 + (T1 - T0) * prog;

        int idx = rng.randInt(0, N*N - 1);
        int i = idx / N, j = idx % N;

        int oldR = rot[idx];
        int newR = rng.randInt(0, 3);
        if (newR == oldR) continue;

        int oldType = curType[idx];
        int newType = rotPow[orig[idx]][newR];

        // Delta edges around (i,j) and its neighbors for localEdges and curEdges
        int prevLocal = localEdges[idx];
        int prevNeighbors = 0;
        if (j-1 >= 0) prevNeighbors += localEdges[i*N + (j-1)];
        if (i-1 >= 0) prevNeighbors += localEdges[(i-1)*N + j];
        if (j+1 < N) prevNeighbors += localEdges[i*N + (j+1)];
        if (i+1 < N) prevNeighbors += localEdges[(i+1)*N + j];

        // Update curType temporarily
        curType[idx] = newType;

        int newLocal = local_edge_sum(curType, i, j);
        int newNeighbors = 0;
        if (j-1 >= 0) newNeighbors += local_edge_sum(curType, i, j-1);
        if (i-1 >= 0) newNeighbors += local_edge_sum(curType, i-1, j);
        if (j+1 < N) newNeighbors += local_edge_sum(curType, i, j+1);
        if (i+1 < N) newNeighbors += local_edge_sum(curType, i+1, j);

        int deltaEdge = (newLocal - prevLocal) + (newNeighbors - prevNeighbors);
        int newEdges = curEdges + deltaEdge;

        long long newProd = compute_top2_product(curType);
        long double newScore = composite_score(newProd, newEdges);

        long double diff = newScore - curScore;
        bool accept = false;
        if (diff >= 0) {
            accept = true;
        } else {
            long double prob = expl(diff / T);
            if (rng.randDouble() < prob) accept = true;
        }

        if (accept) {
            rot[idx] = newR;
            curEdges = newEdges;
            localEdges[idx] = newLocal;
            if (j-1 >= 0) localEdges[i*N + (j-1)] = local_edge_sum(curType, i, j-1);
            if (i-1 >= 0) localEdges[(i-1)*N + j] = local_edge_sum(curType, i-1, j);
            if (j+1 < N) localEdges[i*N + (j+1)] = local_edge_sum(curType, i, j+1);
            if (i+1 < N) localEdges[(i+1)*N + j] = local_edge_sum(curType, i+1, j);
            curTop2Product = newProd;
            curScore = newScore;
            if (newScore > bestScore) {
                bestScore = newScore;
                bestProd = newProd;
                bestRot = rot;
                bestType = curType;
            }
        } else {
            // revert
            curType[idx] = oldType;
        }
    }

    // Output rotations relative to original tiles
    // bestRot already stores rotation count
    string out;
    out.reserve(N*N);
    for (int i = 0; i < N*N; i++) {
        out.push_back(char('0' + (bestRot[i] % 4)));
    }
    cout << out << '\n';

    return 0;
}