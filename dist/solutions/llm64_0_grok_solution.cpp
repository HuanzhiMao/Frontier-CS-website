#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<pair<ll, int>> get_sums(const vector<ll>& v) {
  int m = v.size();
  vector<pair<ll, int>> res;
  res.reserve(1LL << m);
  res.push_back({0, 0});
  for (int i = 0; i < m; i++) {
    int sz = res.size();
    for (int j = 0; j < sz; j++) {
      ll news = res[j].first + v[i];
      int newm = res[j].second | (1 << i);
      res.push_back({news, newm});
    }
  }
  return res;
}

int main() {
  int n;
  ll T;
  cin >> n >> T;
  vector<ll> a(n);
  for (int i = 0; i < n; i++) cin >> a[i];
  int mid = n / 2;
  int h1 = mid, h2 = n - mid;
  if (max(h1, h2) <= 25) {
    vector<ll> left_a(a.begin(), a.begin() + mid);
    auto left = get_sums(left_a);
    sort(left.begin(), left.end());
    vector<ll> right_a(a.begin() + mid, a.end());
    auto right = get_sums(right_a);
    ll min_error = LLONG_MAX;
    int best_lmask = 0, best_rmask = 0;
    for (auto& p : right) {
      ll s2 = p.first;
      ll target = T - s2;
      auto it = lower_bound(left.begin(), left.end(), make_pair(target, -1));
      vector<pair<ll, int>> cands;
      if (it != left.end()) cands.push_back(*it);
      if (it != left.begin()) {
        --it;
        cands.push_back(*it);
      }
      for (auto& c : cands) {
        ll s1 = c.first;
        ll total = s1 + s2;
        ll err = abs(total - T);
        if (err < min_error) {
          min_error = err;
          best_lmask = c.second;
          best_rmask = p.second;
        }
      }
    }
    string s(n, '0');
    for (int i = 0; i < mid; i++) {
      if (best_lmask & (1 << i)) s[i] = '1';
    }
    for (int i = 0; i < n - mid; i++) {
      if (best_rmask & (1 << i)) s[mid + i] = '1';
    }
    cout << s << endl;
  } else {
    vector<pair<ll, int>> b(n);
    for (int i = 0; i < n; i++) b[i] = {a[i], i};
    sort(b.rbegin(), b.rend());
    ll cur_sum = 0;
    vector<int> selected(n, 0);
    for (auto& p : b) {
      if (cur_sum + p.first <= T) {
        cur_sum += p.first;
        selected[p.second] = 1;
      }
    }
    ll current_sum = cur_sum;
    ll current_error = abs(cur_sum - T);
    vector<int> best = selected;
    ll best_error = current_error;
    double temp = 1e10;
    double cooling = 0.999;
    int max_iter = 100000;
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int iter = 0; iter < max_iter; iter++) {
      double r = (double)rng() / numeric_limits<uint32_t>::max();
      ll delta = 0;
      vector<int> to_flip;
      if (r < 0.3) {
        int i = uniform_int_distribution<int>(0, n - 1)(rng);
        delta = selected[i] ? -a[i] : a[i];
        to_flip.push_back(i);
      } else {
        int i = uniform_int_distribution<int>(0, n - 1)(rng);
        int j = uniform_int_distribution<int>(0, n - 1)(rng);
        if (i == j) continue;
        delta = (selected[i] ? -a[i] : a[i]) + (selected[j] ? -a[j] : a[j]);
        to_flip.push_back(i);
        to_flip.push_back(j);
      }
      ll new_sum = current_sum + delta;
      ll new_error = abs(new_sum - T);
      double de = current_error - new_error;
      double prob = exp(de / temp);
      double rand_p = (double)rng() / numeric_limits<uint32_t>::max();
      if (prob > rand_p) {
        for (int idx : to_flip) selected[idx] ^= 1;
        current_sum = new_sum;
        current_error = new_error;
        if (current_error < best_error) {
          best = selected;
          best_error = current_error;
        }
      }
      temp *= cooling;
      if (best_error == 0) break;
    }
    string s = "";
    for (int i = 0; i < n; i++) s += '0' + best[i];
    cout << s << endl;
  }
  return 0;
}