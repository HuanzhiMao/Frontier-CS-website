#include <bits/stdc++.h>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  cin >> n;
  int r, c;
  cin >> r >> c;
  r--; c--;
  vector<vector<int>> vis(n, vector<int>(n, 0));
  vector<pair<int, int>> path;
  int dx[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
  int dy[8] = {2, 1, -1, -2, -2, -1, 1, 2};
  int cur_r = r;
  int cur_c = c;
  vis[cur_r][cur_c] = 1;
  path.push_back({cur_r, cur_c});
  while (true) {
    vector<tuple<int, int, int, int>> candidates;
    for (int i = 0; i < 8; i++) {
      int nr = cur_r + dx[i];
      int nc = cur_c + dy[i];
      if (nr >= 0 && nr < n && nc >= 0 && nc < n && vis[nr][nc] == 0) {
        int deg = 0;
        for (int j = 0; j < 8; j++) {
          int nnr = nr + dx[j];
          int nnc = nc + dy[j];
          if (nnr >= 0 && nnr < n && nnc >= 0 && nnc < n && vis[nnr][nnc] == 0) deg++;
        }
        candidates.emplace_back(deg, i, nr, nc);
      }
    }
    if (candidates.empty()) break;
    sort(candidates.begin(), candidates.end());
    auto [deg, midx, nr, nc] = candidates[0];
    vis[nr][nc] = 1;
    path.push_back({nr, nc});
    cur_r = nr;
    cur_c = nc;
  }
  cout << path.size() << '\n';
  for (auto [pr, pc] : path) {
    cout << (pr + 1) << ' ' << (pc + 1) << '\n';
  }
  return 0;
}