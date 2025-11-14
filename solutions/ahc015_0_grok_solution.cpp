#include <bits/stdc++.h>
using namespace std;

void tilt(vector<vector<int>>& g, char dir) {
    if (dir == 'F') { // towards row 0
        for (int j = 0; j < 10; j++) {
            vector<int> collected;
            for (int i = 0; i < 10; i++) {
                if (g[i][j] != 0) collected.push_back(g[i][j]);
            }
            for (int i = 0; i < 10; i++) g[i][j] = 0;
            for (size_t k = 0; k < collected.size(); k++) {
                g[k][j] = collected[k];
            }
        }
    } else if (dir == 'B') { // towards row 9
        for (int j = 0; j < 10; j++) {
            vector<int> collected;
            for (int i = 9; i >= 0; i--) {
                if (g[i][j] != 0) collected.push_back(g[i][j]);
            }
            for (int i = 0; i < 10; i++) g[i][j] = 0;
            for (size_t k = 0; k < collected.size(); k++) {
                g[9 - k][j] = collected[k];
            }
        }
    } else if (dir == 'L') { // towards col 0
        for (int i = 0; i < 10; i++) {
            vector<int> collected;
            for (int j = 0; j < 10; j++) {
                if (g[i][j] != 0) collected.push_back(g[i][j]);
            }
            for (int j = 0; j < 10; j++) g[i][j] = 0;
            for (size_t k = 0; k < collected.size(); k++) {
                g[i][k] = collected[k];
            }
        }
    } else if (dir == 'R') { // towards col 9
        for (int i = 0; i < 10; i++) {
            vector<int> collected;
            for (int j = 9; j >= 0; j--) {
                if (g[i][j] != 0) collected.push_back(g[i][j]);
            }
            for (int j = 0; j < 10; j++) g[i][j] = 0;
            for (size_t k = 0; k < collected.size(); k++) {
                g[i][9 - k] = collected[k];
            }
        }
    }
}

int compute_sum_sq(const vector<vector<int>>& g) {
    vector<vector<bool>> vis(10, vector<bool>(10, false));
    int sum = 0;
    vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (g[i][j] != 0 && !vis[i][j]) {
                int flav = g[i][j];
                int size = 0;
                queue<pair<int, int>> q;
                q.push({i, j});
                vis[i][j] = true;
                size++;
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    for (auto [dx, dy] : dirs) {
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && !vis[nx][ny] && g[nx][ny] == flav) {
                            vis[nx][ny] = true;
                            q.push({nx, ny});
                            size++;
                        }
                    }
                }
                sum += size * size;
            }
        }
    }
    return sum;
}

int main() {
    vector<int> f(101);
    for (int i = 1; i <= 100; i++) {
        cin >> f[i];
    }
    vector<vector<int>> grid(10, vector<int>(10, 0));
    string dir_str = "FBLR";
    for (int t = 1; t <= 100; t++) {
        int p;
        cin >> p;
        int cnt = 0;
        bool placed = false;
        for (int i = 0; i < 10 && !placed; i++) {
            for (int j = 0; j < 10 && !placed; j++) {
                if (grid[i][j] == 0) {
                    cnt++;
                    if (cnt == p) {
                        grid[i][j] = f[t];
                        placed = true;
                    }
                }
            }
        }
        char bestdir = 'F';
        int bestscore = -1;
        for (char d : dir_str) {
            vector<vector<int>> temp = grid;
            tilt(temp, d);
            int score = compute_sum_sq(temp);
            if (score > bestscore) {
                bestscore = score;
                bestdir = d;
            }
        }
        cout << bestdir << endl;
        tilt(grid, bestdir);
    }
    return 0;
}