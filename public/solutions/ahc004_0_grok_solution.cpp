#include <bits/stdc++.h>
using namespace std;

struct Place {
  int dir, line, start;
};

bool covers(int x, int y, const Place& pl, int k, int N) {
  if (pl.dir == 0) {
    if (pl.line != x) return false;
    for (int p = 0; p < k; p++) {
      if ((pl.start + p) % N == y) return true;
    }
    return false;
  } else {
    if (pl.line != y) return false;
    for (int p = 0; p < k; p++) {
      if ((pl.start + p) % N == x) return true;
    }
    return false;
  }
}

int main() {
  int N, M;
  cin >> N >> M;
  vector<string> strs(M);
  for (int i = 0; i < M; i++) cin >> strs[i];
  vector<int> order(M);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) { return strs[a].size() > strs[b].size(); });
  const int TRIES = 30;
  char best_grid[20][20];
  int best_c = 0;
  int best_filled = 1e9;
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  for (int t = 0; t < TRIES; t++) {
    char g[20][20];
    memset(g, '.', sizeof(g));
    for (int oi = 0; oi < M; oi++) {
      int m = order[oi];
      string s = strs[m];
      int k = s.size();
      bool covered = false;
      for (int i = 0; i < N && !covered; i++) {
        for (int j = 0; j < N; j++) {
          bool match = true;
          for (int p = 0; p < k; p++) {
            if (g[i][(j + p) % N] != s[p]) {
              match = false;
              break;
            }
          }
          if (match) {
            covered = true;
            break;
          }
        }
      }
      for (int j = 0; j < N && !covered; j++) {
        for (int i = 0; i < N; i++) {
          bool match = true;
          for (int p = 0; p < k; p++) {
            if (g[(i + p) % N][j] != s[p]) {
              match = false;
              break;
            }
          }
          if (match) {
            covered = true;
            break;
          }
        }
      }
      if (covered) continue;
      vector<tuple<int, int, int, int>> candidates;
      int min_new = 1e9;
      for (int i = 0; i < N; i++) {
        for (int st = 0; st < N; st++) {
          bool conf = false;
          int nw = 0;
          for (int p = 0; p < k; p++) {
            int col = (st + p) % N;
            char curr = g[i][col];
            if (curr != '.' && curr != s[p]) {
              conf = true;
              break;
            }
            if (curr == '.') nw++;
          }
          if (!conf) {
            candidates.emplace_back(nw, 0, i, st);
            min_new = min(min_new, nw);
          }
        }
      }
      for (int j = 0; j < N; j++) {
        for (int st = 0; st < N; st++) {
          bool conf = false;
          int nw = 0;
          for (int p = 0; p < k; p++) {
            int row = (st + p) % N;
            char curr = g[row][j];
            if (curr != '.' && curr != s[p]) {
              conf = true;
              break;
            }
            if (curr == '.') nw++;
          }
          if (!conf) {
            candidates.emplace_back(nw, 1, j, st);
            min_new = min(min_new, nw);
          }
        }
      }
      if (candidates.empty()) continue;
      vector<tuple<int, int, int, int>> best_cand;
      for (auto& cand : candidates) {
        if (get<0>(cand) == min_new) best_cand.push_back(cand);
      }
      uniform_int_distribution<int> dist(0, best_cand.size() - 1);
      auto chosen = best_cand[dist(rng)];
      int ch_nw, ch_dir, ch_line, ch_st;
      tie(ch_nw, ch_dir, ch_line, ch_st) = chosen;
      if (ch_dir == 0) {
        int i = ch_line;
        for (int p = 0; p < k; p++) {
          int col = (ch_st + p) % N;
          g[i][col] = s[p];
        }
      } else {
        int j = ch_line;
        for (int p = 0; p < k; p++) {
          int row = (ch_st + p) % N;
          g[row][j] = s[p];
        }
      }
    }
    int this_filled = 0;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (g[i][j] != '.') this_filled++;
    int this_c = 0;
    for (int m = 0; m < M; m++) {
      string s = strs[m];
      int k = s.size();
      bool cov = false;
      for (int i = 0; i < N && !cov; i++) {
        for (int j = 0; j < N; j++) {
          bool mt = true;
          for (int p = 0; p < k; p++) {
            if (g[i][(j + p) % N] != s[p]) {
              mt = false;
              break;
            }
          }
          if (mt) {
            cov = true;
            break;
          }
        }
      }
      for (int j = 0; j < N && !cov; j++) {
        for (int i = 0; i < N; i++) {
          bool mt = true;
          for (int p = 0; p < k; p++) {
            if (g[(i + p) % N][j] != s[p]) {
              mt = false;
              break;
            }
          }
          if (mt) {
            cov = true;
            break;
          }
        }
      }
      if (cov) this_c++;
    }
    if (this_c > best_c || (this_c == best_c && this_filled < best_filled)) {
      best_c = this_c;
      best_filled = this_filled;
      memcpy(best_grid, g, sizeof(g));
    }
  }
  char grid[20][20];
  memcpy(grid, best_grid, sizeof(grid));
  if (best_c == M) {
    vector<vector<Place>> matching(M);
    for (int m = 0; m < M; m++) {
      string s = strs[m];
      int k = s.size();
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          bool mt = true;
          for (int p = 0; p < k; p++) {
            if (grid[i][(j + p) % N] != s[p]) {
              mt = false;
              break;
            }
          }
          if (mt) matching[m].push_back({0, i, j});
        }
      }
      for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
          bool mt = true;
          for (int p = 0; p < k; p++) {
            if (grid[(i + p) % N][j] != s[p]) {
              mt = false;
              break;
            }
          }
          if (mt) matching[m].push_back({1, j, i});
        }
      }
    }
    vector<pair<int, int>> filled_list;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (grid[i][j] != '.') filled_list.push_back({i, j});
    shuffle(filled_list.begin(), filled_list.end(), rng);
    for (auto& p : filled_list) {
      int x = p.first, y = p.second;
      if (grid[x][y] == '.') continue;
      bool can = true;
      for (int m = 0; m < M; m++) {
        bool has_alt = false;
        const auto& lst = matching[m];
        for (const auto& pl : lst) {
          bool covs = covers(x, y, pl, strs[m].size(), N);
          if (!covs) {
            has_alt = true;
            break;
          }
        }
        if (!has_alt) {
          can = false;
          break;
        }
      }
      if (can) {
        grid[x][y] = '.';
        for (int m = 0; m < M; m++) {
          vector<Place> newl;
          for (const auto& pl : matching[m]) {
            bool covs = covers(x, y, pl, strs[m].size(), N);
            if (!covs) {
              newl.push_back(pl);
            }
          }
          matching[m] = move(newl);
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cout << grid[i][j];
    }
    cout << endl;
  }
  return 0;
}