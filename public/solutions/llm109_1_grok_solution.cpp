#include <bits/stdc++.h>

using namespace std;

int main() {
  int N;
  cin >> N;
  int r0, c0;
  cin >> r0 >> c0;
  int M = N * N;
  vector<vector<int>> adj(M);
  vector<pair<int, int>> deltas = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  auto getidx = [&](int r, int c) { return (r - 1) * N + (c - 1); };
  auto valid = [&](int r, int c) { return r >= 1 && r <= N && c >= 1 && c <= N; };
  for (int r = 1; r <= N; r++) {
    for (int c = 1; c <= N; c++) {
      int u = getidx(r, c);
      for (auto [dr, dc] : deltas) {
        int nr = r + dr, nc = c + dc;
        if (valid(nr, nc)) {
          int v = getidx(nr, nc);
          adj[u].push_back(v);
        }
      }
    }
  }
  vector<bool> vis(M, false);
  vector<int> path;
  int current = getidx(r0, c0);
  vis[current] = true;
  path.push_back(current);
  bool extended = true;
  while (extended) {
    extended = false;
    int best = -1;
    int min_deg = 9;
    for (int nei : adj[current]) {
      if (vis[nei]) continue;
      int deg = 0;
      for (int nn : adj[nei]) if (!vis[nn]) deg++;
      if (deg < min_deg) {
        min_deg = deg;
        best = nei;
      } else if (deg == min_deg && nei > best) {
        best = nei;
      }
    }
    if (best != -1) {
      extended = true;
      path.push_back(best);
      vis[best] = true;
      current = best;
    }
  }
  int l = path.size();
  cout << l << endl;
  for (int idx : path) {
    int r = idx / N + 1;
    int c = idx % N + 1;
    cout << r << " " << c << endl;
  }
  return 0;
}