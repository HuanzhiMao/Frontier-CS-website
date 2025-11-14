#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  vector<pair<int, int>> pos(M);
  for (int k = 0; k < M; k++) {
    cin >> pos[k].first >> pos[k].second;
  }
  vector<string> actions;
  int x = pos[0].first, y = pos[0].second;
  for (int k = 1; k < M; k++) {
    int tx = pos[k].first, ty = pos[k].second;
    int dx = tx - x;
    int dy = ty - y;
    bool can_v = false;
    char vdir = ' ';
    if (dx > 0 && tx == N - 1) {
      can_v = true;
      vdir = 'D';
    } else if (dx < 0 && tx == 0) {
      can_v = true;
      vdir = 'U';
    }
    bool can_h = false;
    char hdir = ' ';
    if (dy > 0 && ty == N - 1) {
      can_h = true;
      hdir = 'R';
    } else if (dy < 0 && ty == 0) {
      can_h = true;
      hdir = 'L';
    }
    int adx = abs(dx);
    int ady = abs(dy);
    if (!can_v && !can_h) {
      char vd = (dx > 0) ? 'D' : 'U';
      int cnt = adx;
      for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + vd);
      char hd = (dy > 0) ? 'R' : 'L';
      cnt = ady;
      for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + hd);
    } else if (can_v && can_h) {
      if (adx >= ady) {
        char hd = (dy > 0) ? 'R' : 'L';
        int cnt = ady;
        for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + hd);
        actions.push_back(string(1, 'S') + " " + vdir);
      } else {
        char vd = (dx > 0) ? 'D' : 'U';
        int cnt = adx;
        for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + vd);
        actions.push_back(string(1, 'S') + " " + hdir);
      }
    } else if (can_v) {
      char hd = (dy > 0) ? 'R' : 'L';
      int cnt = ady;
      for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + hd);
      actions.push_back(string(1, 'S') + " " + vdir);
    } else {
      char vd = (dx > 0) ? 'D' : 'U';
      int cnt = adx;
      for (int i = 0; i < cnt; i++) actions.push_back(string(1, 'M') + " " + vd);
      actions.push_back(string(1, 'S') + " " + hdir);
    }
    x = tx;
    y = ty;
  }
  for (auto& s : actions) {
    cout << s << endl;
  }
  return 0;
}