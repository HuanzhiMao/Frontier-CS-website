#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

ull xy2d(ull n, ull x, ull y) {
  ull d = 0;
  for (ull s = n / 2; s; s /= 2) {
    ull rx = (x & s) > 0;
    ull ry = (y & s) > 0;
    d = d * 4 + (rx * 3 ^ ry);
    if (ry == 0) {
      if (rx) {
        x = n - 1 - x;
        y = n - 1 - y;
      }
      swap(x, y);
    }
  }
  return d;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int N;
  cin >> N;
  vector<ll> X(N), Y(N);
  for (int i = 0; i < N; i++) {
    cin >> X[i] >> Y[i];
  }
  vector<char> prime(N, 0);
  if (N > 2) prime[2] = 1;
  for (int i = 3; i < N; i += 2) prime[i] = 1;
  for (int i = 3; i * i < N; i += 2) {
    if (prime[i]) {
      for (int j = i * i; j < N; j += i * 2) prime[j] = 0;
    }
  }
  const ll SHIFT = 1000000001LL;
  const ull NN = 1ULL << 31;
  vector<pair<ull, int>> v(N);
  for (int i = 0; i < N; i++) {
    ull ux = X[i] + SHIFT;
    ull uy = Y[i] + SHIFT;
    v[i] = {xy2d(NN, ux, uy), i};
  }
  sort(v.begin(), v.end());
  int j = -1;
  for (int i = 0; i < N; i++) if (v[i].second == 0) { j = i; break; }
  vector<int> path(N + 1);
  path[0] = 0;
  for (int k = 1; k < N; k++) {
    int idx = (j + k) % N;
    path[k] = v[idx].second;
  }
  path[N] = 0;
  vector<int> path_rev(N + 1);
  path_rev[0] = 0;
  for (int k = 1; k < N; k++) {
    int idx = ((j - k) % N + N) % N;
    path_rev[k] = v[idx].second;
  }
  path_rev[N] = 0;
  auto compute_L = [&](const vector<int>& P) -> double {
    double sum = 0;
    for (int t = 1; t <= N; t++) {
      int a = P[t - 1];
      int b = P[t];
      ll dx = X[a] - X[b];
      ll dy = Y[a] - Y[b];
      double d = sqrt(dx * dx + dy * dy);
      double m = 1.0;
      if (t % 10 == 0 && !prime[a]) m = 1.1;
      sum += m * d;
    }
    return sum;
  };
  double L1 = compute_L(path);
  double L2 = compute_L(path_rev);
  const vector<int>& best = (L1 < L2) ? path : path_rev;
  cout << N + 1 << "\n";
  for (int p : best) cout << p << "\n";
  return 0;
}