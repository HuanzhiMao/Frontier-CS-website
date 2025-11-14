#include <bits/stdc++.h>
using namespace std;

const int N = 50;
const int MAX_M = 2501;
const int BEAM = 100;

struct State {
  int i, j;
  long long sum;
  bitset<MAX_M> used;
  int prev;
  char move;
};

int main() {
  int si, sj;
  cin >> si >> sj;
  int T[N][N], P[N][N];
  int max_t = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> T[i][j];
      max_t = max(max_t, T[i][j]);
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> P[i][j];
    }
  }
  struct Dir {
    char c;
    int di, dj;
  } dirs[4] = {{'U', -1, 0}, {'D', 1, 0}, {'L', 0, -1}, {'R', 0, 1}};
  vector<State> all_states;
  all_states.reserve(2000000);
  bitset<MAX_M> init_used;
  init_used.set(T[si][sj]);
  all_states.push_back({si, sj, P[si][sj], init_used, -1, '\0'});
  vector<int> curr_beam = {0};
  long long max_sum = P[si][sj];
  int best_idx = 0;
  for (int step = 0; step < 2500; ++step) {
    vector<pair<long long, int>> cand;
    cand.reserve(curr_beam.size() * 4);
    for (int idx : curr_beam) {
      const State& st = all_states[idx];
      for (auto& d : dirs) {
        int ni = st.i + d.di;
        int nj = st.j + d.dj;
        if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
        int tt = T[ni][nj];
        if (st.used.test(tt)) continue;
        bitset<MAX_M> newu = st.used;
        newu.set(tt);
        long long nsum = st.sum + P[ni][nj];
        int nidx = all_states.size();
        all_states.push_back({ni, nj, nsum, newu, idx, d.c});
        cand.emplace_back(nsum, nidx);
        if (nsum > max_sum) {
          max_sum = nsum;
          best_idx = nidx;
        }
      }
    }
    if (cand.empty()) break;
    sort(cand.rbegin(), cand.rend());
    vector<int> new_beam;
    new_beam.reserve(BEAM);
    for (int k = 0; k < BEAM && k < (int)cand.size(); ++k) {
      new_beam.push_back(cand[k].second);
    }
    curr_beam = new_beam;
  }
  string path = "";
  int cur = best_idx;
  while (cur != -1) {
    const State& st = all_states[cur];
    if (st.move != '\0') path += st.move;
    cur = st.prev;
  }
  reverse(path.begin(), path.end());
  cout << path << endl;
  return 0;
}