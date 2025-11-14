#include <bits/stdc++.h>
using namespace std;

vector<int> smallest_prime_factor(int n) {
  vector<int> spf(n + 1);
  for (int i = 0; i <= n; i++) spf[i] = i;
  for (int i = 2; i * i <= n; i++) {
    if (spf[i] == i) {
      for (int j = i * i; j <= n; j += i) {
        if (spf[j] == j) spf[j] = i;
      }
    }
  }
  return spf;
}

vector<int> get_primes(int n, const vector<int>& spf) {
  vector<int> pr;
  for (int i = 2; i <= n; i++) if (spf[i] == i) pr.push_back(i);
  return pr;
}

int compute_f(int x, const vector<int>& spf, const vector<int>& prime_sign_arr, int current_p, int trial_sign) {
  if (x == 1) return 1;
  int res = 1;
  int temp = x;
  while (temp > 1) {
    int pp = spf[temp];
    int exp = 0;
    while (temp % pp == 0) {
      temp /= pp;
      exp++;
    }
    int fps = (pp == current_p) ? trial_sign : prime_sign_arr[pp];
    res *= (fps == 1 ? 1 : ((exp % 2 == 0) ? 1 : -1));
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  cin >> n;
  auto spf = smallest_prime_factor(n);
  auto primes = get_primes(n, spf);
  primes.push_back(n + 1);
  vector<int> f(n + 1, 0);
  vector<int> prime_sign_arr(n + 1, 0);
  f[1] = 1;
  long long current_S = 1;
  int current_end = 1;
  for (size_t pi = 0; pi < primes.size() - 1; ++pi) {
    int p = primes[pi];
    int next_p = primes[pi + 1];
    struct Option {
      int trial;
      int max_abs;
      long long final_S;
    };
    vector<Option> options;
    for (int tr : {1, -1}) {
      long long temp_S = current_S;
      int loc_max = 0;
      for (int j = current_end + 1; j < next_p; ++j) {
        int fj = compute_f(j, spf, prime_sign_arr, p, tr);
        temp_S += fj;
        loc_max = max(loc_max, (int)abs(temp_S));
      }
      options.push_back({tr, loc_max, temp_S});
    }
    sort(options.begin(), options.end(), [](const Option& a, const Option& b) {
      if (a.max_abs != b.max_abs) return a.max_abs < b.max_abs;
      int absa = abs(a.final_S);
      int absb = abs(b.final_S);
      if (absa != absb) return absa < absb;
      return a.trial > b.trial;
    });
    int chosen = options[0].trial;
    prime_sign_arr[p] = chosen;
    for (int j = current_end + 1; j < next_p; ++j) {
      f[j] = compute_f(j, spf, prime_sign_arr, p, chosen);
      current_S += f[j];
    }
    current_end = next_p - 1;
  }
  for (int i = 1; i <= n; i++) {
    if (i > 1) cout << " ";
    cout << f[i];
  }
  cout << "\n";
  return 0;
}