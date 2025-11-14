#include <bits/stdc++.h>

using namespace std;

int main() {
  int N;
  cin >> N;
  vector<vector<int>> h(N, vector<int>(N));
  for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) cin >> h[i][j];
  vector<pair<int, int>> order_;
  set<pair<int, int>> unproc;
  for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) unproc.insert({i, j});
  int curr_load_ = 0;
  int curr_r = 0, curr_c = 0;
  while (!unproc.empty()) {
    vector<pair<int, int>> candidates;
    for (auto p : unproc) {
      int i = p.first, j = p.second;
      if (h[i][j] >= 0 || curr_load_ >= -h[i][j]) {
        candidates.push_back(p);
      }
    }
    if (candidates.empty()) {
      assert(false);
    }
    int max_h = -101;
    for (auto p : candidates) {
      int i = p.first, j = p.second;
      max_h = max(max_h, h[i][j]);
    }
    vector<pair<int, int>> best_cands;
    for (auto p : candidates) {
      int i = p.first, j = p.second;
      if (h[i][j] == max_h) best_cands.push_back(p);
    }
    int min_dist = INT_MAX;
    pair<int, int> chosen = {-1, -1};
    for (auto p : best_cands) {
      int i = p.first, j = p.second;
      int d = abs(i - curr_r) + abs(j - curr_c);
      if (d < min_dist || (d == min_dist && (i < chosen.first || (i == chosen.first && j < chosen.second)))) {
        min_dist = d;
        chosen = p;
      }
    }
    order_.push_back(chosen);
    unproc.erase(chosen);
    curr_load_ += h[chosen.first][chosen.second];
    assert(curr_load_ >= 0);
  }
  vector<string> ops;
  curr_r = 0; curr_c = 0;
  for (auto p : order_) {
    int tr = p.first, tc = p.second;
    int dr = tr - curr_r;
    string row_dir = (dr > 0 ? "D" : "U");
    for (int i = 0; i < abs(dr); i++) {
      ops.push_back(row_dir);
    }
    int dc = tc - curr_c;
    string col_dir = (dc > 0 ? "R" : "L");
    for (int i = 0; i < abs(dc); i++) {
      ops.push_back(col_dir);
    }
    curr_r = tr;
    curr_c = tc;
    int hh = h[tr][tc];
    if (hh != 0) {
      string sign = (hh > 0 ? "+" : "-");
      int amt = abs(hh);
      ops.push_back(sign + to_string(amt));
    }
  }
  for (auto s : ops) {
    cout << s << endl;
  }
  return 0;
}