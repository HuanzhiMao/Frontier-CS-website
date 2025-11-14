#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int si, sj, ti, tj;
    double p;
    if (!(cin >> si >> sj >> ti >> tj >> p)) return 0;
    vector<string> H(20), V(19);
    for (int i = 0; i < 20; ++i) cin >> H[i];
    for (int i = 0; i < 19; ++i) cin >> V[i];

    const int HN = 20, WN = 20;
    const int INF = 1e9;
    vector<vector<int>> dist(HN, vector<int>(WN, INF));
    vector<vector<pair<int,int>>> prv(HN, vector<pair<int,int>>(WN, {-1,-1}));
    vector<vector<char>> prvDir(HN, vector<char>(WN, '?'));

    queue<pair<int,int>> q;
    dist[si][sj] = 0;
    q.push({si, sj});

    auto can_move = [&](int i, int j, char d)->bool{
        if (d == 'U') {
            if (i <= 0) return false;
            return V[i-1][j] == '0';
        } else if (d == 'D') {
            if (i >= HN-1) return false;
            return V[i][j] == '0';
        } else if (d == 'L') {
            if (j <= 0) return false;
            return H[i][j-1] == '0';
        } else { // R
            if (j >= WN-1) return false;
            return H[i][j] == '0';
        }
    };

    auto move_to = [&](int i, int j, char d)->pair<int,int>{
        if (d == 'U') return {i-1, j};
        if (d == 'D') return {i+1, j};
        if (d == 'L') return {i, j-1};
        return {i, j+1};
    };

    const string dirs = "UDLR";
    while (!q.empty()) {
        auto [i, j] = q.front(); q.pop();
        if (i == ti && j == tj) break;
        for (char d : dirs) {
            if (!can_move(i, j, d)) continue;
            auto [ni, nj] = move_to(i, j, d);
            if (dist[ni][nj] > dist[i][j] + 1) {
                dist[ni][nj] = dist[i][j] + 1;
                prv[ni][nj] = {i, j};
                prvDir[ni][nj] = d;
                q.push({ni, nj});
            }
        }
    }

    string path;
    if (dist[ti][tj] < INF) {
        int ci = ti, cj = tj;
        while (!(ci == si && cj == sj)) {
            char d = prvDir[ci][cj];
            path.push_back(d);
            auto pcell = prv[ci][cj];
            ci = pcell.first; cj = pcell.second;
        }
        reverse(path.begin(), path.end());
    }

    string ans;
    if (!path.empty()) {
        while ((int)ans.size() + (int)path.size() <= 200) ans += path;
        int rem = 200 - (int)ans.size();
        if (rem > 0) ans += path.substr(0, rem);
    }
    cout << ans << "\n";
    return 0;
}