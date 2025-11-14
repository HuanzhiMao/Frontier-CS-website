#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;
  int M = 1;
  while (M * 1ll * M * M < n) M++;
  vector<array<double, 3>> points(n);
  int index = 0;
  for (int i = 0; i < M && index < n; i++) {
    double x = (i + 0.5) / M;
    for (int j = 0; j < M && index < n; j++) {
      double y = (j + 0.5) / M;
      for (int k = 0; k < M && index < n; k++) {
        double z = (k + 0.5) / M;
        points[index] = {x, y, z};
        index++;
      }
    }
  }
  const int ITERS = 50;
  double step = 0.02;
  for (int iter = 0; iter < ITERS; iter++) {
    vector<array<double, 3>> forces(n, {0, 0, 0});
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
      double dx = points[i][0] - points[j][0];
      double dy = points[i][1] - points[j][1];
      double dz = points[i][2] - points[j][2];
      double dist2 = dx * dx + dy * dy + dz * dz;
      if (dist2 < 1e-8) dist2 = 1e-8;
      double f = 1.0 / dist2;
      dx *= f;
      dy *= f;
      dz *= f;
      forces[i][0] += dx;
      forces[i][1] += dy;
      forces[i][2] += dz;
      forces[j][0] -= dx;
      forces[j][1] -= dy;
      forces[j][2] -= dz;
    }
    for (int i = 0; i < n; i++) {
      for (int d = 0; d < 3; d++) {
        double x = points[i][d];
        forces[i][d] += 1.0 / (x * x + 1e-8) - 1.0 / ((1 - x) * (1 - x) + 1e-8);
      }
    }
    for (int i = 0; i < n; i++) {
      for (int d = 0; d < 3; d++) {
        double delta = forces[i][d] * step;
        points[i][d] += delta;
        if (points[i][d] < 0) points[i][d] = 0;
        if (points[i][d] > 1) points[i][d] = 1;
      }
    }
    step *= 0.95;
  }
  cout << fixed << setprecision(10);
  for (int i = 0; i < n; i++) {
    cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << "\n";
  }
  return 0;
}