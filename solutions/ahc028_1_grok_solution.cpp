#include <bits/stdc++.h>

using namespace std;

int main() {
  int N, M;
  cin >> N >> M;
  int si, sj;
  cin >> si >> sj;
  vector<string> grid(N);
  for (int i = 0; i < N; i++) cin >> grid[i];
  vector<string> t(M);
  for (int i = 0; i < M; i++) cin >> t[i];

  vector<vector<pair<int, int>>> pos(26);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      char c = grid[i][j];
      pos[c - 'A'].push_back({i, j});
    }
  }

  vector<pair<int, int>> start_scores;
  for (int k = 0; k < M; k++) {
    char need = t[k][0];
    int mind = 1e9;
    for (auto [x, y] : pos[need - 'A']) {
      int d = abs(x - si) + abs(y - sj);
      if (d < mind) mind = d;
    }
    start_scores.push_back({mind, k});
  }
  sort(start_scores.begin(), start_scores.end());

  const int NUM_TRY = 5;
  const int INF = 1e9 + 10;
  vector<pair<int, int>> best_sequence;
  int best_cost = INF;

  for (int tryy = 0; tryy < min(NUM_TRY, (int)start_scores.size()); tryy++) {
    int best_start = start_scores[tryy].second;
    string S = t[best_start];
    vector<string> remaining;
    for (int i = 0; i < M; i++)
      if (i != best_start) remaining.push_back(t[i]);

    while (!remaining.empty()) {
      int max_ov = -1;
      int best_idx = -1;
      int rem_size = remaining.size();
      for (int idx = 0; idx < rem_size; idx++) {
        string cand = remaining[idx];
        int ov = 0;
        for (int k = 4; k >= 1; k--) {
          if (k > S.size()) continue;
          if (S.substr(S.size() - k, k) == cand.substr(0, k)) {
            ov = k;
            break;
          }
        }
        if (max_ov == -1 || ov > max_ov) {
          max_ov = ov;
          best_idx = idx;
        } else if (ov == max_ov) {
          if (idx < best_idx) best_idx = idx;
        }
      }
      if (best_idx == -1) {
        best_idx = 0;
        max_ov = 0;
      }
      string to_add = remaining[best_idx].substr(max_ov);
      S += to_add;
      remaining.erase(remaining.begin() + best_idx);
    }

    int len = S.size();
    vector<vector<vector<int>>> dp(len + 1, vector<vector<int>>(N, vector<int>(N, INF)));
    dp[0][si][sj] = 0;
    using pii = pair<int, int>;
    vector<vector<vector<pii>>> pre(len + 1, vector<vector<pii>>(N, vector<pii>(N, {-1, -1})));

    for (int step = 0; step < len; step++) {
      char need = S[step];
      int cid = need - 'A';
      for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
          if (dp[step][x][y] >= INF) continue;
          for (auto [nx, ny] : pos[cid]) {
            int dist = abs(nx - x) + abs(ny - y);
            int add = dist + 1;
            int nc = dp[step][x][y] + add;
            if (nc < dp[step + 1][nx][ny]) {
              dp[step + 1][nx][ny] = nc;
              pre[step + 1][nx][ny] = {x, y};
            }
          }
        }
      }
    }

    int min_t = INF;
    pii end_pos = {-1, -1};
    for (int x = 0; x < N; x++) {
      for (int y = 0; y < N; y++) {
        if (dp[len][x][y] < min_t) {
          min_t = dp[len][x][y];
          end_pos = {x, y};
        }
      }
    }
    if (min_t < best_cost) {
      best_cost = min_t;
      vector<pii> sequence;
      int cur_step = len;
      pii cur = end_pos;
      while (cur_step > 0) {
        sequence.push_back(cur);
        cur = pre[cur_step][cur.first][cur.second];
        cur_step--;
      }
      reverse(sequence.begin(), sequence.end());
      best_sequence = sequence;
    }
  }

  for (auto [i, j] : best_sequence) {
    cout << i << " " << j << endl;
  }
  return 0;
}