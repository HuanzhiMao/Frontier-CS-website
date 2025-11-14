#include <bits/stdc++.h>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> stacks(m);
  for (int i = 0; i < m; i++) {
    stacks[i].resize(n / m);
    for (int &x : stacks[i]) cin >> x;
  }
  vector<pair<int, int>> ops;
  int current = 1;
  while (current <= n) {
    int s = -1, pos = -1;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < stacks[i].size(); j++) {
        if (stacks[i][j] == current) {
          s = i;
          pos = j;
          goto nxt;
        }
      }
    }
  nxt:;
    while ((int)stacks[s].size() > pos + 1) {
      int h = stacks[s].size() - (pos + 1);
      int best_len = 1;
      for (int l = 2; l <= h; l++) {
        int start = h - l;
        bool dec = true;
        int mn = INT_MAX, mx = INT_MIN;
        for (int jj = start; jj < h; jj++) {
          int val = stacks[s][pos + 1 + jj];
          mn = min(mn, val);
          mx = max(mx, val);
          if (jj < h - 1 && val <= stacks[s][pos + 1 + jj + 1]) dec = false;
        }
        if (dec && (mx - mn + 1 == l)) best_len = l;
      }
      int bottom_idx = pos + 1 + (h - best_len);
      int v_move = stacks[s][bottom_idx];
      int max_top = -1;
      int best_d = -1;
      for (int d = 0; d < m; d++) {
        if (d == s) continue;
        int t = stacks[d].empty() ? 2000000000 : stacks[d].back();
        if (t > max_top || (t == max_top && d < best_d)) {
          max_top = t;
          best_d = d;
        }
      }
      ops.push_back({v_move, best_d + 1});
      vector<int> moved;
      for (int j = bottom_idx; j < (int)stacks[s].size(); j++) {
        moved.push_back(stacks[s][j]);
      }
      stacks[s].resize(bottom_idx);
      for (int x : moved) {
        stacks[best_d].push_back(x);
      }
    }
    assert(stacks[s].back() == current);
    ops.push_back({current, 0});
    stacks[s].pop_back();
    current++;
  }
  for (auto [v, i] : ops) {
    cout << v << " " << i << "\n";
  }
  return 0;
}