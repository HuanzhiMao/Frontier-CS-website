#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    vector<int> sz(m + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sz[grid[i][j]]++;
        }
    }
    vector<vector<int>> cnt(m + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c = grid[i][j];
            // right
            if (j + 1 < n) {
                int d = grid[i][j + 1];
                if (c != d) {
                    cnt[c][d]++;
                    cnt[d][c]++;
                }
            }
            // down
            if (i + 1 < n) {
                int d = grid[i + 1][j];
                if (c != d) {
                    cnt[c][d]++;
                    cnt[d][c]++;
                }
            }
        }
    }
    // contacts to 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c = grid[i][j];
            int outer = 0;
            if (i == 0) outer++;
            if (i == n - 1) outer++;
            if (j == 0) outer++;
            if (j == n - 1) outer++;
            if (outer > 0) {
                cnt[c][0] += outer;
                cnt[0][c] += outer;
            }
        }
    }
    set<int> landlocked;
    for (int c = 1; c <= m; c++) {
        if (cnt[c][0] == 0) landlocked.insert(c);
    }
    auto is_frontier = [&](int i, int j) -> bool {
        if (grid[i][j] == 0) return false;
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x < 0 || x >= n || y < 0 || y >= n) return true;
            if (grid[x][y] == 0) return true;
        }
        return false;
    };
    set<pair<int, int>> cand;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (is_frontier(i, j)) cand.insert({i, j});
        }
    }
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    while (!cand.empty()) {
        auto it = cand.begin();
        int i = it->first;
        int j = it->second;
        cand.erase(it);
        if (grid[i][j] == 0) continue;
        int c = grid[i][j];
        if (landlocked.count(c)) continue;
        bool has_L = false;
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x >= 0 && x < n && y >= 0 && y < n && landlocked.count(grid[x][y])) {
                has_L = true;
                break;
            }
        }
        if (has_L) continue;
        if (!is_frontier(i, j)) continue;
        // compute contacts to d > 0
        map<int, int> contacts;
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x >= 0 && x < n && y >= 0 && y < n) {
                int dd = grid[x][y];
                if (dd != c && dd != 0) contacts[dd]++;
            }
        }
        bool can = true;
        for (auto& p : contacts) {
            int dd = p.first;
            if (p.second == cnt[c][dd]) {
                can = false;
                break;
            }
        }
        // to 0
        int to0 = 0;
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x < 0 || x >= n || y < 0 || y >= n) to0++;
            else if (grid[x][y] == 0) to0++;
        }
        if (cnt[c][0] > 0 && to0 == cnt[c][0]) can = false;
        if (!can) continue;
        // check connectivity
        int temp = grid[i][j];
        grid[i][j] = -1;
        pair<int, int> start = {-1, -1};
        int reached = 0;
        for (int a = 0; a < n && start.first == -1; a++) {
            for (int b = 0; b < n; b++) {
                if (grid[a][b] == c) {
                    start = {a, b};
                    break;
                }
            }
        }
        if (start.first != -1) {
            vector<vector<bool>> vis(n, vector<bool>(n, false));
            queue<pair<int, int>> q;
            q.push(start);
            vis[start.first][start.second] = true;
            reached = 1;
            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (nx >= 0 && nx < n && ny >= 0 && ny < n && !vis[nx][ny] && grid[nx][ny] == c) {
                        vis[nx][ny] = true;
                        q.push({nx, ny});
                        reached++;
                    }
                }
            }
        }
        bool connected = (reached == sz[c] - 1);
        grid[i][j] = temp;
        if (!connected) continue;
        // remove
        grid[i][j] = 0;
        sz[c]--;
        // update cnt for d > 0
        for (auto& p : contacts) {
            int dd = p.first;
            int num = p.second;
            cnt[c][dd] -= num;
            cnt[dd][c] -= num;
        }
        // update for 0
        cnt[c][0] -= to0;
        cnt[0][c] -= to0;
        // add new contacts to 0
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x >= 0 && x < n && y >= 0 && y < n && grid[x][y] > 0) {
                int dd = grid[x][y];
                cnt[dd][0]++;
                cnt[0][dd]++;
            }
        }
        // add new candidates
        for (int d = 0; d < 4; d++) {
            int x = i + dx[d];
            int y = j + dy[d];
            if (x >= 0 && x < n && y >= 0 && y < n && grid[x][y] != 0) {
                cand.insert({x, y});
            }
        }
    }
    // output
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j > 0) cout << " ";
            cout << grid[i][j];
        }
        cout << endl;
    }
    return 0;
}