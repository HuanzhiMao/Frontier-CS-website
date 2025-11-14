#include <bits/stdc++.h>

using namespace std;

int main() {
  int G;
  cin >> G;
  int remaining_budget = 3000000;
  while (true) {
    string s;
    cin >> s;
    if (s == "-1") break;
    if (s == "SCORE") {
      double W;
      cin >> W;
      break;
    }
    if (s == "RESULT") {
      int delta;
      cin >> delta;
      continue;
    }
    if (s == "OPP") {
      string act;
      cin >> act;
      if (act == "CALL") {
        int x;
        cin >> x;
      }
      continue;
    }
    if (s == "STATE") {
      int h, r, a, b, P, k;
      cin >> h >> r >> a >> b >> P >> k;
      string al;
      cin >> al;
      int c1, v1, c2, v2;
      cin >> c1 >> v1 >> c2 >> v2;
      string bd;
      cin >> bd;
      vector<pair<int, int>> board;
      for (int i = 0; i < k; i++) {
        int c, v;
        cin >> c >> v;
        board.emplace_back(c, v);
      }
      int unk = 5 - k;
      int base_t = 15 + 15 * (4 - r) + 15 * unk;
      int t = min(base_t, remaining_budget);
      remaining_budget -= t;
      double e = 0.5;
      if (t >= 1) {
        cout << "RATE " << t << endl;
        string rt;
        cin >> rt;
        if (rt == "-1") return 0;
        double w, d;
        cin >> w >> d;
        e = w + d / 2.0;
      }
      string action = "CHECK";
      if (a >= 1) {
        double thresh = 0.65 - 0.05 * (r - 1);
        if (e > thresh) {
          double size_factor = 0.1 * r;
          int x = (int)(size_factor * P);
          x = max(1, x);
          x = min(x, a);
          action = "RAISE " + to_string(x);
        } else if (r == 1 && e < 0.4) {
          int x = 5;
          x = min(x, a);
          if (x >= 1) action = "RAISE " + to_string(x);
        }
      }
      cout << "ACTION " << action << endl;
    }
  }
  return 0;
}