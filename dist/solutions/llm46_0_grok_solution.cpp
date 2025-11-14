#include <bits/stdc++.h>
using namespace std;

int main() {
  int J, M;
  cin >> J >> M;
  vector<vector<int>> route(J, vector<int>(M));
  vector<vector<int>> duration(J, vector<int>(M));
  for (int j = 0; j < J; j++) {
    for (int k = 0; k < M; k++) {
      int m, p;
      cin >> m >> p;
      route[j][k] = m;
      duration[j][k] = p;
    }
  }
  vector<vector<int>> machine_pos(J, vector<int>(M));
  for (int j = 0; j < J; j++)
    for (int k = 0; k < M; k++) machine_pos[j][route[j][k]] = k;
  vector<vector<long long>> remaining(J, vector<long long>(M + 1, 0));
  for (int j = 0; j < J; j++) {
    remaining[j][M] = 0;
    for (int k = M - 1; k >= 0; k--)
      remaining[j][k] = remaining[j][k + 1] + duration[j][k];
  }
  vector<vector<long long>> start_time(J, vector<long long>(M, -1));
  vector<long long> machine_free_at(M, 0);
  vector<long long> job_free_at(J, 0);
  vector<int> next_op(J, 0);
  const long long INF = 1LL << 60;
  while (true) {
    bool done = true;
    for (int j = 0; j < J; j++)
      if (next_op[j] < M) done = false;
    if (done) break;
    long long t_min = INF;
    for (int j = 0; j < J; j++) {
      if (next_op[j] >= M) continue;
      int k = next_op[j];
      int m = route[j][k];
      long long est = max(job_free_at[j], machine_free_at[m]);
      if (est < t_min) t_min = est;
    }
    if (t_min == INF) assert(false);
    for (int m = 0; m < M; m++) {
      if (machine_free_at[m] > t_min) continue;
      vector<int> cands;
      for (int jj = 0; jj < J; jj++) {
        if (next_op[jj] < M && job_free_at[jj] <= t_min &&
            route[jj][next_op[jj]] == m)
          cands.push_back(jj);
      }
      if (cands.empty()) continue;
      long long best_rem = -1;
      int best_p = -1;
      int best_j = INT_MAX;
      for (int jj : cands) {
        int kk = next_op[jj];
        long long rem = remaining[jj][kk];
        int pp = duration[jj][kk];
        bool better = false;
        if (rem > best_rem)
          better = true;
        else if (rem == best_rem) {
          if (pp > best_p)
            better = true;
          else if (pp == best_p) {
            if (jj < best_j) better = true;
          }
        }
        if (better) {
          best_rem = rem;
          best_p = pp;
          best_j = jj;
        }
      }
      int j = best_j;
      int k = next_op[j];
      start_time[j][k] = t_min;
      int p = duration[j][k];
      long long comp = t_min + p;
      machine_free_at[m] = comp;
      job_free_at[j] = comp;
      next_op[j]++;
    }
  }
  for (int m = 0; m < M; m++) {
    vector<pair<long long, int>> order;
    for (int j = 0; j < J; j++) {
      int k = machine_pos[j][m];
      long long st = start_time[j][k];
      order.push_back({st, j});
    }
    sort(order.begin(), order.end());
    for (int i = 0; i < J; i++) {
      if (i > 0) cout << " ";
      cout << order[i].second;
    }
    cout << endl;
  }
  return 0;
}