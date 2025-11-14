#include <bits/stdc++.h>
using namespace std;

int N, si, sj;
vector<string> grid;
vector<vector<char>> visited;
string ans;

int di[4] = {-1, 1, 0, 0};
int dj[4] = {0, 0, -1, 1};
char mv[4] = {'U', 'D', 'L', 'R'};
char rv[4] = {'D', 'U', 'R', 'L'};

void dfs(int i, int j) {
    for (int k = 0; k < 4; ++k) {
        int ni = i + di[k], nj = j + dj[k];
        if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
        if (grid[ni][nj] == '#') continue;
        if (visited[ni][nj]) continue;
        visited[ni][nj] = 1;
        ans.push_back(mv[k]);
        dfs(ni, nj);
        ans.push_back(rv[k]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> si >> sj)) return 0;
    grid.resize(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    visited.assign(N, vector<char>(N, 0));
    visited[si][sj] = 1;
    dfs(si, sj);

    cout << ans << '\n';
    return 0;
}