#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int N = 30;
    vector<string> s;
    s.reserve(N);
    for (int i = 0; i < N; ++i) {
        string line;
        if (!(cin >> line)) return 0;
        // If the line is not length 30 (unlikely), try to read tokens and concatenate
        if ((int)line.size() != N) {
            string rebuilt = line;
            while ((int)rebuilt.size() < N) {
                string tok;
                cin >> tok;
                rebuilt += tok;
            }
            line = rebuilt.substr(0, N);
        }
        s.push_back(line);
    }
    // base tile types
    int base[N][N];
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) base[i][j] = s[i][j] - '0';

    // to mapping from problem statement
    int toTbl[8][4] = {
        {1, 0, -1, -1},
        {3, -1, -1, 0},
        {-1, -1, 3, 2},
        {-1, 2, 1, -1},
        {1, 0, 3, 2},
        {3, 2, 1, 0},
        {2, -1, 0, -1},
        {-1, 3, -1, 1},
    };
    bool hasEdge[8][4];
    for (int t = 0; t < 8; ++t) for (int d = 0; d < 4; ++d) hasEdge[t][d] = (toTbl[t][d] != -1);

    // rotation mapping: rotate 90 deg CCW once
    int rotOnce[8] = {1,2,3,0,5,4,7,6};
    int rotType[8][4];
    for (int t = 0; t < 8; ++t) {
        rotType[t][0] = t;
        for (int r = 1; r < 4; ++r) rotType[t][r] = rotOnce[rotType[t][r-1]];
    }

    // directions: 0 L, 1 U, 2 R, 3 D
    int di[4] = {0, -1, 0, 1};
    int dj[4] = {-1, 0, 1, 0};
    auto opp = [](int d){ return d ^ 2; };

    // rotations result (0..3), initialize to 0
    int rot[N][N];
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) rot[i][j] = 0;

    // current state after rotation
    int state[N][N];
    auto apply_state = [&](int i, int j){ state[i][j] = rotType[base[i][j]][rot[i][j]]; };
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) apply_state(i,j);

    // local improvement: maximize matched edges with neighbors
    auto localScore = [&](int i, int j, int t)->int{
        int sc = 0;
        for (int d = 0; d < 4; ++d) {
            int ni = i + di[d], nj = j + dj[d];
            if (0 <= ni && ni < N && 0 <= nj && nj < N) {
                if (hasEdge[t][d] && hasEdge[state[ni][nj]][opp(d)]) sc += 1;
            } else {
                // Optional: boundary penalty could be added here.
                // For simplicity, no penalty is used.
            }
        }
        return sc;
    };

    // Iterative improvement
    vector<int> order(N*N);
    iota(order.begin(), order.end(), 0);
    std::mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    int iterations = 8; // a few passes
    for (int it = 0; it < iterations; ++it) {
        shuffle(order.begin(), order.end(), rng);
        bool changed = false;
        for (int idx = 0; idx < N*N; ++idx) {
            int i = order[idx] / N;
            int j = order[idx] % N;

            int curT = state[i][j];
            int curScore = localScore(i, j, curT);

            int bestR = rot[i][j];
            int bestScore = curScore;

            for (int r = 0; r < 4; ++r) {
                if (r == rot[i][j]) continue;
                int nt = rotType[base[i][j]][r];
                int sc = localScore(i, j, nt);
                if (sc > bestScore || (sc == bestScore && (rng() & 1))) {
                    bestScore = sc;
                    bestR = r;
                }
            }
            if (bestR != rot[i][j]) {
                rot[i][j] = bestR;
                apply_state(i, j);
                changed = true;
                // We don't need to recompute neighbors here; state[] used is always current.
            }
        }
        if (!changed) break;
    }

    // Output rotations as a single line string of 900 characters
    string out;
    out.reserve(N*N);
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) out.push_back(char('0' + (rot[i][j] & 3)));
    cout << out << '\n';
    return 0;
}