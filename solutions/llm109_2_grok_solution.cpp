#include <bits/stdc++.h>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int N;
  cin >> N;
  int r0, c0;
  cin >> r0 >> c0;
  auto idx = [N](int r, int c) { return (r - 1) * N + (c - 1); };
  vector<char> visited(N * N, 0);
  vector<pair<int, int>> path;
  path.push_back({r0, c0});
  visited[idx(r0, c0)] = 1;
  int cur_r = r0, cur_c = c0;
  int dr[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
  int dc[8] = {2, 1, -1, -2, -2, -1, 1, 2};
  double center_r = (N + 1) / 2.0;
  double center_c = (N + 1) / 2.0;
  while (true) {
    int min_deg = 9;
    double best_dist = -1;
    int best_d = -1;
    for (int d = 0; d < 8; d++) {
      int nr = cur_r + dr[d];
      int nc = cur_c + dc[d];
      if (nr < 1 || nr > N || nc < 1 || nc > N || visited[idx(nr, nc)]) continue;
      int deg = 0;
      for (int dd = 0; dd < 8; dd++) {
        int nnr = nr + dr[dd];
        int nnc = nc + dc[dd];
        if (nnr >= 1 && nnr <= N && nnc >= 1 && nnc <= N && !visited[idx(nnr, nnc)]) deg++;
      }
      double dist = (nr - center_r) * (nr - center_r) + (nc - center_c) * (nc - center_c);
      bool better = false;
      if (deg < min_deg) {
        better = true;
      } else if (deg == min_deg) {
        if (dist > best_dist) {
          better = true;
        } else if (dist == best_dist && d < best_d) {
          better = true;
        }
      }
      if (better || best_d == -1) {
        min_deg = deg;
        best_dist = dist;
        best_d = d;
      }
    }
    if (best_d == -1) break;
    cur_r += dr[best_d];
    cur_c += dc[best_d];
    visited[idx(cur_r, cur_c)] = 1;
    path.push_back({cur_r, cur_c});
  }
  cout << path.size() << "\n";
  for (auto [r, c] : path) {
    cout << r << " " << c << "\n";
  }
  return 0;
}