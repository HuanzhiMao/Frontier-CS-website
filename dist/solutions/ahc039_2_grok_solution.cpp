#include <bits/stdc++.h>
using namespace std;

long long sumq(const vector<vector<int>>& ft, int x, int y) {
  long long res = 0;
  for (int i = x; i > 0; i -= i & -i) {
    for (int j = y; j > 0; j -= j & -j) {
      res += ft[i][j];
    }
  }
  return res;
}

void add(vector<vector<int>>& ft, int x, int y, int val) {
  int maxx = ft.size() - 1;
  int maxy = ft[0].size() - 1;
  for (int i = x; i <= maxx; i += i & -i) {
    for (int j = y; j <= maxy; j += j & -j) {
      ft[i][j] += val;
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  srand(time(NULL));
  int N;
  cin >> N;
  vector<pair<int, int>> mack(N), sard(N);
  set<pair<int, int>> allp;
  vector<int> allx, ally;
  for (int i = 0; i < N; i++) {
    cin >> mack[i].first >> mack[i].second;
    allp.insert(mack[i]);
    allx.push_back(mack[i].first);
    ally.push_back(mack[i].second);
  }
  for (int i = 0; i < N; i++) {
    cin >> sard[i].first >> sard[i].second;
    allp.insert(sard[i]);
    allx.push_back(sard[i].first);
    ally.push_back(sard[i].second);
  }
  sort(allx.begin(), allx.end());
  allx.erase(unique(allx.begin(), allx.end()), allx.end());
  sort(ally.begin(), ally.end());
  ally.erase(unique(ally.begin(), ally.end()), ally.end());
  int D = allx.size();
  int E = ally.size();
  vector<vector<int>> ft_m(D + 2, vector<int>(E + 2, 0));
  vector<vector<int>> ft_s(D + 2, vector<int>(E + 2, 0));
  map<int, int> xrank, yrank;
  for (int i = 0; i < D; i++) xrank[allx[i]] = i + 1;
  for (int i = 0; i < E; i++) yrank[ally[i]] = i + 1;
  for (auto p : mack) {
    add(ft_m, xrank[p.first], yrank[p.second], 1);
  }
  for (auto p : sard) {
    add(ft_s, xrank[p.first], yrank[p.second], 1);
  }
  auto get_l_r = [&](const vector<int>& uniq, int mn, int mx) -> pair<int, int> {
    auto it = lower_bound(uniq.begin(), uniq.end(), mn);
    int l = (it - uniq.begin()) + 1;
    auto it2 = upper_bound(uniq.begin(), uniq.end(), mx);
    int r = (it2 - uniq.begin());
    return {l, r};
  };
  auto query = [&](const vector<vector<int>>& ft, int minx, int maxx, int miny, int maxy) -> int {
    auto [lx, rx] = get_l_r(allx, minx, maxx);
    auto [ly, ry] = get_l_r(ally, miny, maxy);
    if (lx > rx || ly > ry) return 0;
    return sumq(ft, rx, ry) - sumq(ft, rx, ly - 1) - sumq(ft, lx - 1, ry) + sumq(ft, lx - 1, ly - 1);
  };
  long long best_sc = LLONG_MIN;
  int best_xl = 0, best_xr = 0, best_yl = 0, best_yr = 0;
  const int TRIALS = 1000000;
  for (int t = 0; t < TRIALS; t++) {
    int i = rand() % N;
    int j = rand() % N;
    if (i == j) continue;
    int xl = min(mack[i].first, mack[j].first);
    int xr = max(mack[i].first, mack[j].first);
    int yl = min(mack[i].second, mack[j].second);
    int yr = max(mack[i].second, mack[j].second);
    long long sc = (long long)query(ft_m, xl, xr, yl, yr) - query(ft_s, xl, xr, yl, yr);
    if (sc > best_sc) {
      best_sc = sc;
      best_xl = xl;
      best_xr = xr;
      best_yl = yl;
      best_yr = yr;
    }
  }
  vector<pair<int, int>> dirs = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (int i = 0; i < N; i++) {
    int x = mack[i].first;
    int y = mack[i].second;
    for (auto d : dirs) {
      int dx = d.first, dy = d.second;
      int nx = x + dx, ny = y + dy;
      if (nx < 0 || nx > 100000 || ny < 0 || ny > 100000) continue;
      int xl = min(x, nx), xr = max(x, nx);
      int yl = min(y, ny), yr = max(y, ny);
      long long sc = (long long)query(ft_m, xl, xr, yl, yr) - query(ft_s, xl, xr, yl, yr);
      if (sc > best_sc) {
        best_sc = sc;
        best_xl = xl;
        best_xr = xr;
        best_yl = yl;
        best_yr = yr;
      }
    }
  }
  long long cand_score = best_sc + 1;
  if (cand_score < 1) {
    int found = 0;
    int exl = 0, exr = 0, eyl = 0, eyr = 0;
    for (int tx = 0; tx <= 100000 && !found; tx += 10000) {
      for (int ty = 0; ty <= 100000 && !found; ty += 10000) {
        int xl = tx, xr = tx + 1, yl = ty, yr = ty + 1;
        if (xr > 100000 || yr > 100000) continue;
        int aa = query(ft_m, xl, xr, yl, yr);
        int bb = query(ft_s, xl, xr, yl, yr);
        if (aa == 0 && bb == 0) {
          found = 1;
          exl = xl;
          exr = xr;
          eyl = yl;
          eyr = yr;
        }
      }
    }
    if (!found) {
      for (int xl = 0; xl < 100000 && !found; xl++) {
        for (int yl = 0; yl < 100000 && !found; yl++) {
          int xr = xl + 1, yr = yl + 1;
          if (allp.count({xl, yl}) || allp.count({xl, yr}) || allp.count({xr, yl}) || allp.count({xr, yr})) continue;
          found = 1;
          exl = xl;
          exr = xr;
          eyl = yl;
          eyr = yr;
        }
      }
    }
    cout << 4 << "\n";
    cout << exl << " " << eyl << "\n";
    cout << exr << " " << eyl << "\n";
    cout << exr << " " << eyr << "\n";
    cout << exl << " " << eyr << "\n";
  } else {
    if (best_xl == best_xr) {
      if (best_xl > 0) best_xl--;
      else best_xr++;
    }
    if (best_yl == best_yr) {
      if (best_yl > 0) best_yl--;
      else best_yr++;
    }
    cout << 4 << "\n";
    cout << best_xl << " " << best_yl << "\n";
    cout << best_xr << " " << best_yl << "\n";
    cout << best_xr << " " << best_yr << "\n";
    cout << best_xl << " " << best_yr << "\n";
  }
  return 0;
}