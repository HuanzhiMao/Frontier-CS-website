#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int si, sj, ti, tj;
    double p;
    if (!(cin >> si >> sj >> ti >> tj >> p)) {
        return 0;
    }
    const int N = 20;
    vector<string> h(N);      // h[i][j] for j in [0..18], between (i,j) and (i,j+1)
    vector<string> v(N - 1);  // v[i][j] for i in [0..18], between (i,j) and (i+1,j)
    for (int i = 0; i < N; i++) cin >> h[i];
    for (int i = 0; i < N - 1; i++) cin >> v[i];
    
    auto inb = [&](int i, int j) { return 0 <= i && i < N && 0 <= j && j < N; };
    auto can_move = [&](int i, int j, int ni, int nj) {
        if (!inb(ni, nj)) return false;
        if (ni == i && nj == j + 1) { // Right
            return h[i][j] == '0';
        } else if (ni == i && nj == j - 1) { // Left
            return h[i][j - 1] == '0';
        } else if (ni == i + 1 && nj == j) { // Down
            return v[i][j] == '0';
        } else if (ni == i - 1 && nj == j) { // Up
            return v[i - 1][j] == '0';
        }
        return false;
    };
    
    // BFS
    vector<int> par(N * N, -1);
    vector<char> mv(N * N, '?');
    vector<char> used(N * N, 0);
    queue<int> q;
    int s = si * N + sj, t = ti * N + tj;
    used[s] = 1;
    q.push(s);
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == t) break;
        int i = cur / N, j = cur % N;
        // Order: U, D, L, R
        const int di[4] = {-1, 1, 0, 0};
        const int dj[4] = {0, 0, -1, 1};
        const char dc[4] = {'U', 'D', 'L', 'R'};
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d], nj = j + dj[d];
            if (!inb(ni, nj)) continue;
            if (!can_move(i, j, ni, nj)) continue;
            int nxt = ni * N + nj;
            if (!used[nxt]) {
                used[nxt] = 1;
                par[nxt] = cur;
                mv[nxt] = dc[d];
                q.push(nxt);
            }
        }
    }
    
    string path;
    if (par[t] != -1 || s == t) {
        int cur = t;
        while (cur != s) {
            path.push_back(mv[cur]);
            cur = par[cur];
            if (cur == -1) { // Just in case (shouldn't happen with guarantees)
                path.clear();
                break;
            }
        }
        reverse(path.begin(), path.end());
    } else {
        // Fallback: simple greedy towards target (will be blocked by walls but valid output)
        int i = si, j = sj;
        while (i < ti && (int)path.size() < 200) { path.push_back('D'); i++; }
        while (j < tj && (int)path.size() < 200) { path.push_back('R'); j++; }
        while (i > ti && (int)path.size() < 200) { path.push_back('U'); i--; }
        while (j > tj && (int)path.size() < 200) { path.push_back('L'); j--; }
    }
    
    // Extend by repeating the path to fill up to 200 (extra chars are ignored after arrival)
    string ans = path;
    while ((int)ans.size() < 200 && !path.empty()) {
        int add = min((int)path.size(), 200 - (int)ans.size());
        ans += path.substr(0, add);
    }
    if (ans.empty()) ans = ""; // Ensure some output even if path was empty (already at target)
    if ((int)ans.size() > 200) ans.resize(200);
    
    cout << ans << '\n';
    return 0;
}