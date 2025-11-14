#include <bits/stdc++.h>
using namespace std;

int main() {
  int N;
  cin >> N;
  vector<string> board(N);
  for (auto& s : board) cin >> s;
  vector<vector<char>> grid(N, vector<char>(N));
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) grid[i][j] = board[i][j];
  vector<pair<char, int>> moves;

  auto shift_up = [&](int col) {
    for (int row = 1; row < N; row++) {
      grid[row - 1][col] = grid[row][col];
    }
    grid[N - 1][col] = '.';
  };
  auto shift_down = [&](int col) {
    for (int row = N - 1; row >= 1; row--) {
      grid[row][col] = grid[row - 1][col];
    }
    grid[0][col] = '.';
  };
  auto shift_left = [&](int row) {
    for (int j = 1; j < N; j++) {
      grid[row][j - 1] = grid[row][j];
    }
    grid[row][N - 1] = '.';
  };
  auto shift_right = [&](int row) {
    for (int j = N - 1; j >= 1; j--) {
      grid[row][j] = grid[row - 1][j];
    }
    grid[row][0] = '.';
  };

  while (true) {
    vector<pair<int, int>> onis;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (grid[i][j] == 'x') onis.emplace_back(i, j);
      }
    }
    if (onis.empty()) break;
    int best_num = -1;
    int best_dist = INT_MAX;
    int best_idx = -1;
    int best_d = -1;
    for (int idx = 0; idx < onis.size(); idx++) {
      int r = onis[idx].first, c = onis[idx].second;
      string dirs = "UDLR";
      for (int dd = 0; dd < 4; dd++) {
        char dir = dirs[dd];
        bool can = true;
        int this_dist = 0;
        int this_num = 0;
        if (dir == 'U') {
          this_dist = r + 1;
          for (int i = 0; i <= r; i++) {
            char ch = grid[i][c];
            if (ch == 'o') can = false;
            if (ch == 'x') this_num++;
          }
        } else if (dir == 'D') {
          this_dist = N - r;
          for (int i = r; i < N; i++) {
            char ch = grid[i][c];
            if (ch == 'o') can = false;
            if (ch == 'x') this_num++;
          }
        } else if (dir == 'L') {
          this_dist = c + 1;
          for (int j = 0; j <= c; j++) {
            char ch = grid[r][j];
            if (ch == 'o') can = false;
            if (ch == 'x') this_num++;
          }
        } else if (dir == 'R') {
          this_dist = N - c;
          for (int j = c; j < N; j++) {
            char ch = grid[r][j];
            if (ch == 'o') can = false;
            if (ch == 'x') this_num++;
          }
        }
        if (can && this_num > 0 && (this_num > best_num || (this_num == best_num && this_dist < best_dist))) {
          best_num = this_num;
          best_dist = this_dist;
          best_idx = idx;
          best_d = dd;
        }
      }
    }
    int r = onis[best_idx].first;
    int c = onis[best_idx].second;
    char shift_c = "UDLR"[best_d];
    char opp_c;
    if (shift_c == 'U') opp_c = 'D';
    else if (shift_c == 'D') opp_c = 'U';
    else if (shift_c == 'L') opp_c = 'R';
    else opp_c = 'L';
    int pp = (shift_c == 'U' || shift_c == 'D') ? c : r;
    for (int k = 0; k < best_dist; k++) {
      moves.emplace_back(shift_c, pp);
      if (shift_c == 'U') shift_up(pp);
      else if (shift_c == 'D') shift_down(pp);
      else if (shift_c == 'L') shift_left(pp);
      else if (shift_c == 'R') shift_right(pp);
    }
    for (int k = 0; k < best_dist; k++) {
      moves.emplace_back(opp_c, pp);
      if (opp_c == 'U') shift_up(pp);
      else if (opp_c == 'D') shift_down(pp);
      else if (opp_c == 'L') shift_left(pp);
      else if (opp_c == 'R') shift_right(pp);
    }
  }
  for (auto [d, p] : moves) {
    cout << d << " " << p << "\n";
  }
}