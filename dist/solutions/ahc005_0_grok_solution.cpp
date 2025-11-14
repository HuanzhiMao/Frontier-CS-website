#include <bits/stdc++.h>

using namespace std;

int main() {
  int N, si, sj;
  cin >> N >> si >> sj;
  vector<string> grid(N);
  for(int i = 0; i < N; i++) cin >> grid[i];
  vector<vector<bool>> vis(N, vector<bool>(N, false));
  struct Pos {
    int x, y;
  };
  vector<Pos> pathh;
  auto dfs = [&](auto self, int x, int y) -> void {
    pathh.push_back({x, y});
    vis[x][y] = true;
    vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for(auto [dx, dy] : dirs) {
      int nx = x + dx, ny = y + dy;
      if(nx >= 0 && nx < N && ny >= 0 && ny < N && grid[nx][ny] != '#' && !vis[nx][ny]) {
        self(self, nx, ny);
        pathh.push_back({x, y});
      }
    }
  };
  dfs(dfs, si, sj);
  string ans;
  vector<char> dirchars = {'U', 'D', 'L', 'R'};
  vector<pair<int, int>> dird = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  for(size_t i = 0; i + 1 < pathh.size(); ++i) {
    int dx = pathh[i + 1].x - pathh[i].x;
    int dy = pathh[i + 1].y - pathh[i].y;
    bool found = false;
    for(int d = 0; d < 4; d++) {
      if(dird[d].first == dx && dird[d].second == dy) {
        ans += dirchars[d];
        found = true;
        break;
      }
    }
    assert(found);
  }
  cout << ans << endl;
  return 0;
}