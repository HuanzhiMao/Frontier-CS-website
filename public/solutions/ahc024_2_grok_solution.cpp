#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> c(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c[i][j];
        }
    }
    vector<vector<int>> d = c;

    // Compute adj
    vector<set<int>> adj(m + 1);
    // Between 1..m
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int col = d[i][j];
            // right
            if (j + 1 < n) {
                int col2 = d[i][j + 1];
                if (col != col2) {
                    adj[col].insert(col2);
                    adj[col2].insert(col);
                }
            }
            // down
            if (i + 1 < n) {
                int col2 = d[i + 1][j];
                if (col != col2) {
                    adj[col].insert(col2);
                    adj[col2].insert(col);
                }
            }
        }
    }
    // To 0
    set<int> border;
    for (int i = 0; i < n; i++) {
        border.insert(d[i][0]);
        border.insert(d[i][n - 1]);
    }
    for (int j = 0; j < n; j++) {
        border.insert(d[0][j]);
        border.insert(d[n - 1][j]);
    }
    for (int col : border) {
        adj[0].insert(col);
        adj[col].insert(0);
    }

    // positions
    vector<set<pair<int, int>>> positions(m + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            positions[d[i][j]].insert({i, j});
        }
    }

    // contact counts
    vector<vector<int>> contact(m + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c1 = d[i][j];
            if (j + 1 < n) {
                int c2 = d[i][j + 1];
                if (c1 != c2) {
                    int mn = min(c1, c2), mx = max(c1, c2);
                    contact[mn][mx]++;
                }
            }
            if (i + 1 < n) {
                int c2 = d[i + 1][j];
                if (c1 != c2) {
                    int mn = min(c1, c2), mx = max(c1, c2);
                    contact[mn][mx]++;
                }
            }
        }
    }

    // directions
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    // functions
    auto is_boundary = [&](int i, int j) {
        return i == 0 || i == n - 1 || j == 0 || j == n - 1;
    };

    auto has_adj_to_0 = [&](int col) -> bool {
        for (auto p : positions[col]) {
            int x = p.first, y = p.second;
            if (is_boundary(x, y)) return true;
            for (int r = 0; r < 4; r++) {
                int nx = x + dx[r], ny = y + dy[r];
                if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
                if (d[nx][ny] == 0) return true;
            }
        }
        return false;
    };

    auto is_connected = [&](int col, int ignore_i, int ignore_j) -> bool {
        pair<int, int> start = {-1, -1};
        int count = 0;
        for (auto p : positions[col]) {
            int x = p.first, y = p.second;
            if (x == ignore_i && y == ignore_j) continue;
            count++;
            if (start.first == -1) start = {x, y};
        }
        if (count == 0) return true;
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        queue<pair<int, int>> qq;
        qq.push(start);
        visited[start.first][start.second] = true;
        int reached = 1;
        while (!qq.empty()) {
            auto [x, y] = qq.front(); qq.pop();
            for (int r = 0; r < 4; r++) {
                int nx = x + dx[r], ny = y + dy[r];
                if (nx >= 0 && nx < n && ny >= 0 && ny < n && !visited[nx][ny] &&
                    d[nx][ny] == col && !(nx == ignore_i && ny == ignore_j)) {
                    visited[nx][ny] = true;
                    qq.push({nx, ny});
                    reached++;
                }
            }
        }
        return reached == count;
    };

    // queue
    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (d[i][j] != 0 && is_boundary(i, j)) {
                q.push({i, j});
            }
        }
    }

    while (!q.empty()) {
        auto [i, j] = q.front(); q.pop();
        if (d[i][j] == 0) continue;
        int k = d[i][j];
        if (!is_connected(k, i, j)) continue;

        // affected
        map<int, int> aff_count;
        for (int r = 0; r < 4; r++) {
            int nx = i + dx[r], ny = j + dy[r];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                int dd = d[nx][ny];
                if (dd != k && dd != 0) {
                    aff_count[dd]++;
                }
            }
        }
        bool contact_ok = true;
        for (auto& pr : aff_count) {
            int dd = pr.first, cnt = pr.second;
            int c1 = min(k, dd), c2 = max(k, dd);
            if (contact[c1][c2] == cnt && adj[k].count(dd)) {
                contact_ok = false;
                break;
            }
        }
        if (!contact_ok) continue;

        // unwanted
        bool unwanted = false;
        for (int r = 0; r < 4; r++) {
            int nx = i + dx[r], ny = j + dy[r];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                int b = d[nx][ny];
                if (b != 0 && adj[b].find(0) == adj[b].end() && !has_adj_to_0(b)) {
                    unwanted = true;
                    break;
                }
            }
        }
        if (unwanted) continue;

        // future has for k
        bool future_has = false;
        bool req = adj[k].count(0);
        if (!req) {
            future_has = true;
        } else if (positions[k].size() == 1) {
            future_has = false;
        } else {
            for (auto p : positions[k]) {
                int x = p.first, y = p.second;
                if (x == i && y == j) continue;
                if (is_boundary(x, y)) {
                    future_has = true;
                    break;
                }
                bool adj_cur_0 = false;
                for (int rr = 0; rr < 4; rr++) {
                    int nnx = x + dx[rr], nny = y + dy[rr];
                    if (nnx < 0 || nnx >= n || nny < 0 || nny >= n) continue;
                    if (d[nnx][nny] == 0) {
                        adj_cur_0 = true;
                        break;
                    }
                }
                if (adj_cur_0) {
                    future_has = true;
                    break;
                }
                if (abs(x - i) + abs(y - j) == 1) {
                    future_has = true;
                    break;
                }
            }
        }
        if (!future_has) continue;

        // remove
        d[i][j] = 0;
        positions[k].erase({i, j});
        // update contact
        for (auto& pr : aff_count) {
            int dd = pr.first, cnt = pr.second;
            int c1 = min(k, dd), c2 = max(k, dd);
            contact[c1][c2] -= cnt;
        }
        // add new to q
        for (int r = 0; r < 4; r++) {
            int nx = i + dx[r], ny = j + dy[r];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && d[nx][ny] != 0) {
                q.push({nx, ny});
            }
        }
    }

    // output
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j > 0) cout << " ";
            cout << d[i][j];
        }
        cout << endl;
    }
    return 0;
}