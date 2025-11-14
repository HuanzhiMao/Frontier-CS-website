#include <bits/stdc++.h>

using namespace std;

int main() {
  int N;
  cin >> N;
  vector<vector<int>> h(N, vector<int>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> h[i][j];
    }
  }
  vector<vector<bool>> visited(N, vector<bool>(N, false));
  int ri = 0, rj = 0;
  long long current_load = 0;
  vector<string> operations;
  while (true) {
    int best_dist = INT_MAX;
    int best_i = -1, best_j = -1;
    int best_h = INT_MIN;
    for (int x = 0; x < N; x++) {
      for (int y = 0; y < N; y++) {
        if (h[x][y] != 0 && !visited[x][y]) {
          int hh = h[x][y];
          bool can = (hh >= 0) || (current_load >= (long long)-hh);
          if (can) {
            int d = abs(x - ri) + abs(y - rj);
            if (d < best_dist || (d == best_dist && (hh > best_h || (hh == best_h && (x < best_i || (x == best_i && y < best_j)))))) {
              best_dist = d;
              best_i = x;
              best_j = y;
              best_h = hh;
            }
          }
        }
      }
    }
    if (best_i == -1) break;
    int dx = best_i - ri;
    int dy = best_j - rj;
    string vdir = (dx > 0 ? "D" : "U");
    int vabs = abs(dx);
    for (int k = 0; k < vabs; k++) {
      operations.push_back(vdir);
    }
    string hdir = (dy > 0 ? "R" : "L");
    int habs = abs(dy);
    for (int k = 0; k < habs; k++) {
      operations.push_back(hdir);
    }
    ri = best_i;
    rj = best_j;
    int hh = h[ri][rj];
    string op = (hh > 0 ? "+" : "-") + to_string(abs(hh));
    operations.push_back(op);
    current_load += hh;
    visited[ri][rj] = true;
  }
  for (auto& s : operations) {
    cout << s << endl;
  }
  return 0;
}