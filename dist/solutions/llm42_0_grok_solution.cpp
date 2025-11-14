#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  cin >> n;
  double sq = sqrt(n);
  int k = ceil(sq);
  double L = k;
  vector<tuple<double, double, double>> pos;
  int col = 0, row = 0;
  for (int i = 0; i < n; i++) {
    double x = 0.5 + col * 1.0;
    double y = 0.5 + row * 1.0;
    pos.emplace_back(x, y, 0.0);
    col++;
    if (col == k) {
      col = 0;
      row++;
    }
  }
  printf("%.6f\n", L);
  for (auto [x, y, a] : pos) {
    printf("%.6f %.6f %.6f\n", x, y, a);
  }
  return 0;
}