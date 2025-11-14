#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, K;
  cin >> N >> K;
  vector<int> aa(11);
  for (int d = 1; d <= 10; d++) {
    cin >> aa[d];
  }
  for (int i = 0; i < N; i++) {
    int x, y;
    cin >> x >> y;
  }
  vector<double> fact(11, 1.0);
  for (int i = 1; i <= 10; i++) fact[i] = fact[i - 1] * i;
  double best = -1.0;
  int bestk = 0;
  for (int kk = 0; kk <= 100; kk++) {
    long long RR = 1LL + kk + kk * 1LL * (kk - 1) / 2;
    if (RR == 0) continue;
    double lam = double(N) / RR;
    double cursum = 0.0;
    for (int d = 1; d <= 10; d++) {
      double pd = pow(lam, d) / fact[d] * exp(-lam);
      double ebd = pd * RR;
      cursum += min(double(aa[d]), ebd);
    }
    if (cursum > best + 1e-9) {
      best = cursum;
      bestk = kk;
    }
  }
  cout << bestk << endl;
  for (int i = 1; i <= bestk; i++) {
    long long px = 0, py = (long long)i * i;
    long long qx = 1, qy = (long long)i * i + i;
    cout << px << " " << py << " " << qx << " " << qy << endl;
  }
  return 0;
}