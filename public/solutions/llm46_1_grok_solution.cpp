#include <bits/stdc++.h>
using namespace std;

int main() {
  int J, M;
  cin >> J >> M;
  vector<vector<int>> route(J, vector<int>(M));
  vector<vector<long long>> proc(J, vector<long long>(M));
  for (int j = 0; j < J; j++) {
    for (int i = 0; i < M; i++) {
      cin >> route[j][i] >> proc[j][i];
    }
  }
  vector<vector<long long>> prefix_remaining(J, vector<long long>(M + 1, 0LL));
  for (int j = 0; j < J; j++) {
    for (int kk = M - 1; kk >= 0; kk--) {
      prefix_remaining[j][kk] = prefix_remaining[j][kk + 1] + proc[j][kk];
    }
  }
  auto simulate = [&](int rule) -> pair<long long, vector<vector<int>>> {
    vector<vector<int>> ord(M);
    vector<int> jnk(J, 0);
    vector<long long> jna(J, 0LL);
    vector<long long> mav(M, 0LL);
    long long cur_time = 0LL;
    int tot_sch = 0;
    long long max_fin = 0LL;
    while (true) {
      while (true) {
        long long best_prio = LLONG_MIN / 2;
        int ch_j = -1;
        for (int jj = 0; jj < J; jj++) {
          if (jnk[jj] >= M) continue;
          if (jna[jj] > cur_time) continue;
          int kk = jnk[jj];
          int mm = route[jj][kk];
          if (mav[mm] > cur_time) continue;
          long long ppp = proc[jj][kk];
          long long rem = prefix_remaining[jj][kk];
          long long prio;
          if (rule == 0) prio = -ppp;
          else if (rule == 1) prio = ppp;
          else if (rule == 2) prio = -rem;
          else if (rule == 3) prio = rem;
          else assert(false);
          if (prio > best_prio || (prio == best_prio && jj < ch_j)) {
            best_prio = prio;
            ch_j = jj;
          }
        }
        if (ch_j == -1) break;
        int jj = ch_j;
        int kk = jnk[jj];
        int mm = route[jj][kk];
        long long p = proc[jj][kk];
        long long finish = cur_time + p;
        ord[mm].push_back(jj);
        jnk[jj]++;
        if (jnk[jj] < M) jna[jj] = finish;
        mav[mm] = finish;
        max_fin = max(max_fin, finish);
        tot_sch++;
      }
      if (tot_sch == J * M) break;
      long long next_t = LLONG_MAX;
      for (int jj = 0; jj < J; jj++) {
        if (jnk[jj] < M && jna[jj] > cur_time) {
          next_t = min(next_t, jna[jj]);
        }
      }
      for (int mm = 0; mm < M; mm++) {
        if (mav[mm] > cur_time) {
          next_t = min(next_t, mav[mm]);
        }
      }
      if (next_t == LLONG_MAX) {
        return {LLONG_MAX, {}};
      }
      cur_time = next_t;
    }
    return {max_fin, ord};
  };
  long long best_ms = LLONG_MAX;
  vector<vector<int>> best_order;
  for (int r = 0; r < 4; r++) {
    auto [ms, ord] = simulate(r);
    if (ms < best_ms) {
      best_ms = ms;
      best_order = move(ord);
    }
  }
  for (int m = 0; m < M; m++) {
    for (size_t i = 0; i < best_order[m].size(); i++) {
      if (i > 0) cout << " ";
      cout << best_order[m][i];
    }
    cout << endl;
  }
  return 0;
}