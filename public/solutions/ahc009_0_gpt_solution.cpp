#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int si, sj, ti, tj;
    double p;
    if (!(cin >> si >> sj >> ti >> tj >> p)) return 0;

    vector<string> h(20), v(19);
    for (int i = 0; i < 20; ++i) cin >> h[i];
    for (int i = 0; i < 19; ++i) cin >> v[i];

    const int H = 20, W = 20;
    vector<vector<int>> vis(H, vector<int>(W, 0));
    vector<vector<int>> parR(H, vector<int>(W, -1));
    vector<vector<int>> parC(H, vector<int>(W, -1));
    vector<vector<char>> pch(H, vector<char>(W, 0));

    queue<pair<int,int>> q;
    q.push({si, sj});
    vis[si][sj] = 1;

    auto push = [&](int r, int c, int nr, int nc, char ch) {
        if (nr < 0 || nr >= H || nc < 0 || nc >= W) return;
        if (vis[nr][nc]) return;
        vis[nr][nc] = 1;
        parR[nr][nc] = r;
        parC[nr][nc] = c;
        pch[nr][nc] = ch;
        q.push({nr, nc});
    };

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == ti && c == tj) break;
        // Up
        if (r > 0 && v[r-1][c] == '0') push(r, c, r-1, c, 'U');
        // Down
        if (r < 19 && v[r][c] == '0') push(r, c, r+1, c, 'D');
        // Left
        if (c > 0 && h[r][c-1] == '0') push(r, c, r, c-1, 'L');
        // Right
        if (c < 19 && h[r][c] == '0') push(r, c, r, c+1, 'R');
    }

    string res;
    if (!vis[ti][tj]) {
        // Fallback (should not happen due to problem guarantees). Output empty string.
        cout << "\n";
        return 0;
    } else {
        int r = ti, c = tj;
        while (!(r == si && c == sj)) {
            res.push_back(pch[r][c]);
            int pr = parR[r][c], pc = parC[r][c];
            r = pr; c = pc;
        }
        reverse(res.begin(), res.end());
    }

    if ((int)res.size() > 200) res.resize(200);
    cout << res << "\n";
    return 0;
}