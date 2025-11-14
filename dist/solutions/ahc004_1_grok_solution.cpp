#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  vector<string> S(M);
  for (int i = 0; i < M; i++) cin >> S[i];
  sort(S.begin(), S.end(), [](const string& a, const string& b) {
    return a.size() > b.size();
  });
  vector<vector<char>> grid(N, vector<char>(N, 0));
  int covered = 0;
  for (const string& s : S) {
    int k = s.size();
    int min_fills = INT_MAX;
    int best_dir = -1, best_fixed = -1, best_start = -1;
    for (int dir = 0; dir < 2; dir++) {
      for (int fixed = 0; fixed < N; fixed++) {
        for (int start = 0; start < N; start++) {
          bool can = true;
          int fills = 0;
          for (int p = 0; p < k; p++) {
            int pos = (start + p) % N;
            int r = (dir == 0) ? fixed : pos;
            int c = (dir == 0) ? pos : fixed;
            char curr = grid[r][c];
            char need = s[p];
            if (curr == 0) {
              fills++;
            } else if (curr != need) {
              can = false;
              break;
            }
          }
          if (can && fills < min_fills) {
            min_fills = fills;
            best_dir = dir;
            best_fixed = fixed;
            best_start = start;
          }
        }
      }
    }
    if (best_dir != -1) {
      covered++;
      for (int p = 0; p < k; p++) {
        int pos = (best_start + p) % N;
        int r = (best_dir == 0) ? best_fixed : pos;
        int c = (best_dir == 0) ? pos : best_fixed;
        if (grid[r][c] == 0) {
          grid[r][c] = s[p];
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (grid[i][j] == 0) {
        grid[i][j] = 'A';
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