#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  vector<pair<int, int>> pos(M);
  for (int k = 0; k < M; k++) {
    cin >> pos[k].first >> pos[k].second;
  }
  vector<pair<char, char>> acts;
  int ci = pos[0].first;
  int cj = pos[0].second;
  for (int k = 1; k < M; k++) {
    int ti = pos[k].first;
    int tj = pos[k].second;
    // change row
    if (ci != ti) {
      int delta = abs(ti - ci);
      int s = 1 + min(ti, N - 1 - ti);
      if (s < delta) {
        // use slide
        int cost_u = ti;
        int cost_d = N - 1 - ti;
        if (cost_u <= cost_d) {
          acts.emplace_back('S', 'U');
          for (int z = 0; z < ti; z++) {
            acts.emplace_back('M', 'D');
          }
        } else {
          acts.emplace_back('S', 'D');
          for (int z = 0; z < N - 1 - ti; z++) {
            acts.emplace_back('M', 'U');
          }
        }
      } else {
        // direct
        char dir = (ti > ci) ? 'D' : 'U';
        for (int z = 0; z < delta; z++) {
          acts.emplace_back('M', dir);
        }
      }
    }
    ci = ti;
    // change col
    if (cj != tj) {
      int delta = abs(tj - cj);
      int s = 1 + min(tj, N - 1 - tj);
      if (s < delta) {
        // use slide
        int cost_l = tj;
        int cost_r = N - 1 - tj;
        if (cost_l <= cost_r) {
          acts.emplace_back('S', 'L');
          for (int z = 0; z < tj; z++) {
            acts.emplace_back('M', 'R');
          }
        } else {
          acts.emplace_back('S', 'R');
          for (int z = 0; z < N - 1 - tj; z++) {
            acts.emplace_back('M', 'L');
          }
        }
      } else {
        char dir = (tj > cj) ? 'R' : 'L';
        for (int z = 0; z < delta; z++) {
          acts.emplace_back('M', dir);
        }
      }
    }
    cj = tj;
  }
  for (auto [a, d] : acts) {
    cout << a << " " << d << endl;
  }
  return 0;
}