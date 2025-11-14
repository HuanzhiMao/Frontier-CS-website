#include <bits/stdc++.h>
using namespace std;

int sgn(int x) { return x > 0 ? 1 : (x < 0 ? -1 : 0); }

bool try_axis(int px, int py, vector<vector<bool>>& dot, vector<vector<bool>>& used_hor, vector<vector<bool>>& used_vert, vector<vector<int>>& ops, int N) {
  for (int ox = 0; ox < N; ox++) if (ox != px) {
    for (int oy = 0; oy < N; oy++) if (oy != py) {
      if (!dot[px][oy] || !dot[ox][py] || !dot[ox][oy]) continue;
      int minx = min(px, ox), maxx = max(px, ox);
      int miny = min(py, oy), maxy = max(py, oy);
      if (minx == maxx || miny == maxy) continue;
      bool valid = true;
      for (int k = minx + 1; k < maxx; k++) {
        if (dot[k][miny] || dot[k][maxy]) valid = false;
      }
      for (int k = miny + 1; k < maxy; k++) {
        if (dot[minx][k] || dot[maxx][k]) valid = false;
      }
      if (!valid) continue;
      for (int k = minx; k < maxx; k++) {
        if (used_hor[miny][k] || used_hor[maxy][k]) valid = false;
      }
      for (int k = miny; k < maxy; k++) {
        if (used_vert[minx][k] || used_vert[maxx][k]) valid = false;
      }
      if (!valid) continue;
      dot[px][py] = true;
      for (int k = minx; k < maxx; k++) {
        used_hor[miny][k] = true;
        used_hor[maxy][k] = true;
      }
      for (int k = miny; k < maxy; k++) {
        used_vert[minx][k] = true;
        used_vert[maxx][k] = true;
      }
      ops.push_back({px, py, px, oy, ox, oy, ox, py});
      return true;
    }
  }
  return false;
}

bool try_45(int px, int py, vector<vector<bool>>& dot, vector<vector<bool>>& used_pos, vector<vector<bool>>& used_neg, vector<vector<int>>& ops, int N, int offset) {
  int maxd = N - 1;
  for (int a = -maxd; a <= maxd; a++) if (a != 0) {
    for (int b = -maxd; b <= maxd; b++) if (b != 0) {
      int qx = px + a, qy = py + a;
      int sx = px + b, sy = py - b;
      int rx = px + a + b, ry = py + a - b;
      if (qx < 0 || qx >= N || qy < 0 || qy >= N) continue;
      if (sx < 0 || sx >= N || sy < 0 || sy >= N) continue;
      if (rx < 0 || rx >= N || ry < 0 || ry >= N) continue;
      if (!dot[qx][qy] || !dot[sx][sy] || !dot[rx][ry]) continue;
      vector<pair<pair<int, int>, pair<int, int>>> edges = {
        {{px, py}, {qx, qy}},
        {{qx, qy}, {rx, ry}},
        {{rx, ry}, {sx, sy}},
        {{sx, sy}, {px, py}}
      };
      bool valid = true;
      for (auto& e : edges) {
        int x1 = e.first.first, y1 = e.first.second;
        int x2 = e.second.first, y2 = e.second.second;
        int dx = x2 - x1, dy = y2 - y1;
        int ad = abs(dx);
        if (ad != abs(dy) || ad == 0) { valid = false; break; }
        int sx = sgn(dx), sy = sgn(dy);
        for (int k = 1; k < ad; k++) {
          int xx = x1 + k * sx;
          int yy = y1 + k * sy;
          if (dot[xx][yy]) { valid = false; break; }
        }
        if (!valid) break;
      }
      if (!valid) continue;
      for (auto& e : edges) {
        int x1 = e.first.first, y1 = e.first.second;
        int x2 = e.second.first, y2 = e.second.second;
        int dx = x2 - x1, dy = y2 - y1;
        bool is_pos = (sgn(dx) == sgn(dy) && dx != 0);
        bool is_neg = (sgn(dx) == -sgn(dy) && dx != 0);
        int minx = min(x1, x2), maxx = max(x1, x2);
        for (int k = minx; k < maxx; k++) {
          if (is_pos) {
            int diff = y1 - x1;
            int idx = diff + offset;
            if (used_pos[idx][k]) { valid = false; break; }
          } else if (is_neg) {
            int sm = x1 + y1;
            if (used_neg[sm][k]) { valid = false; break; }
          }
        }
        if (!valid) break;
      }
      if (!valid) continue;
      dot[px][py] = true;
      for (auto& e : edges) {
        int x1 = e.first.first, y1 = e.first.second;
        int x2 = e.second.first, y2 = e.second.second;
        int dx = x2 - x1, dy = y2 - y1;
        bool is_pos = (sgn(dx) == sgn(dy) && dx != 0);
        bool is_neg = (sgn(dx) == -sgn(dy) && dx != 0);
        int minx = min(x1, x2), maxx = max(x1, x2);
        for (int k = minx; k < maxx; k++) {
          if (is_pos) {
            int diff = y1 - x1;
            int idx = diff + offset;
            used_pos[idx][k] = true;
          } else if (is_neg) {
            int sm = x1 + y1;
            used_neg[sm][k] = true;
          }
        }
      }
      ops.push_back({px, py, qx, qy, rx, ry, sx, sy});
      return true;
    }
  }
  return false;
}

int main() {
  int N, M;
  cin >> N >> M;
  vector<vector<bool>> dot(N, vector<bool>(N, false));
  for (int i = 0; i < M; i++) {
    int x, y;
    cin >> x >> y;
    dot[x][y] = true;
  }
  int c = (N - 1) / 2;
  vector<vector<long long>> w(N, vector<long long>(N));
  for (int x = 0; x < N; x++) {
    for (int y = 0; y < N; y++) {
      w[x][y] = (long long)(x - c) * (x - c) + (long long)(y - c) * (y - c) + 1;
    }
  }
  vector<pair<long long, pair<int, int>>> cands;
  for (int x = 0; x < N; x++) {
    for (int y = 0; y < N; y++) {
      if (!dot[x][y]) {
        cands.push_back({-w[x][y], {x, y}});
      }
    }
  }
  sort(cands.begin(), cands.end());
  vector<vector<bool>> used_hor(N, vector<bool>(N - 1, false));
  vector<vector<bool>> used_vert(N, vector<bool>(N - 1, false));
  int maxd = N - 1;
  int offset = maxd;
  vector<vector<bool>> used_pos(2 * N - 1, vector<bool>(N - 1, false));
  vector<vector<bool>> used_neg(2 * N - 1, vector<bool>(N - 1, false));
  vector<vector<int>> ops;
  int passes = 0;
  while (true) {
    bool added = false;
    for (auto& cand : cands) {
      int px = cand.second.first, py = cand.second.second;
      if (dot[px][py]) continue;
      bool success = try_axis(px, py, dot, used_hor, used_vert, ops, N);
      if (success) {
        added = true;
        continue;
      }
      success = try_45(px, py, dot, used_pos, used_neg, ops, N, offset);
      if (success) {
        added = true;
      }
    }
    if (!added) break;
    passes++;
    if (passes > 50) break;
  }
  cout << ops.size() << endl;
  for (auto& op : ops) {
    for (int v : op) cout << v << " ";
    cout << endl;
  }
  return 0;
}