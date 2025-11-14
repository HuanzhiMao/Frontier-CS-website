#include <bits/stdc++.h>
using namespace std;

static const int H = 10;
static const int W = 10;

using Board = array<array<int, W>, H>;

pair<int,int> findNthEmpty(const Board& b, int nth) {
    int cnt = 0;
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (b[r][c] == 0) {
                cnt++;
                if (cnt == nth) return {r, c};
            }
        }
    }
    return {-1, -1};
}

Board tiltBoard(const Board& b, char dir) {
    Board res{};
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) res[r][c] = 0;

    if (dir == 'L') {
        for (int r = 0; r < H; r++) {
            int idx = 0;
            for (int c = 0; c < W; c++) if (b[r][c] != 0) res[r][idx++] = b[r][c];
        }
    } else if (dir == 'R') {
        for (int r = 0; r < H; r++) {
            int idx = W - 1;
            for (int c = W - 1; c >= 0; c--) if (b[r][c] != 0) res[r][idx--] = b[r][c];
        }
    } else if (dir == 'F') { // towards row 0
        for (int c = 0; c < W; c++) {
            int idx = 0;
            for (int r = 0; r < H; r++) if (b[r][c] != 0) res[idx++][c] = b[r][c];
        }
    } else if (dir == 'B') { // towards row 9
        for (int c = 0; c < W; c++) {
            int idx = H - 1;
            for (int r = H - 1; r >= 0; r--) if (b[r][c] != 0) res[idx--][c] = b[r][c];
        }
    }
    return res;
}

long long manhattan(int r1, int c1, int r2, int c2) {
    return llabs(r1 - r2) + llabs(c1 - c2);
}

double boardCost(const Board& b, const array<pair<int,int>, 4>& target) {
    long long distSum = 0;
    int adjSame = 0;
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            int v = b[r][c];
            if (v > 0) {
                auto [tr, tc] = target[v];
                distSum += manhattan(r, c, tr, tc);
            }
            if (c + 1 < W) {
                int v2 = b[r][c + 1];
                if (v > 0 && v2 > 0 && v == v2) adjSame++;
            }
            if (r + 1 < H) {
                int v3 = b[r + 1][c];
                if (v > 0 && v3 > 0 && v == v3) adjSame++;
            }
        }
    }
    // Weight parameters
    const double wAdj = 1.0;
    return (double)distSum - wAdj * adjSame;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> f(100);
    for (int i = 0; i < 100; i++) {
        if (!(cin >> f[i])) return 0;
    }

    Board board{};
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) board[r][c] = 0;

    // Assign corners to flavors: 1->(0,0), 2->(0,9), 3->(9,9)
    array<pair<int,int>, 4> target;
    target[0] = {-1, -1};
    target[1] = {0, 0};
    target[2] = {0, 9};
    target[3] = {9, 9};

    for (int t = 0; t < 100; t++) {
        int p;
        if (!(cin >> p)) return 0;

        // Place the new candy
        auto [r, c] = findNthEmpty(board, p);
        if (r >= 0) board[r][c] = f[t];

        // Decide direction
        vector<char> dirs = {'F', 'B', 'L', 'R'};
        double bestCost = 1e100;
        vector<char> bestDirs;

        for (char d : dirs) {
            Board moved = tiltBoard(board, d);
            double cost = boardCost(moved, target);
            if (cost < bestCost - 1e-9) {
                bestCost = cost;
                bestDirs.clear();
                bestDirs.push_back(d);
            } else if (abs(cost - bestCost) <= 1e-9) {
                bestDirs.push_back(d);
            }
        }

        // Tie-breaker: prefer moving toward target corner of current candy
        char pref1 = (target[f[t]].first == 0 ? 'F' : 'B');
        char pref2 = (target[f[t]].second == 0 ? 'L' : 'R');
        char out = bestDirs[0];
        for (char d : bestDirs) {
            if (d == pref1) { out = d; break; }
        }
        if (out != pref1) {
            for (char d : bestDirs) {
                if (d == pref2) { out = d; break; }
            }
        }

        // Output direction (even at t=100; allowed to skip but safe to output)
        cout << out << '\n' << flush;

        // Apply the chosen tilt to the real board
        board = tiltBoard(board, out);
    }
    return 0;
}