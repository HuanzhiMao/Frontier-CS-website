#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  long long T;
  cin >> n >> T;
  vector<long long> a(n);
  for (auto &x : a) cin >> x;
  vector<pair<long long, int>> p(n);
  for (int i = 0; i < n; i++) p[i] = {a[i], i};
  sort(p.rbegin(), p.rend());
  struct State {
    long long sum;
    bitset<100> mask;
  };
  const int W = 10000;
  vector<State> beam = {{0, bitset<100>()}};
  for (int j = 0; j < n; j++) {
    long long val = p[j].first;
    int idx = p[j].second;
    vector<State> newb;
    for (const auto& st : beam) {
      newb.push_back(st);
      State nst = st;
      nst.sum += val;
      if (idx < 100) nst.mask.set(idx);
      newb.push_back(nst);
    }
    sort(newb.begin(), newb.end(), [&](const State& x, const State& y) {
      long long ex = abs(x.sum - T);
      long long ey = abs(y.sum - T);
      if (ex != ey) return ex < ey;
      return x.sum < y.sum;
    });
    if (newb.size() > W) newb.resize(W);
    beam = std::move(newb);
  }
  auto& best = beam[0];
  string s(n, '0');
  for (int i = 0; i < n; i++) {
    if (best.mask.test(i)) s[i] = '1';
  }
  cout << s << endl;
}