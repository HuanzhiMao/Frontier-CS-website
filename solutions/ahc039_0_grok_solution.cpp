#include <bits/stdc++.h>
using namespace std;

struct Point {
  int x, y;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int N;
  cin >> N;
  vector<Point> mack(N), sard(N);
  for (int i = 0; i < N; i++) cin >> mack[i].x >> mack[i].y;
  for (int i = 0; i < N; i++) cin >> sard[i].x >> sard[i].y;
  map<int, set<int>> x_ys;
  for (auto& p : mack) x_ys[p.x].insert(p.y);
  for (auto& p : sard) x_ys[p.x].insert(p.y);
  set<int> uniq_x, uniq_y;
  for (auto& p : mack) {
    uniq_x.insert(p.x);
    uniq_y.insert(p.y);
  }
  for (auto& p : sard) {
    uniq_x.insert(p.x);
    uniq_y.insert(p.y);
  }
  vector<int> X(uniq_x.begin(), uniq_x.end());
  vector<int> Y(uniq_y.begin(), uniq_y.end());
  int M = X.size();
  int K = Y.size();
  auto get_x_rank = [&](int val) -> int {
    return lower_bound(X.begin(), X.end(), val) - X.begin() + 1;
  };
  auto get_y_rank = [&](int val) -> int {
    return lower_bound(Y.begin(), Y.end(), val) - Y.begin() + 1;
  };
  vector<vector<short>> sum_m(M + 2, vector<short>(K + 2, 0));
  vector<vector<short>> sum_s(M + 2, vector<short>(K + 2, 0));
  for (auto& p : mack) {
    int rx = get_x_rank(p.x);
    int ry = get_y_rank(p.y);
    sum_m[rx][ry]++;
  }
  for (auto& p : sard) {
    int rx = get_x_rank(p.x);
    int ry = get_y_rank(p.y);
    sum_s[rx][ry]++;
  }
  for (int i = 1; i <= M; i++) {
    for (int j = 1; j <= K; j++) {
      sum_m[i][j] += sum_m[i - 1][j] + sum_m[i][j - 1] - sum_m[i - 1][j - 1];
      sum_s[i][j] += sum_s[i - 1][j] + sum_s[i][j - 1] - sum_s[i - 1][j - 1];
    }
  }
  auto count_points = [&](const vector<vector<short>>& sum, int xl, int xr, int yl, int yr, const vector<int>& XX, const vector<int>& YY) -> int {
    if (xl > xr || yl > yr) return 0;
    auto it = lower_bound(XX.begin(), XX.end(), xl);
    int l_idx = it - XX.begin();
    int l_rank = l_idx + 1;
    auto ut = upper_bound(XX.begin(), XX.end(), xr);
    int u_idx = ut - XX.begin();
    int r_rank = u_idx;
    if (l_rank > r_rank) return 0;
    auto jt = lower_bound(YY.begin(), YY.end(), yl);
    int ly_idx = jt - YY.begin();
    int ly_rank = ly_idx + 1;
    auto kt = upper_bound(YY.begin(), YY.end(), yr);
    int uy_idx = kt - YY.begin();
    int ry_rank = uy_idx;
    if (ly_rank > ry_rank) return 0;
    int res = sum[r_rank][ry_rank] - (ly_rank - 1 >= 0 ? sum[r_rank][ly_rank - 1] : 0) - (l_rank - 1 >= 0 ? sum[l_rank - 1][ry_rank] : 0) + (l_rank - 1 >= 0 && ly_rank - 1 >= 0 ? sum[l_rank - 1][ly_rank - 1] : 0);
    return res;
  };
  vector<int> parent(N);
  for (int i = 0; i < N; i++) parent[i] = i;
  function<int(int)> find = [&](int x) -> int {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
  };
  long long D2 = 20000LL * 20000;
  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      long long dx = mack[i].x - mack[j].x;
      long long dy = mack[i].y - mack[j].y;
      if (dx * dx + dy * dy <= D2) {
        int pi = find(i), pj = find(j);
        if (pi != pj) parent[pi] = pj;
      }
    }
  }
  map<int, vector<int>> groups;
  for (int i = 0; i < N; i++) {
    groups[find(i)].push_back(i);
  }
  int best_value = -100000000;
  int best_lx = 0, best_rx = 0, best_ly = 0, best_ry = 0;
  for (auto& pr : groups) {
    auto& comp = pr.second;
    if (comp.empty()) continue;
    int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
    for (int id : comp) {
      minx = min(minx, mack[id].x);
      maxx = max(maxx, mack[id].x);
      miny = min(miny, mack[id].y);
      maxy = max(maxy, mack[id].y);
    }
    int lx = minx, rx = maxx, ly = miny, ry = maxy;
    if (lx == rx && rx < 100000) rx++;
    else if (lx == rx && lx > 0) lx--;
    if (ly == ry && ry < 100000) ry++;
    else if (ly == ry && ly > 0) ly--;
    if (lx >= rx || ly >= ry) continue;
    int a = count_points(sum_m, lx, rx, ly, ry, X, Y);
    int b = count_points(sum_s, lx, rx, ly, ry, X, Y);
    int val = a - b;
    if (val > best_value) {
      best_value = val;
      best_lx = lx;
      best_rx = rx;
      best_ly = ly;
      best_ry = ry;
    }
  }
  bool use_empty = (best_value + 1 <= 0);
  int elx = 0, erx = 0, ely = 0, ery = 0;
  bool found = false;
  if (use_empty) {
    for (int xx = 0; xx < 100000; xx++) {
      if (x_ys.find(xx) == x_ys.end() && x_ys.find(xx + 1) == x_ys.end()) {
        elx = xx;
        erx = xx + 1;
        ely = 0;
        ery = 1;
        found = true;
        break;
      }
    }
    if (!found) {
      for (int yy = 0; yy < 100000; yy++) {
        bool good = true;
        if (x_ys[0].count(yy) || x_ys[0].count(yy + 1)) good = false;
        if (x_ys[1].count(yy) || x_ys[1].count(yy + 1)) good = false;
        if (good) {
          elx = 0;
          erx = 1;
          ely = yy;
          ery = yy + 1;
          found = true;
          break;
        }
      }
    }
    assert(found);
    cout << 4 << '\n';
    cout << elx << ' ' << ely << '\n';
    cout << erx << ' ' << ely << '\n';
    cout << erx << ' ' << ery << '\n';
    cout << elx << ' ' << ery << '\n';
  } else {
    cout << 4 << '\n';
    cout << best_lx << ' ' << best_ly << '\n';
    cout << best_rx << ' ' << best_ly << '\n';
    cout << best_rx << ' ' << best_ry << '\n';
    cout << best_lx << ' ' << best_ry << '\n';
  }
  return 0;
}