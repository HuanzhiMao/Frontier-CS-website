#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> h(N, vector<int>(N));
    long long base = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> h[i][j];
            base += llabs(h[i][j]);
        }
    }

    if (base == 0) {
        // Already flat, output nothing.
        return 0;
    }

    // Build serpentine path
    vector<pair<int,int>> path;
    path.reserve(N*N);
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            for (int j = 0; j < N; ++j) path.emplace_back(i, j);
        } else {
            for (int j = N-1; j >= 0; --j) path.emplace_back(i, j);
        }
    }

    vector<string> ops;
    vector<vector<int>> cur = h;
    long long load = 0;

    auto do_load = [&](int d){
        if (d <= 0) return;
        ops.push_back("+" + to_string(d));
        load += d;
    };
    auto do_unload = [&](int d){
        if (d <= 0) return;
        ops.push_back("-" + to_string(d));
        load -= d;
    };
    auto move_char = [&](pair<int,int> a, pair<int,int> b) -> char {
        if (a.first == b.first) {
            if (b.second == a.second + 1) return 'R';
            if (b.second == a.second - 1) return 'L';
        } else if (a.second == b.second) {
            if (b.first == a.first + 1) return 'D';
            if (b.first == a.first - 1) return 'U';
        }
        return 'U'; // should not happen
    };

    // Forward pass
    for (int idx = 0; idx < (int)path.size(); ++idx) {
        auto [x, y] = path[idx];
        if (cur[x][y] > 0) {
            do_load(cur[x][y]);
            cur[x][y] = 0;
        }
        if (cur[x][y] < 0) {
            int d = (int)min<long long>(load, - (long long)cur[x][y]);
            if (d > 0) {
                do_unload(d);
                cur[x][y] += d;
            }
        }
        if (idx + 1 < (int)path.size()) {
            char mv = move_char(path[idx], path[idx + 1]);
            ops.emplace_back(string(1, mv));
        }
    }

    // Reverse pass until load is zero
    if (load > 0) {
        for (int r = (int)path.size() - 1; r >= 0; --r) {
            auto [x, y] = path[r];
            if (cur[x][y] < 0) {
                int d = (int)min<long long>(load, - (long long)cur[x][y]);
                if (d > 0) {
                    do_unload(d);
                    cur[x][y] += d;
                }
            }
            if (load == 0) break;
            if (r - 1 >= 0) {
                char mv = move_char(path[r], path[r - 1]);
                ops.emplace_back(string(1, mv));
            }
        }
    }

    // Output operations
    for (auto &s : ops) cout << s << '\n';

    return 0;
}