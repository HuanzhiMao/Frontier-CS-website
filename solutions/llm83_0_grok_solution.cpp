#include <bits/stdc++.h>
using namespace std;

const int N = 1000010;
int spf[N];
vector<int> primes;
int fp[N];
vector<int> func(N);
int n;

int get_f(int x, int trial_p = -1, int trial_sign = 0) {
  if (x == 1) return 1;
  int res = 1;
  int y = x;
  while (y > 1) {
    int p = spf[y];
    int exp = 0;
    while (y % p == 0) {
      y /= p;
      exp++;
    }
    if (exp % 2 == 1) {
      if (p == trial_p) {
        res *= trial_sign;
      } else {
        res *= fp[p];
      }
    }
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n;
  for (int i = 0; i <= n; i++) spf[i] = i;
  for (int i = 2; i * i <= n; i++) {
    if (spf[i] == i) {
      for (int j = i * i; j <= n; j += i) {
        if (spf[j] == j) spf[j] = i;
      }
    }
  }
  for (int i = 2; i <= n; i++) {
    if (spf[i] == i) primes.push_back(i);
  }
  func[1] = 1;
  long long current_S = 1;
  int prime_idx = 0;
  int current_pos = 2;
  while (current_pos <= n) {
    if (prime_idx >= (int)primes.size() || current_pos < primes[prime_idx]) {
      int fj = get_f(current_pos);
      func[current_pos] = fj;
      current_S += fj;
      current_pos++;
    } else {
      int p = current_pos;
      int next_q = (prime_idx + 1 < (int)primes.size()) ? primes[prime_idx + 1] : n + 1;
      pair<int, int> best_choice = {INT_MAX, INT_MAX};
      int chosen_sign = 0;
      for (int sgn : {1, -1}) {
        long long temp_S = current_S;
        int seg_max = 0;
        for (int j = p; j < next_q; j++) {
          int fj = get_f(j, p, sgn);
          temp_S += fj;
          seg_max = max(seg_max, (int)abs(temp_S));
        }
        int end_abs = abs((int)temp_S);
        pair<int, int> score = {seg_max, end_abs};
        if (score < best_choice || (score == best_choice && sgn == -1)) {
          best_choice = score;
          chosen_sign = sgn;
        }
      }
      fp[p] = chosen_sign;
      for (int j = p; j < next_q; j++) {
        int fj = get_f(j);
        func[j] = fj;
        current_S += fj;
      }
      current_pos = next_q;
      prime_idx++;
    }
  }
  for (int i = 1; i <= n; i++) {
    cout << func[i];
    if (i < n) cout << " ";
    else cout << "\n";
  }
  return 0;
}