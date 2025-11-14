#include <bits/stdc++.h>
using namespace std;

pair<int, int> can_move(int x, int y, char ch, const vector<string>& h, const vector<string>& v) {
    map<char, pair<int, int>> dirs;
    dirs['U'] = {-1, 0};
    dirs['D'] = {1, 0};
    dirs['L'] = {0, -1};
    dirs['R'] = {0, 1};
    auto [dx, dy] = dirs[ch];
    int nx = x + dx;
    int ny = y + dy;
    if (nx < 0 || nx > 19 || ny < 0 || ny > 19) return {x, y};
    bool wall = true;
    if (ch == 'R') {
        if (h[x][y] == '0') wall = false;
    } else if (ch == 'L') {
        if (h[x][ny] == '0') wall = false;
    } else if (ch == 'D') {
        if (v[x][y] == '0') wall = false;
    } else if (ch == 'U') {
        if (v[nx][y] == '0') wall = false;
    }
    if (wall) return {x, y};
    return {nx, ny};
}

double compute_E(const string& S, int si, int sj, int ti, int tj, double pp, const vector<string>& h, const vector<string>& v) {
    int L = S.length();
    if (L > 200 || L < 0) return 0.0;
    vector<vector<vector<double>>> dp(2, vector<vector<double>>(20, vector<double>(20, 0.0)));
    int cur = 0, nxt = 1;
    dp[cur][si][sj] = 1.0;
    double ES = 0.0;
    for (int t = 0; t < L; ++t) {
        for (int i = 0; i < 20; ++i) for (int j = 0; j < 20; ++j) dp[nxt][i][j] = 0.0;
        char ch = S[t];
        for (int x = 0; x < 20; ++x) {
            for (int y = 0; y < 20; ++y) {
                if (dp[cur][x][y] == 0.0) continue;
                if (x == ti && y == tj) continue;
                // forget
                double prob_stay = dp[cur][x][y] * pp;
                dp[nxt][x][y] += prob_stay;
                // move
                double prob_move = dp[cur][x][y] * (1 - pp);
                auto [nx, ny] = can_move(x, y, ch, h, v);
                if (nx == ti && ny == tj) {
                    ES += prob_move * (401 - (t + 1));
                } else {
                    dp[nxt][nx][ny] += prob_move;
                }
            }
        }
        swap(cur, nxt);
    }
    return ES;
}

int main() {
    int si, sj, ti, tj;
    double pp;
    cin >> si >> sj >> ti >> tj >> pp;
    vector<string> h(20);
    for (auto& s : h) cin >> s;
    vector<string> v(19);
    for (auto& s : v) cin >> s;
    vector<vector<int>> dist(20, vector<int>(20, -1));
    queue<pair<int, int>> q;
    q.push({si, sj});
    dist[si][sj] = 0;
    vector<vector<pair<int, int>>> parent(20, vector<pair<int, int>>(20, {-1, -1}));
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (char ch : {'U', 'D', 'L', 'R'}) {
            auto [nx, ny] = can_move(x, y, ch, h, v);
            if (nx == x && ny == y) continue;
            if (dist[nx][ny] == -1) {
                dist[nx][ny] = dist[x][y] + 1;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }
    if (dist[ti][tj] == -1) {
        cout << "" << endl;
        return 0;
    }
    int k = dist[ti][tj];
    vector<char> moves;
    pair<int, int> cur = {ti, tj};
    while (cur != make_pair(si, sj)) {
        pair<int, int> prev = parent[cur.first][cur.second];
        int dx = cur.first - prev.first;
        int dy = cur.second - prev.second;
        char ch;
        if (dx == -1) ch = 'U';
        else if (dx == 1) ch = 'D';
        else if (dy == -1) ch = 'L';
        else ch = 'R';
        moves.push_back(ch);
        cur = prev;
    }
    reverse(moves.begin(), moves.end());
    string P(moves.begin(), moves.end());
    int max_r = 200 / k;
    if (max_r == 0) {
        cout << "" << endl;
        return 0;
    }
    double best = -1.0;
    string bestS;
    for (int r = 1; r <= max_r; ++r) {
        string SS;
        for (int i = 0; i < r; ++i) SS += P;
        double e = compute_E(SS, si, sj, ti, tj, pp, h, v);
        if (e > best) {
            best = e;
            bestS = SS;
        }
    }
    cout << bestS << endl;
    return 0;
}