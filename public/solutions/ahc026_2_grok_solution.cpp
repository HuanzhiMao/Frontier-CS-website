#include <bits/stdc++.h>

using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> stacks(m + 1);
  vector<int> pos_stk(n + 1), pos_idx(n + 1);
  int height = n / m;
  for (int i = 1; i <= m; i++) {
    stacks[i].resize(height);
    for (int j = 0; j < height; j++) {
      cin >> stacks[i][j];
      pos_stk[stacks[i][j]] = i;
      pos_idx[stacks[i][j]] = j;
    }
  }
  vector<pair<int, int>> ops;
  for (int next = 1; next <= n; next++) {
    while (true) {
      int s = pos_stk[next];
      int p = pos_idx[next];
      int h = stacks[s].size();
      if (p == h - 1) {
        ops.push_back({next, 0});
        stacks[s].pop_back();
        pos_stk[next] = 0;
        break;
      } else {
        int v = stacks[s][p + 1];
        int best_i = -1;
        int best_top = -1;
        int empty_i = -1;
        for (int cand = 1; cand <= m; cand++) {
          if (cand == s) continue;
          if (stacks[cand].empty()) {
            if (empty_i == -1 || cand < empty_i) empty_i = cand;
          } else {
            int t = stacks[cand].back();
            if (t > best_top || (t == best_top && cand < best_i)) {
              best_top = t;
              best_i = cand;
            }
          }
        }
        int target = (empty_i != -1) ? empty_i : best_i;
        ops.push_back({v, target});
        vector<int> moved;
        for (int idx = p + 1; idx < h; idx++) {
          moved.push_back(stacks[s][idx]);
        }
        stacks[s].resize(p + 1);
        int target_oldh = stacks[target].size();
        for (int k = 0; k < moved.size(); k++) {
          int box = moved[k];
          stacks[target].push_back(box);
          pos_stk[box] = target;
          pos_idx[box] = target_oldh + k;
        }
      }
    }
  }
  for (auto pr : ops) {
    cout << pr.first << " " << pr.second << endl;
  }
  return 0;
}