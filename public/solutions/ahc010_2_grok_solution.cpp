#include <bits/stdc++.h>
using namespace std;

int main() {
  vector<vector<int>> grid(30, vector<int>(30));
  for (int i = 0; i < 30; i++) {
    string s;
    cin >> s;
    for (int j = 0; j < 30; j++) {
      grid[i][j] = s[j] - '0';
    }
  }
  int next_type[8] = {1, 2, 3, 0, 5, 4, 7, 6};
  int TO[8][4] = {
      {1, 0, -1, -1}, {3, -1, -1, 0}, {-1, -1, 3, 2}, {-1, 2, 1, -1},
      {1, 0, 3, 2},   {3, 2, 1, 0},   {2, -1, 0, -1}, {-1, 3, -1, 1}};
  int di[4] = {0, -1, 0, 1};
  int dj[4] = {-1, 0, 1, 0};
  long long best_score = -1;
  string best_ans = "";
  for (int st = 6; st <= 7; st++) {
    for (int tw = 4; tw <= 5; tw++) {
      for (int si = 0; si <= 3; si++) {
        vector<vector<int>> this_rot(30, vector<int>(30));
        for (int i = 0; i < 30; i++) {
          for (int j = 0; j < 30; j++) {
            int init = grid[i][j];
            int target;
            if (init <= 3)
              target = si;
            else if (init <= 5)
              target = tw;
            else
              target = st;
            int curr = init;
            int r = 0;
            while (curr != target && r < 4) {
              curr = next_type[curr];
              r++;
            }
            this_rot[i][j] = r;
          }
        }
        vector<vector<int>> tiles(30, vector<int>(30));
        for (int i = 0; i < 30; i++) {
          for (int j = 0; j < 30; j++) {
            int t = grid[i][j];
            for (int k = 0; k < this_rot[i][j]; k++) {
              t = next_type[t];
            }
            tiles[i][j] = t;
          }
        }
        const int SZ = 30;
        const int NN = SZ * SZ * 4;
        vector<int> succ(NN, -1);
        for (int i = 0; i < SZ; i++) {
          for (int j = 0; j < SZ; j++) {
            for (int d = 0; d < 4; d++) {
              int pos = (i * SZ + j) * 4 + d;
              int t = tiles[i][j];
              int d2 = TO[t][d];
              if (d2 == -1) continue;
              int ni = i + di[d2];
              int nj = j + dj[d2];
              if (ni < 0 || ni >= SZ || nj < 0 || nj >= SZ) continue;
              int nd = (d2 + 2) % 4;
              int npos = (ni * SZ + nj) * 4 + nd;
              succ[pos] = npos;
            }
          }
        }
        vector<long long> cycle_lengths;
        vector<bool> visited(NN, false);
        for (int start = 0; start < NN; start++) {
          if (visited[start] || succ[start] == -1) continue;
          vector<int> component;
          int cur = start;
          while (!visited[cur] && succ[cur] != -1) {
            visited[cur] = true;
            component.push_back(cur);
            cur = succ[cur];
          }
          if (succ[cur] != -1) {
            auto it = find(component.begin(), component.end(), cur);
            if (it != component.end()) {
              long long len = component.size() - (it - component.begin());
              cycle_lengths.push_back(len);
            }
          }
        }
        if (cycle_lengths.size() < 2) continue;
        sort(cycle_lengths.rbegin(), cycle_lengths.rend());
        long long L1 = cycle_lengths[0];
        long long L2 = cycle_lengths[1];
        long long score = L1 * L2;
        if (score > best_score) {
          best_score = score;
          best_ans = "";
          for (int i = 0; i < 30; i++) {
            for (int j = 0; j < 30; j++) {
              best_ans += '0' + this_rot[i][j];
            }
          }
        }
      }
    }
  }
  if (best_score == -1) {
    best_ans = "";
    for (int i = 0; i < 900; i++) best_ans += '0';
  }
  cout << best_ans << endl;
  return 0;
}