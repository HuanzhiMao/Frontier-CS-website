#include <bits/stdc++.h>
using namespace std;

bool can_move(int r, int c, char dir, const vector<vector<int>>& h, const vector<vector<int>>& v) {
  int dr = 0, dc = 0;
  if (dir == 'U') dr = -1;
  if (dir == 'D') dr = 1;
  if (dir == 'L') dc = -1;
  if (dir == 'R') dc = 1;
  int nr = r + dr, nc = c + dc;
  if (nr < 0 || nr > 19 || nc < 0 || nc > 19) return false;
  if (dir == 'R') return h[r][c] == 0;
  if (dir == 'L') return h[r][nc] == 0;
  if (dir == 'D') return v[r][c] == 0;
  if (dir == 'U') return v[nr][c] == 0;
  return false;
}

double compute_E(const string& S, int si, int sj, int ti, int tj, double p, const vector<vector<int>>& h, const vector<vector<int>>& v, const vector<vector<int>>& dist_to_target) {
  int L = S.length();
  vector<vector<vector<double>>> dp(L + 2, vector<vector<double>>(20, vector<double>(20, 0.0)));
  dp[1][si][sj] = 1.0;
  double q = 1.0 - p;
  double expected = 0.0;
  for (int k = 1; k <= L; ++k) {
    for (int i = 0; i < 20; ++i) {
      for (int j = 0; j < 20; ++j) {
        double pr = dp[k][i][j];
        if (pr == 0.0) continue;
        if (i == ti && j == tj) continue;
        char dir = S[k - 1];
        // forget
        {
          int ni = i, nj = j;
          double contrib = p * pr;
          if (ni == ti && nj == tj) {
            expected += contrib * (401 - k);
          } else {
            dp[k + 1][ni][nj] += contrib;
          }
        }
        // not forget
        {
          double contrib = q * pr;
          int dr = 0, dc = 0;
          if (dir == 'U') dr = -1;
          if (dir == 'D') dr = 1;
          if (dir == 'L') dc = -1;
          if (dir == 'R') dc = 1;
          int ni = i + dr, nj = j + dc;
          bool can = (ni >= 0 && ni < 20 && nj >= 0 && nj < 20) && can_move(i, j, dir, h, v);
          if (!can) {
            ni = i;
            nj = j;
          }
          if (ni == ti && nj == tj) {
            expected += contrib * (401 - k);
          } else {
            dp[k + 1][ni][nj] += contrib;
          }
        }
      }
    }
  }
  if (expected == 0.0) {
    int cr = si, cc = sj;
    for (char d : S) {
      int dr = 0, dc = 0;
      if (d == 'U') dr = -1;
      if (d == 'D') dr = 1;
      if (d == 'L') dc = -1;
      if (d == 'R') dc = 1;
      int nr = cr + dr, nc = cc + dc;
      bool can = can_move(cr, cc, d, h, v);
      if (can) {
        cr = nr;
        cc = nc;
      }
    }
    double pot = -dist_to_target[cr][cc];
    expected += pot * 1e-9;
  }
  return expected;
}

int main() {
  int si, sj, ti, tj;
  double p;
  cin >> si >> sj >> ti >> tj >> p;
  vector<vector<int>> h(20, vector<int>(19));
  for (int i = 0; i < 20; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < 19; ++j) {
      h[i][j] = s[j] - '0';
    }
  }
  vector<vector<int>> v(19, vector<int>(20));
  for (int i = 0; i < 19; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < 20; ++j) {
      v[i][j] = s[j] - '0';
    }
  }
  vector<vector<int>> dist_to_target(20, vector<int>(20, -1));
  vector<vector<bool>> viss(20, vector<bool>(20, false));
  queue<pair<int, int>> qq;
  qq.push({ti, tj});
  viss[ti][tj] = true;
  dist_to_target[ti][tj] = 0;
  while (!qq.empty()) {
    auto [r, c] = qq.front();
    qq.pop();
    for (char d : "UDLR") {
      int dr = 0, dc = 0;
      if (d == 'U') dr = -1;
      if (d == 'D') dr = 1;
      if (d == 'L') dc = -1;
      if (d == 'R') dc = 1;
      int nr = r + dr, nc = c + dc;
      bool can = (nr >= 0 && nr < 20 && nc >= 0 && nc < 20) && can_move(r, c, d, h, v);
      if (can && !viss[nr][nc]) {
        viss[nr][nc] = true;
        dist_to_target[nr][nc] = dist_to_target[r][c] + 1;
        qq.push({nr, nc});
      }
    }
  }
  const int B = 20;
  vector<string> beam = {""};
  for (int len = 0; len < 200; ++len) {
    vector<pair<double, string>> cands;
    for (const auto& s : beam) {
      for (char d : "UDLR") {
        string ns = s + d;
        double e = compute_E(ns, si, sj, ti, tj, p, h, v, dist_to_target);
        cands.emplace_back(e, ns);
      }
    }
    sort(cands.rbegin(), cands.rend());
    beam.clear();
    int max_take = min(B, (int)cands.size());
    for (int i = 0; i < max_take; ++i) {
      beam.push_back(cands[i].second);
    }
  }
  double max_e = -1.0;
  string best;
  for (const auto& s : beam) {
    double e = compute_E(s, si, sj, ti, tj, p, h, v, dist_to_target);
    if (e > max_e) {
      max_e = e;
      best = s;
    }
  }
  cout << best << endl;
  return 0;
}