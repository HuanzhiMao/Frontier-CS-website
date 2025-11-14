#include <bits/stdc++.h>
using namespace std;

using Grid = array<array<int, 10>, 10>;

Grid tilt(const Grid& g, char dir) {
    Grid ng{};
    for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++) ng[i][j] = 0;
    if (dir == 'F') {
        for (int c = 0; c < 10; c++) {
            int write = 0;
            for (int r = 0; r < 10; r++) {
                if (g[r][c] != 0) {
                    ng[write][c] = g[r][c];
                    write++;
                }
            }
        }
    } else if (dir == 'B') {
        for (int c = 0; c < 10; c++) {
            int write = 9;
            for (int r = 9; r >= 0; r--) {
                if (g[r][c] != 0) {
                    ng[write][c] = g[r][c];
                    write--;
                }
            }
        }
    } else if (dir == 'L') {
        for (int r = 0; r < 10; r++) {
            int write = 0;
            for (int c = 0; c < 10; c++) {
                if (g[r][c] != 0) {
                    ng[r][write] = g[r][c];
                    write++;
                }
            }
        }
    } else { // 'R'
        for (int r = 0; r < 10; r++) {
            int write = 9;
            for (int c = 9; c >= 0; c--) {
                if (g[r][c] != 0) {
                    ng[r][write] = g[r][c];
                    write--;
                }
            }
        }
    }
    return ng;
}

long long scoreGrid(const Grid& g) {
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    bool vis[10][10] = {};
    long long res = 0;
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            if (g[r][c] == 0 || vis[r][c]) continue;
            int color = g[r][c];
            int cnt = 0;
            queue<pair<int,int>> q;
            q.emplace(r, c);
            vis[r][c] = true;
            while (!q.empty()) {
                auto [rr, cc] = q.front(); q.pop();
                cnt++;
                for (int k = 0; k < 4; k++) {
                    int nr = rr + dr[k], nc = cc + dc[k];
                    if (nr < 0 || nr >= 10 || nc < 0 || nc >= 10) continue;
                    if (!vis[nr][nc] && g[nr][nc] == color) {
                        vis[nr][nc] = true;
                        q.emplace(nr, nc);
                    }
                }
            }
            res += 1LL * cnt * cnt;
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> f(100);
    for (int i = 0; i < 100; i++) {
        if (!(cin >> f[i])) return 0;
    }

    Grid grid{};
    for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++) grid[i][j] = 0;

    for (int t = 0; t < 100; t++) {
        int p;
        if (!(cin >> p)) return 0;

        vector<pair<int,int>> empties;
        empties.reserve(101 - t);
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 10; c++) {
                if (grid[r][c] == 0) empties.emplace_back(r, c);
            }
        }
        auto [r, c] = empties[p - 1];
        grid[r][c] = f[t];

        if (t == 99) {
            // No output needed; tilting has no effect on a full grid.
            continue;
        }

        array<char,4> dirs = {'F','B','L','R'};
        long long bestVal = -1;
        vector<char> bestDirs;
        for (char d : dirs) {
            Grid ng = tilt(grid, d);
            long long val = scoreGrid(ng);
            if (val > bestVal) {
                bestVal = val;
                bestDirs.clear();
                bestDirs.push_back(d);
            } else if (val == bestVal) {
                bestDirs.push_back(d);
            }
        }

        // Tie-break preference based on current flavor
        vector<char> pref;
        if (f[t] == 1) pref = {'F','L','R','B'};     // target top-left
        else if (f[t] == 2) pref = {'R','F','B','L'}; // target top-right
        else pref = {'B','R','L','F'};               // target bottom-right

        char ans = bestDirs[0];
        for (char d : pref) {
            if (find(bestDirs.begin(), bestDirs.end(), d) != bestDirs.end()) {
                ans = d;
                break;
            }
        }

        cout << ans << '\n' << flush;
        grid = tilt(grid, ans);
    }

    return 0;
}