#include <bits/stdc++.h>
using namespace std;

vector<long long> get_primes(int maxn) {
  vector<bool> is_prime(maxn + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i * i <= maxn; i++) {
    if (is_prime[i]) {
      for (int j = i * i; j <= maxn; j += i) {
        is_prime[j] = false;
      }
    }
  }
  vector<long long> primes;
  for (int i = 2; i <= maxn; i++) {
    if (is_prime[i]) primes.push_back(i);
  }
  return primes;
}

set<long long> factor(long long n) {
  set<long long> res;
  for (long long i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      res.insert(i);
      while (n % i == 0) n /= i;
    }
  }
  if (n > 1) res.insert(n);
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  auto primes = get_primes(1000);
  vector<vector<long long>> groups;
  vector<long long> curr;
  __int128 big_prod = 1;
  for (auto p : primes) {
    __int128 next = big_prod * (__int128)p;
    if (next > 1000000000000000000LL) {
      groups.push_back(curr);
      curr.clear();
      big_prod = 1;
    }
    curr.push_back(p);
    big_prod *= (__int128)p;
  }
  if (!curr.empty()) groups.push_back(curr);
  for (int t = 0; t < T; t++) {
    set<long long> dividing;
    for (auto& group : groups) {
      long long Q = 1;
      for (auto p : group) Q *= p;
      cout << "0 " << Q << endl;
      cout.flush();
      long long g;
      cin >> g;
      if (g > 1) {
        auto fac = factor(g);
        for (auto pf : fac) dividing.insert(pf);
      }
    }
    long long d = 1;
    for (auto p : dividing) {
      long long Q = 1;
      while (true) {
        __int128 next = (__int128)Q * p;
        if (next > 1000000000000000000LL) break;
        Q = (long long)next;
      }
      cout << "0 " << Q << endl;
      cout.flush();
      long long g;
      cin >> g;
      int e = 0;
      long long temp = g;
      while (temp % p == 0) {
        temp /= p;
        e++;
      }
      d *= (e + 1);
    }
    long long ans = 2 * d;
    cout << "1 " << ans << endl;
    cout.flush();
  }
  return 0;
}