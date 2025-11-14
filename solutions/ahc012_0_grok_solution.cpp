#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, K;
  cin >> N >> K;
  vector<int> a(11);
  for (int d = 1; d <= 10; d++) cin >> a[d];
  vector<long long> X(N), Y(N);
  for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];
  vector<long long> fact(11, 1);
  for (int i = 1; i <= 10; i++) fact[i] = fact[i - 1] * i;
  int best_k = 0;
  double max_sum = 0;
  for (int kk = 0; kk <= 100; kk++) {
    long long R = (long long)kk * (kk + 1) / 2 + 1;
    double s = 0;
    if (R == 1) {
      if (N >= 1 && N <= 10) s = min((double)a[N], 1.0);
    } else {
      double lam = (double)N / R;
      for (int d = 1; d <= 10; d++) {
        double p = exp(-lam) * pow(lam, d) / fact[d];
        double bd = R * p;
        s += min((double)a[d], bd);
      }
    }
    if (s > max_sum) {
      max_sum = s;
      best_k = kk;
    }
  }
  vector<array<long long, 4>> lines;
  const long long L = 100000;
  srand(time(0));
  int attempts = 0;
  const int MAX_ATT = 1000000;
  while (lines.size() < best_k && attempts < MAX_ATT) {
    attempts++;
    long long px = (rand() % (2 * L + 1)) - L;
    long long py = (rand() % (2 * L + 1)) - L;
    long long qx = (rand() % (2 * L + 1)) - L;
    long long qy = (rand() % (2 * L + 1)) - L;
    if (px == qx && py == qy) continue;
    long long dx = qx - px, dy = qy - py;
    bool hits = false;
    for (int i = 0; i < N; i++) {
      long long xx = X[i] - px, yy = Y[i] - py;
      if (dx * yy == dy * xx) {
        hits = true;
        break;
      }
    }
    if (hits) continue;
    bool par = false;
    for (auto &ln : lines) {
      long long dx2 = ln[2] - ln[0], dy2 = ln[3] - ln[1];
      if (dx * dy2 == dy * dx2) {
        par = true;
        break;
      }
    }
    if (par) continue;
    bool conc = false;
    int ns = lines.size();
    for (int i = 0; i < ns; i++) {
      for (int j = i + 1; j < ns; j++) {
        long long ax = lines[i][0], ay = lines[i][1], bx = lines[i][2], by = lines[i][3];
        long long dx1 = bx - ax, dy1 = by - ay;
        long long cx = lines[j][0], cy = lines[j][1], ex = lines[j][2], ey = lines[j][3];
        long long dx2 = ex - cx, dy2 = ey - cy;
        long long det = dx1 * dy2 - dy1 * dx2;
        if (det == 0) continue;
        long long dett = -(cx - ax) * dy2 + (cy - ay) * dx2;
        long long rx = px, ry = py, sx = qx, sy = qy;
        long long numx = ax * det + dett * dx1 - rx * det;
        long long numy = ay * det + dett * dy1 - ry * det;
        if (numx * (sy - ry) == numy * (sx - rx)) {
          conc = true;
          break;
        }
      }
      if (conc) break;
    }
    if (conc) continue;
    lines.push_back({px, py, qx, qy});
  }
  cout << lines.size() << endl;
  for (auto &ln : lines) {
    cout << ln[0] << " " << ln[1] << " " << ln[2] << " " << ln[3] << endl;
  }
}