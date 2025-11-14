#include <bits/stdc++.h>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> st(m);
  for (int i = 0; i < m; i++) {
    st[i].resize(n / m);
    for (int j = 0; j < n / m; j++) {
      cin >> st[i][j];
    }
  }
  int next = 1;
  vector<pair<int, int>> ops;
  while (next <= n) {
    int top_s = -1;
    for (int i = 0; i < m; i++) {
      if (!st[i].empty() && st[i].back() == next) {
        top_s = i;
        break;
      }
    }
    if (top_s != -1) {
      ops.push_back({next, 0});
      st[top_s].pop_back();
      next++;
      continue;
    }
    int s = -1, idx = -1;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < st[i].size(); j++) {
        if (st[i][j] == next) {
          s = i;
          idx = j;
          break;
        }
      }
      if (s != -1) break;
    }
    vector<int> empties;
    int max_t = -1;
    int best_d = -1;
    for (int i = 0; i < m; i++) {
      if (i == s) continue;
      if (st[i].empty()) {
        empties.push_back(i);
      } else {
        int t = st[i].back();
        if (t > max_t) {
          max_t = t;
          best_d = i;
        } else if (t == max_t && i < best_d) {
          best_d = i;
        }
      }
    }
    int d;
    if (!empties.empty()) {
      sort(empties.begin(), empties.end());
      d = empties[0];
    } else {
      d = best_d;
    }
    int v = st[s][idx + 1];
    ops.push_back({v, d + 1});
    for (int j = idx + 1; j < st[s].size(); j++) {
      st[d].push_back(st[s][j]);
    }
    st[s].resize(idx + 1);
  }
  for (auto [v, i] : ops) {
    cout << v << " " << i << "\n";
  }
  return 0;
}