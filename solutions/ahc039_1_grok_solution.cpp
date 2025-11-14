#include <bits/stdc++.h>
using namespace std;

int main() {
  int N;
  cin >> N;
  vector<pair<int, int>> mack(N), sard(N);
  for (int i = 0; i < N; i++) cin >> mack[i].first >> mack[i].second;
  for (int i = 0; i < N; i++) cin >> sard[i].first >> sard[i].second;
  set<pair<int, int>> allp;
  for (auto p : mack) allp.insert(p);
  for (auto p : sard) allp.insert(p);
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  int K = 17;
  vector<pair<double, double>> centers(K);
  set<int> chos;
  while (chos.size() < K) {
    int idx = rng() % N;
    if (chos.count(idx) == 0) chos.insert(idx);
  }
  vector<int> chosen(chos.begin(), chos.end());
  for (int i = 0; i < K; i++) {
    centers[i] = {double(mack[chosen[i]].first), double(mack[chosen[i]].second)};
  }
  vector<int> assign(N);
  for (int iter = 0; iter < 30; iter++) {
    for (int i = 0; i < N; i++) {
      double mind = 1e18;
      int best = 0;
      double px = mack[i].first, py = mack[i].second;
      for (int j = 0; j < K; j++) {
        double dx = px - centers[j].first;
        double dy = py - centers[j].second;
        double d = dx * dx + dy * dy;
        if (d < mind) {
          mind = d;
          best = j;
        }
      }
      assign[i] = best;
    }
    vector<double> sx(K, 0), sy(K, 0);
    vector<int> cnt(K, 0);
    for (int i = 0; i < N; i++) {
      int j = assign[i];
      sx[j] += mack[i].first;
      sy[j] += mack[i].second;
      cnt[j]++;
    }
    for (int j = 0; j < K; j++) {
      if (cnt[j] == 0) {
        int idx = rng() % N;
        centers[j] = {double(mack[idx].first), double(mack[idx].second)};
      } else {
        centers[j].first = sx[j] / cnt[j];
        centers[j].second = sy[j] / cnt[j];
      }
    }
  }
  vector<vector<int>> clus(K);
  for (int i = 0; i < N; i++) {
    clus[assign[i]].push_back(i);
  }
  int bestval = INT_MIN;
  vector<pair<int, int>> bestverts;
  for (int j = 0; j < K; j++) {
    if (clus[j].empty()) continue;
    int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
    for (int ii : clus[j]) {
      int xx = mack[ii].first, yy = mack[ii].second;
      minx = min(minx, xx);
      maxx = max(maxx, xx);
      miny = min(miny, yy);
      maxy = max(maxy, yy);
    }
    if (minx == maxx) {
      if (maxx < 100000) {
        maxx++;
      } else if (minx > 0) {
        minx--;
      } else
        continue;
    }
    if (miny == maxy) {
      if (maxy < 100000) {
        maxy++;
      } else if (miny > 0) {
        miny--;
      } else
        continue;
    }
    int aa = 0;
    for (auto& p : mack) {
      int xx = p.first, yy = p.second;
      if (xx >= minx && xx <= maxx && yy >= miny && yy <= maxy) aa++;
    }
    int bb = 0;
    for (auto& p : sard) {
      int xx = p.first, yy = p.second;
      if (xx >= minx && xx <= maxx && yy >= miny && yy <= maxy) bb++;
    }
    int val = aa - bb;
    if (val > bestval) {
      bestval = val;
      vector<pair<int, int>> verts = {{minx, miny}, {maxx, miny}, {maxx, maxy}, {minx, maxy}};
      bestverts = verts;
    }
  }
  // global
  {
    int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
    bool has = false;
    for (auto& p : mack) {
      has = true;
      minx = min(minx, p.first);
      maxx = max(maxx, p.first);
      miny = min(miny, p.second);
      maxy = max(maxy, p.second);
    }
    if (has) {
      if (minx == maxx) {
        if (maxx < 100000)
          maxx++;
        else if (minx > 0)
          minx--;
        else
          goto next;
      }
      if (miny == maxy) {
        if (maxy < 100000)
          maxy++;
        else if (miny > 0)
          miny--;
        else
          goto next;
      }
      int aa = 0;
      for (auto& p : mack) {
        int xx = p.first, yy = p.second;
        if (xx >= minx && xx <= maxx && yy >= miny && yy <= maxy) aa++;
      }
      int bb = 0;
      for (auto& p : sard) {
        int xx = p.first, yy = p.second;
        if (xx >= minx && xx <= maxx && yy >= miny && yy <= maxy) bb++;
      }
      int val = aa - bb;
      if (val > bestval) {
        bestval = val;
        bestverts = {{minx, miny}, {maxx, miny}, {maxx, maxy}, {minx, maxy}};
      }
    }
  next:;
  }
  // singles
  vector<pair<int, int>> dirdelta = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (int i = 0; i < N; i++) {
    int px = mack[i].first, py = mack[i].second;
    bool found = false;
    vector<pair<int, int>> verts;
    for (auto [ddx, ddy] : dirdelta) {
      int nx = px + ddx;
      int ny = py + ddy;
      if (nx < 0 || nx > 100000 || ny < 0 || ny > 100000) continue;
      bool ok = allp.count({nx, py}) == 0 && allp.count({px, ny}) == 0 && allp.count({nx, ny}) == 0;
      if (ok) {
        int x1 = min(px, nx);
        int y1 = min(py, ny);
        int x2 = max(px, nx);
        int y2 = max(py, ny);
        verts = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
        found = true;
        break;
      }
    }
    if (found) {
      int val = 1 - 0;
      if (val > bestval) {
        bestval = val;
        bestverts = verts;
      }
    }
  }
  if (bestverts.empty()) {
    auto [px, py] = mack[0];
    int nx = px + 1;
    int ny = py + 1;
    if (nx > 100000) nx = px - 1;
    if (ny > 100000) ny = py - 1;
    int x1 = min(px, nx), x2 = max(px, nx);
    int y1 = min(py, ny), y2 = max(py, ny);
    bestverts = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
  }
  cout << bestverts.size() << endl;
  for (auto p : bestverts) {
    cout << p.first << " " << p.second << endl;
  }
}