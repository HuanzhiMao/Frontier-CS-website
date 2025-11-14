#include <bits/stdc++.h>
using namespace std;

const int H = 10, W = 10;

using Grid = array<array<int, W>, H>;

Grid tiltGrid(const Grid &g, char d) {
    Grid res{};
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) res[i][j] = 0;
    if (d == 'L') {
        for (int i = 0; i < H; ++i) {
            int idx = 0;
            for (int j = 0; j < W; ++j) if (g[i][j] != 0) res[i][idx++] = g[i][j];
        }
    } else if (d == 'R') {
        for (int i = 0; i < H; ++i) {
            int idx = W - 1;
            for (int j = W - 1; j >= 0; --j) if (g[i][j] != 0) res[i][idx--] = g[i][j];
        }
    } else if (d == 'F') { // up
        for (int j = 0; j < W; ++j) {
            int idx = 0;
            for (int i = 0; i < H; ++i) if (g[i][j] != 0) res[idx++][j] = g[i][j];
        }
    } else if (d == 'B') { // down
        for (int j = 0; j < W; ++j) {
            int idx = H - 1;
            for (int i = H - 1; i >= 0; --i) if (g[i][j] != 0) res[idx--][j] = g[i][j];
        }
    }
    return res;
}

long long computeScore(const Grid &g) {
    static bool vis[H][W];
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) vis[i][j] = false;
    long long score = 0;
    const int di[4] = {-1,1,0,0};
    const int dj[4] = {0,0,-1,1};
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (g[i][j] == 0 || vis[i][j]) continue;
            int color = g[i][j];
            int cnt = 0;
            queue<pair<int,int>> q;
            q.emplace(i,j);
            vis[i][j] = true;
            while (!q.empty()) {
                auto [r,c] = q.front(); q.pop();
                cnt++;
                for (int k = 0; k < 4; ++k) {
                    int nr = r + di[k], nc = c + dj[k];
                    if (nr < 0 || nr >= H || nc < 0 || nc >= W) continue;
                    if (vis[nr][nc]) continue;
                    if (g[nr][nc] != color) continue;
                    vis[nr][nc] = true;
                    q.emplace(nr,nc);
                }
            }
            score += 1LL * cnt * cnt;
        }
    }
    return score;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> f(100);
    for (int i = 0; i < 100; ++i) {
        if (!(cin >> f[i])) return 0;
    }

    Grid grid{};
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j) grid[i][j] = 0;

    const array<char,4> dirs = {'F','L','B','R'}; // tie-break preference

    for (int t = 0; t < 100; ++t) {
        int p; 
        if (!(cin >> p)) return 0;
        vector<pair<int,int>> empties;
        empties.reserve(101 - t);
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                if (grid[i][j] == 0) empties.emplace_back(i, j);
            }
        }
        int idx = p - 1;
        if (idx < 0 || idx >= (int)empties.size()) idx = 0; // safety
        auto [ri, rj] = empties[idx];
        grid[ri][rj] = f[t];

        long long bestScore = -1;
        char bestDir = 'F';
        Grid bestGrid = grid;
        for (char d : dirs) {
            Grid g2 = tiltGrid(grid, d);
            long long sc = computeScore(g2);
            if (sc > bestScore) {
                bestScore = sc;
                bestDir = d;
                bestGrid = g2;
            }
        }

        grid = bestGrid;
        cout << bestDir << endl;
    }

    return 0;
}