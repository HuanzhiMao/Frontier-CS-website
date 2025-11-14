#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Pending {
  ll ready;
  int job;
  int p;
  ll remaining;
};

int main() {
  int J, M;
  cin >> J >> M;
  vector<vector<pair<int, int>>> route(J, vector<pair<int, int>>(M));
  for (int j = 0; j < J; j++) {
    for (int k = 0; k < M; k++) {
      int m, p;
      cin >> m >> p;
      route[j][k] = {m, p};
    }
  }
  // simulate
  ll current_time = 0;
  vector<ll> machine_free(M, 0);
  vector<int> running_job(M, -1);
  vector<int> current_op_idx(J, 0);
  vector<ll> job_ready_time(J, 0);
  vector<vector<Pending>> pending(M);
  vector<vector<int>> order(M);
  // initial pending
  for (int j = 0; j < J; j++) {
    int op = current_op_idx[j];
    int m = route[j][op].first;
    int p = route[j][op].second;
    ll ready = job_ready_time[j];
    ll rem = 0;
    for (int kk = op; kk < M; kk++) rem += route[j][kk].second;
    pending[m].push_back({ready, j, p, rem});
  }
  // loop
  while (true) {
    bool done = true;
    for (int j = 0; j < J; j++)
      if (current_op_idx[j] < M) done = false;
    if (done) break;
    // find next_time
    ll next_time = LLONG_MAX / 2;
    // completions
    for (int m = 0; m < M; m++) {
      if (running_job[m] != -1) {
        next_time = min(next_time, machine_free[m]);
      }
    }
    // idle machines' min ready
    for (int m = 0; m < M; m++) {
      if (running_job[m] == -1 && !pending[m].empty()) {
        ll min_r = LLONG_MAX / 2;
        for (auto& pd : pending[m]) min_r = min(min_r, pd.ready);
        next_time = min(next_time, min_r);
      }
    }
    if (next_time == LLONG_MAX / 2) assert(false);
    current_time = next_time;
    // process completions at current_time
    for (int m = 0; m < M; m++) {
      if (running_job[m] != -1 && machine_free[m] == current_time) {
        int j = running_job[m];
        running_job[m] = -1;
        // advance job
        current_op_idx[j]++;
        job_ready_time[j] = current_time;
        if (current_op_idx[j] < M) {
          int next_op = current_op_idx[j];
          int next_m = route[j][next_op].first;
          int next_p = route[j][next_op].second;
          ll ready = job_ready_time[j];
          ll rem = 0;
          for (int kk = next_op; kk < M; kk++) rem += route[j][kk].second;
          pending[next_m].push_back({ready, j, next_p, rem});
        }
      }
    }
    // now dispatch on idle machines if possible
    for (int m = 0; m < M; m++) {
      if (running_job[m] == -1 && !pending[m].empty()) {
        // find best
        ll best_priority = -1;
        int best_job = INT_MAX;
        int best_idx = -1;
        for (int i = 0; i < pending[m].size(); i++) {
          auto& pd = pending[m][i];
          if (pd.ready <= current_time) {
            ll pri = pd.remaining;
            int jj = pd.job;
            if (pri > best_priority || (pri == best_priority && jj < best_job)) {
              best_priority = pri;
              best_job = jj;
              best_idx = i;
            }
          }
        }
        if (best_idx != -1) {
          // dispatch
          int j = pending[m][best_idx].job;
          int pp = pending[m][best_idx].p;
          swap(pending[m][best_idx], pending[m].back());
          pending[m].pop_back();
          ll start = current_time;
          ll complete = start + pp;
          running_job[m] = j;
          machine_free[m] = complete;
          // record order
          order[m].push_back(j);
        }
      }
    }
  }
  // output
  for (int m = 0; m < M; m++) {
    for (int i = 0; i < order[m].size(); i++) {
      if (i > 0) cout << " ";
      cout << order[m][i];
    }
    cout << endl;
  }
  return 0;
}