#include <bits/stdc++.h>
using namespace std;

const int N = 10;

void tilt(vector<vector<int>>& g, char dir) {
    if (dir == 'L' || dir == 'R') {
        for (int r = 0; r < N; r++) {
            vector<int> v;
            v.reserve(N);
            for (int c = 0; c < N; c++) if (g[r][c]) v.push_back(g[r][c]);
            for (int c = 0; c < N; c++) g[r][c] = 0;
            if (dir == 'L') {
                for (int i = 0; i < (int)v.size(); i++) g[r][i] = v[i];
            } else {
                int start = N - (int)v.size();
                for (int i = 0; i < (int)v.size(); i++) g[r][start + i] = v[i];
            }
        }
    } else {
        for (int c = 0; c < N; c++) {
            vector<int> v;
            v.reserve(N);
            for (int r = 0; r < N; r++) if (g[r][c]) v.push_back(g[r][c]);
            for (int r = 0; r < N; r++) g[r][c] = 0;
            if (dir == 'F') {
                for (int i = 0; i < (int)v.size(); i++) g[i][c] = v[i];
            } else {
                int start = N - (int)v.size();
                for (int i = 0; i < (int)v.size(); i++) g[start + i][c] = v[i];
            }
        }
    }
}

long long scoreGrid(const vector<vector<int>>& g) {
    static int dr[4] = {-1, 1, 0, 0};
    static int dc[4] = {0, 0, -1, 1};
    bool vis[N][N] = {};
    long long ans = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (g[r][c] == 0 || vis[r][c]) continue;
            int col = g[r][c];
            int cnt = 0;
            queue<pair<int,int>> q;
            q.push({r,c});
            vis[r][c] = true;
            while (!q.empty()) {
                auto [rr, cc] = q.front(); q.pop();
                cnt++;
                for (int k = 0; k < 4; k++) {
                    int nr = rr + dr[k], nc = cc + dc[k];
                    if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                    if (vis[nr][nc] || g[nr][nc] != col) continue;
                    vis[nr][nc] = true;
                    q.push({nr,nc});
                }
            }
            ans += 1LL * cnt * cnt;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> f(100);
    for (int i = 0; i < 100; i++) {
        if (!(cin >> f[i])) return 0;
    }

    vector<vector<int>> grid(N, vector<int>(N, 0));

    for (int t = 0; t < 100; t++) {
        int p;
        if (!(cin >> p)) return 0;

        // place new candy at p-th empty cell (row-major: front-to-back, left-to-right)
        int rPlace = -1, cPlace = -1, cnt = 0;
        for (int r = 0; r < N && rPlace == -1; r++) {
            for (int c = 0; c < N; c++) {
                if (grid[r][c] == 0) {
                    cnt++;
                    if (cnt == p) { rPlace = r; cPlace = c; break; }
                }
            }
        }
        if (rPlace == -1) return 0; // should not happen
        grid[rPlace][cPlace] = f[t];

        if (t == 99) {
            // last move has no effect; may skip output
            break;
        }

        // Try all directions and pick the best by current connected components squared
        const char dirs[4] = {'F', 'L', 'R', 'B'}; // tie-break preference
        long long bestScore = -1;
        char bestDir = 'F';
        for (char d : dirs) {
            auto tmp = grid;
            tilt(tmp, d);
            long long s = scoreGrid(tmp);
            if (s > bestScore) {
                bestScore = s;
                bestDir = d;
            }
        }

        cout << bestDir << endl;
        // Apply the chosen tilt to the actual grid
        tilt(grid, bestDir);
    }

    return 0;
}