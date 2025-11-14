#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<string> C(N);
    for (int i = 0; i < N; ++i) cin >> C[i];

    vector<vector<bool>> isF(N, vector<bool>(N, false));
    vector<vector<bool>> isO(N, vector<bool>(N, false));
    int oniCount = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == 'o') isF[i][j] = true;
            if (C[i][j] == 'x') { isO[i][j] = true; oniCount++; }
        }
    }

    // Prefix sums for Fukunokami (constant throughout)
    vector<vector<int>> prefRowF(N, vector<int>(N + 1, 0));
    vector<vector<int>> prefColF(N, vector<int>(N + 1, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            prefRowF[i][j + 1] = prefRowF[i][j] + (isF[i][j] ? 1 : 0);
        }
    }
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            prefColF[j][i + 1] = prefColF[j][i] + (isF[i][j] ? 1 : 0);
        }
    }

    auto safeLeft = [&](int i, int j) -> bool {
        // no F to the left of (i,j): cols [0..j-1]
        return prefRowF[i][j] == 0;
    };
    auto safeRight = [&](int i, int j) -> bool {
        // no F to the right of (i,j): cols [j+1..N-1]
        return (prefRowF[i][N] - prefRowF[i][j + 1]) == 0;
    };
    auto safeUp = [&](int i, int j) -> bool {
        // no F above (i,j): rows [0..i-1]
        return prefColF[j][i] == 0;
    };
    auto safeDown = [&](int i, int j) -> bool {
        // no F below (i,j): rows [i+1..N-1]
        return (prefColF[j][N] - prefColF[j][i + 1]) == 0;
    };

    struct Move {
        int rem;    // number of oni removed
        int cost;   // number of operations (2*t)
        char dir;   // first direction
        int index;  // row or column index for the operation
        int t;      // number of shifts in each direction
        int r, c;   // position of target oni
        bool valid;
        Move(): rem(0), cost(0), dir('U'), index(0), t(0), r(0), c(0), valid(false) {}
    };

    vector<pair<char,int>> ops;

    while (oniCount > 0) {
        Move best;
        long long bestNum = -1; // rem * other.cost
        long long bestDen = 1;  // cost
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!isO[i][j]) continue;
                // Up
                if (safeUp(i, j)) {
                    int t = i + 1;
                    int cnt = 0;
                    for (int r = 0; r <= i; ++r) if (isO[r][j]) cnt++;
                    if (cnt > 0) {
                        int cost = 2 * t;
                        long long num = (long long)cnt * (best.valid ? best.cost : 1);
                        long long den = (long long)cost * (best.valid ? 1 : 1);
                        bool better = false;
                        if (!best.valid) better = true;
                        else {
                            long long curNum = (long long)best.rem * cost;
                            long long candNum = (long long)cnt * best.cost;
                            if (candNum > curNum) better = true;
                            else if (candNum == curNum) {
                                if (cnt > best.rem) better = true;
                                else if (cnt == best.rem && cost < best.cost) better = true;
                            }
                        }
                        if (better) {
                            best.valid = true;
                            best.rem = cnt;
                            best.cost = cost;
                            best.dir = 'U';
                            best.index = j;
                            best.t = t;
                            best.r = i;
                            best.c = j;
                        }
                    }
                }
                // Down
                if (safeDown(i, j)) {
                    int t = N - i;
                    int cnt = 0;
                    for (int r = i; r < N; ++r) if (isO[r][j]) cnt++;
                    if (cnt > 0) {
                        int cost = 2 * t;
                        bool better = false;
                        if (!best.valid) better = true;
                        else {
                            long long curNum = (long long)best.rem * cost;
                            long long candNum = (long long)cnt * best.cost;
                            if (candNum > curNum) better = true;
                            else if (candNum == curNum) {
                                if (cnt > best.rem) better = true;
                                else if (cnt == best.rem && cost < best.cost) better = true;
                            }
                        }
                        if (better) {
                            best.valid = true;
                            best.rem = cnt;
                            best.cost = cost;
                            best.dir = 'D';
                            best.index = j;
                            best.t = t;
                            best.r = i;
                            best.c = j;
                        }
                    }
                }
                // Left
                if (safeLeft(i, j)) {
                    int t = j + 1;
                    int cnt = 0;
                    for (int c = 0; c <= j; ++c) if (isO[i][c]) cnt++;
                    if (cnt > 0) {
                        int cost = 2 * t;
                        bool better = false;
                        if (!best.valid) better = true;
                        else {
                            long long curNum = (long long)best.rem * cost;
                            long long candNum = (long long)cnt * best.cost;
                            if (candNum > curNum) better = true;
                            else if (candNum == curNum) {
                                if (cnt > best.rem) better = true;
                                else if (cnt == best.rem && cost < best.cost) better = true;
                            }
                        }
                        if (better) {
                            best.valid = true;
                            best.rem = cnt;
                            best.cost = cost;
                            best.dir = 'L';
                            best.index = i;
                            best.t = t;
                            best.r = i;
                            best.c = j;
                        }
                    }
                }
                // Right
                if (safeRight(i, j)) {
                    int t = N - j;
                    int cnt = 0;
                    for (int c = j; c < N; ++c) if (isO[i][c]) cnt++;
                    if (cnt > 0) {
                        int cost = 2 * t;
                        bool better = false;
                        if (!best.valid) better = true;
                        else {
                            long long curNum = (long long)best.rem * cost;
                            long long candNum = (long long)cnt * best.cost;
                            if (candNum > curNum) better = true;
                            else if (candNum == curNum) {
                                if (cnt > best.rem) better = true;
                                else if (cnt == best.rem && cost < best.cost) better = true;
                            }
                        }
                        if (better) {
                            best.valid = true;
                            best.rem = cnt;
                            best.cost = cost;
                            best.dir = 'R';
                            best.index = i;
                            best.t = t;
                            best.r = i;
                            best.c = j;
                        }
                    }
                }
            }
        }

        if (!best.valid) break; // Should not happen with given guarantees

        // Apply removal logically
        if (best.dir == 'U') {
            int j = best.c;
            int i = best.r;
            for (int r = 0; r <= i; ++r) {
                if (isO[r][j]) { isO[r][j] = false; oniCount--; }
            }
        } else if (best.dir == 'D') {
            int j = best.c;
            int i = best.r;
            for (int r = i; r < N; ++r) {
                if (isO[r][j]) { isO[r][j] = false; oniCount--; }
            }
        } else if (best.dir == 'L') {
            int i = best.r;
            int j = best.c;
            for (int c = 0; c <= j; ++c) {
                if (isO[i][c]) { isO[i][c] = false; oniCount--; }
            }
        } else if (best.dir == 'R') {
            int i = best.r;
            int j = best.c;
            for (int c = j; c < N; ++c) {
                if (isO[i][c]) { isO[i][c] = false; oniCount--; }
            }
        }

        // Record operations
        auto emit = [&](char d, int p, int times){
            for (int k = 0; k < times; ++k) ops.emplace_back(d, p);
        };
        char d1 = best.dir, d2;
        if (d1 == 'U') d2 = 'D';
        else if (d1 == 'D') d2 = 'U';
        else if (d1 == 'L') d2 = 'R';
        else d2 = 'L';
        int p = best.index;
        emit(d1, p, best.t);
        emit(d2, p, best.t);

        if ((int)ops.size() > 4 * N * N) break; // safety
    }

    // Output
    for (auto &op : ops) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}