#include <bits/stdc++.h>
using namespace std;

int main() {
  int T;
  cin >> T;
  const int MAX_P = 1000;
  vector<long long> primes;
  vector<bool> is_prime(MAX_P + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (long long i = 2; i <= MAX_P; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
      for (long long j = i * i; j <= MAX_P; j += i) is_prime[j] = false;
    }
  }
  vector<vector<long long>> groups;
  vector<long long> current;
  long long prod = 1;
  for (auto p : primes) {
    if (prod > 1000000000000000000LL / p) {
      groups.push_back(current);
      current.clear();
      prod = 1;
    }
    current.push_back(p);
    prod *= p;
  }
  if (!current.empty()) groups.push_back(current);
  for (int t = 0; t < T; t++) {
    set<long long> found_primes;
    for (auto& group : groups) {
      long long Q = 1;
      for (auto p : group) {
        Q *= p;
      }
      cout << "0 " << Q << endl;
      cout.flush();
      long long g;
      cin >> g;
      if (g == 1) continue;
      for (auto p : group) {
        if (g % p == 0) {
          found_primes.insert(p);
          g /= p;
        }
      }
    }
    map<long long, int> exponents;
    for (auto p : found_primes) {
      int low = 1, high = 60;
      int v = 0;
      while (low <= high) {
        int mid = (low + high) / 2;
        long long pow = 1;
        bool over = false;
        for (int i = 1; i <= mid; i++) {
          if (pow > 1000000000000000000LL / p) {
            over = true;
            break;
          }
          pow *= p;
        }
        long long Q;
        if (over) {
          high = mid - 1;
          continue;
        } else {
          Q = pow;
        }
        cout << "0 " << Q << endl;
        cout.flush();
        long long g;
        cin >> g;
        if (g == Q) {
          v = mid;
          low = mid + 1;
        } else {
          high = mid - 1;
        }
      }
      exponents[p] = v;
    }
    long long d_small = 1;
    if (!exponents.empty()) {
      for (auto& pr : exponents) {
        d_small *= (pr.second + 1LL);
      }
    }
    long long ans = d_small * 2;
    cout << "1 " << ans << endl;
    cout.flush();
  }
  return 0;
}