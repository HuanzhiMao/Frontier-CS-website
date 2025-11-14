#include <bits/stdc++.h>
using namespace std;

void tilt(vector<vector<int>>& g, char dir) {
    if (dir == 'F') {
        for (int j = 0; j < 10; j++) {
            vector<int> candies;
            for (int i = 0; i < 10; i++) if (g[i][j] != 0) candies.push_back(g[i][j]);
            for (int i = 0; i < 10; i++) g[i][j] = 0;
            for (size_t k = 0; k < candies.size(); k++) g[k][j] = candies[k];
        }
    } else if (dir == 'B') {
        for (int j = 0; j < 10; j++) {
            vector<int> candies;
            for (int i = 0; i < 10; i++) if (g[i][j] != 0) candies.push_back(g[i][j]);
            for (int i = 0; i < 10; i++) g[i][j] = 0;
            for (size_t k = 0; k < candies.size(); k++) g[9 - k][j] = candies[candies.size() - 1 - k];
        }
    } else if (dir == 'L') {
        for (int i = 0; i < 10; i++) {
            vector<int> candies;
            for (int j = 0; j < 10; j++) if (g[i][j] != 0) candies.push_back(g[i][j]);
            for (int j = 0; j < 10; j++) g[i][j] = 0;
            for (size_t k = 0; k < candies.size(); k++) g[i][k] = candies[k];
        }
    } else if (dir == 'R') {
        for (int i = 0; i < 10; i++) {
            vector<int> candies;
            for (int j = 0; j < 10; j++) if (g[i][j] != 0) candies.push_back(g[i][j]);
            for (int j = 0; j < 10; j++) g[i][j] = 0;
            for (size_t k = 0; k < candies.size(); k++) g[i][9 - k] = candies[candies.size() - 1 - k];
        }
    }
}

long long compute(const vector<vector<int>>& g) {
    vector<vector<bool>> visited(10, vector<bool>(10, false));
    long long sum_sq = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (g[i][j] != 0 && !visited[i][j]) {
                int flav = g[i][j];
                int size = 0;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;
                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    size++;
                    vector<pair<int, int>> deltas = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
                    for (auto [dx, dy] : deltas) {
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && !visited[nx][ny] && g[nx][ny] == flav) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
                sum_sq += (long long)size * size;
            }
        }
    }
    return sum_sq;
}

int main() {
    vector<int> f(100);
    for (int i = 0; i < 100; i++) cin >> f[i];
    vector<vector<int>> grid(10, vector<int>(10, 0));
    for (int t = 0; t < 100; t++) {
        int p; cin >> p;
        int count = 0;
        int pr = -1, pc = -1;
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 10; c++) {
                if (grid[r][c] == 0) {
                    count++;
                    if (count == p) {
                        pr = r; pc = c;
                        goto placed;
                    }
                }
            }
        }
    placed:
        grid[pr][pc] = f[t];
        char dirs[] = {'F', 'B', 'L', 'R'};
        long long best_sc = -1;
        char choice = 'F';
        for (int d = 0; d < 4; d++) {
            auto temp = grid;
            tilt(temp, dirs[d]);
            long long sc = compute(temp);
            if (sc > best_sc) {
                best_sc = sc;
                choice = dirs[d];
            }
        }
        cout << choice << endl;
        tilt(grid, choice);
    }
    return 0;
}