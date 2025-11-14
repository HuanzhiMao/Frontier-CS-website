#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  vector<string> strs(M);
  for (int i = 0; i < M; i++) cin >> strs[i];

  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

  pair<int, int> best_res = {-1, -1}; // placed, dots
  vector<vector<char>> best_grid;

  // Function to try placement
  auto try_place = [&](vector<string> order) -> pair<int, pair<int, vector<vector<char>>>> {
    vector<vector<char>> g(N, vector<char>(N, '.'));
    int placed = 0;
    for (const auto& s : order) {
      int k = s.size();
      int min_new = INT_MAX;
      tuple<int, int, int> best = {-1, -1, -1}; // type, line, start
      for (int tp = 0; tp < 2; tp++) {
        for (int ln = 0; ln < N; ln++) {
          for (int st = 0; st < N; st++) {
            bool can = true;
            int nw = 0;
            for (int p = 0; p < k; p++) {
              int r = (tp == 0 ? ln : (st + p) % N);
              int c = (tp == 0 ? (st + p) % N : ln);
              char cur = g[r][c];
              if (cur != '.' && cur != s[p]) {
                can = false;
                break;
              }
              if (cur == '.') nw++;
            }
            if (can && nw < min_new) {
              min_new = nw;
              best = {tp, ln, st};
            }
          }
        }
      }
      if (min_new != INT_MAX) {
        auto [tp, ln, st] = best;
        for (int p = 0; p < k; p++) {
          int r = (tp == 0 ? ln : (st + p) % N);
          int c = (tp == 0 ? (st + p) % N : ln);
          g[r][c] = s[p];
        }
        placed++;
      }
    }
    int dots = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (g[i][j] == '.') dots++;
    return {placed, {dots, g}};
  };

  // Sorted attempt
  vector<string> sorted_strs = strs;
  sort(sorted_strs.begin(), sorted_strs.end(), [](const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return a < b;
  });
  auto res = try_place(sorted_strs);
  int pl = res.first;
  int dt = res.second.first;
  if (pl > best_res.first || (pl == best_res.first && dt > best_res.second)) {
    best_res = {pl, dt};
    best_grid = res.second.second;
  }

  // Random attempts
  for (int att = 0; att < 100; att++) {
    vector<string> shuffled = strs;
    shuffle(shuffled.begin(), shuffled.end(), rng);
    res = try_place(shuffled);
    pl = res.first;
    dt = res.second.first;
    if (pl > best_res.first || (pl == best_res.first && dt > best_res.second)) {
      best_res = {pl, dt};
      best_grid = res.second.second;
    }
  }

  // Output the best
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cout << best_grid[i][j];
    }
    cout << endl;
  }
  return 0;
}