#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, si, sj;
  cin >> N >> si >> sj;
  vector<string> grid(N);
  for(int i = 0; i < N; i++) cin >> grid[i];
  vector<vector<int>> parx(N, vector<int>(N, -1));
  vector<vector<int>> pary(N, vector<int>(N, -1));
  vector<vector<bool>> visited(N, vector<bool>(N, false));
  queue<pair<int, int>> q;
  q.push({si, sj});
  visited[si][sj] = true;
  parx[si][sj] = -2;
  pary[si][sj] = -2;
  while (!q.empty()) {
    auto [x, y] = q.front(); q.pop();
    int dxs[4] = {-1, 0, 1, 0};
    int dys[4] = {0, 1, 0, -1};
    for (int d = 0; d < 4; d++) {
      int nx = x + dxs[d];
      int ny = y + dys[d];
      if (nx >= 0 && nx < N && ny >= 0 && ny < N && grid[nx][ny] != '#' && !visited[nx][ny]) {
        visited[nx][ny] = true;
        parx[nx][ny] = x;
        pary[nx][ny] = y;
        q.push({nx, ny});
      }
    }
  }
  vector<pair<int, int>> tour;
  int dxs[4] = {-1, 0, 1, 0};
  int dys[4] = {0, 1, 0, -1};
  function<void(int, int, int, int)> dfs = [&](int x, int y, int px, int py) {
    tour.push_back({x, y});
    for (int d = 0; d < 4; d++) {
      int nx = x + dxs[d];
      int ny = y + dys[d];
      if (nx >= 0 && nx < N && ny >= 0 && ny < N && grid[nx][ny] != '#' && parx[nx][ny] == x && pary[nx][ny] == y) {
        dfs(nx, ny, x, y);
        tour.push_back({x, y});
      }
    }
  };
  dfs(si, sj, -1, -1);
  string ans;
  for (size_t k = 0; k < tour.size() - 1; k++) {
    int x1 = tour[k].first;
    int y1 = tour[k].second;
    int x2 = tour[k + 1].first;
    int y2 = tour[k + 1].second;
    int di = x2 - x1;
    int dj = y2 - y1;
    char move;
    if (di == -1 && dj == 0) move = 'U';
    else if (di == 1 && dj == 0) move = 'D';
    else if (di == 0 && dj == -1) move = 'L';
    else if (di == 0 && dj == 1) move = 'R';
    ans += move;
  }
  cout << ans << endl;
  return 0;
}