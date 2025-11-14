#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) cin >> h[i][j];
    }
    
    // Build serpentine path
    vector<pair<int,int>> path;
    path.reserve(N*N);
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            for (int j = 0; j < N; ++j) path.emplace_back(i, j);
        } else {
            for (int j = N - 1; j >= 0; --j) path.emplace_back(i, j);
        }
    }
    
    vector<string> ops;
    ops.reserve(5000);
    
    long long load = 0;
    int M = (int)path.size();
    // Phase 1: traverse forward, collect positives
    for (int t = 0; t < M; ++t) {
        auto [r, c] = path[t];
        if (h[r][c] > 0) {
            ops.push_back("+" + to_string(h[r][c]));
            load += h[r][c];
            h[r][c] = 0;
        }
        if (t + 1 < M) {
            auto [nr, nc] = path[t+1];
            if (nr == r) {
                if (nc == c + 1) ops.push_back("R");
                else ops.push_back("L");
            } else {
                if (nr == r + 1) ops.push_back("D");
                else ops.push_back("U");
            }
        }
    }
    // Phase 2: traverse backward, fill negatives
    for (int t = M - 1; t >= 0; --t) {
        auto [r, c] = path[t];
        if (h[r][c] < 0) {
            int d = -h[r][c];
            // d should be <= load
            if (d > load) d = (int)load; // safety; should not happen
            if (d > 0) {
                ops.push_back("-" + to_string(d));
                load -= d;
                h[r][c] += d; // becomes zero ideally
            }
        }
        if (t - 1 >= 0) {
            auto [nr, nc] = path[t-1];
            if (nr == r) {
                if (nc == c + 1) ops.push_back("R");
                else ops.push_back("L");
            } else {
                if (nr == r + 1) ops.push_back("D");
                else ops.push_back("U");
            }
        }
    }
    
    // Output operations
    for (auto &s : ops) cout << s << '\n';
    return 0;
}