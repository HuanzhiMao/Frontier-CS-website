#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<string> C(N);
    for (int i = 0; i < N; ++i) cin >> C[i];

    vector<pair<char,int>> ops;
    auto add_ops = [&](char d, int p, int times) {
        for (int t = 0; t < times; ++t) ops.emplace_back(d, p);
    };

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (C[i][j] != 'x') continue;

            bool up = true, down = true, left = true, right = true;

            for (int k = 0; k < i; ++k) if (C[k][j] == 'o') { up = false; break; }
            for (int k = i + 1; k < N; ++k) if (C[k][j] == 'o') { down = false; break; }
            for (int k = 0; k < j; ++k) if (C[i][k] == 'o') { left = false; break; }
            for (int k = j + 1; k < N; ++k) if (C[i][k] == 'o') { right = false; break; }

            int bestCost = INT_MAX;
            char bestDir = '?';
            int param = -1, times = 0;

            if (up) {
                int cost = 2 * (i + 1);
                if (cost < bestCost) {
                    bestCost = cost; bestDir = 'U';
                    param = j; times = i + 1;
                }
            }
            if (down) {
                int cost = 2 * (N - i);
                if (cost < bestCost) {
                    bestCost = cost; bestDir = 'D';
                    param = j; times = N - i;
                }
            }
            if (left) {
                int cost = 2 * (j + 1);
                if (cost < bestCost) {
                    bestCost = cost; bestDir = 'L';
                    param = i; times = j + 1;
                }
            }
            if (right) {
                int cost = 2 * (N - j);
                if (cost < bestCost) {
                    bestCost = cost; bestDir = 'R';
                    param = i; times = N - j;
                }
            }

            // Fallback (shouldn't happen due to guarantee)
            if (bestDir == '?') {
                if (i + 1 <= N - i) { bestDir = 'U'; param = j; times = i + 1; }
                else { bestDir = 'D'; param = j; times = N - i; }
            }

            if (bestDir == 'U') {
                add_ops('U', param, times);
                add_ops('D', param, times);
            } else if (bestDir == 'D') {
                add_ops('D', param, times);
                add_ops('U', param, times);
            } else if (bestDir == 'L') {
                add_ops('L', param, times);
                add_ops('R', param, times);
            } else if (bestDir == 'R') {
                add_ops('R', param, times);
                add_ops('L', param, times);
            }
        }
    }

    // Output operations (at most 4*N*N guaranteed)
    for (auto &op : ops) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}