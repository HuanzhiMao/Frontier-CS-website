#include <bits/stdc++.h>

using namespace std;

int main() {
  int si, sj;
  cin >> si >> sj;
  vector<vector<int>> tile(50, vector<int>(50));
  for (int i = 0; i < 50; i++) for (int j = 0; j < 50; j++) cin >> tile[i][j];
  vector<vector<int>> score(50, vector<int>(50));
  for (int i = 0; i < 50; i++) for (int j = 0; j < 50; j++) cin >> score[i][j];
  set<int> visited;
  visited.insert(tile[si][sj]);
  string path = "";
  int ci = si, cj = sj;
  vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  string movs = "UDLR";
  while (true) {
    struct Cand {
      int val;
      int d;
      int ni, nj;
    };
    vector<Cand> cands;
    for (int d = 0; d < 4; d++) {
      int ni = ci + dirs[d].first;
      int nj = cj + dirs[d].second;
      if (ni >= 0 && ni < 50 && nj >= 0 && nj < 50) {
        int tt = tile[ni][nj];
        if (visited.find(tt) == visited.end()) {
          set<int> newvis = visited;
          newvis.insert(tt);
          int maxnext = 0;
          for (auto& dd : dirs) {
            int nni = ni + dd.first;
            int nnj = nj + dd.second;
            if (nni >= 0 && nni < 50 && nnj >= 0 && nnj < 50) {
              int ttt = tile[nni][nnj];
              if (newvis.find(ttt) == newvis.end()) {
                maxnext = max(maxnext, score[nni][nnj]);
              }
            }
          }
          int val = score[ni][nj] + maxnext;
          cands.push_back({val, d, ni, nj});
        }
      }
    }
    if (cands.empty()) break;
    sort(cands.begin(), cands.end(), [](const Cand& a, const Cand& b) {
      return a.val > b.val;
    });
    auto chosen = cands[0];
    path += movs[chosen.d];
    ci = chosen.ni;
    cj = chosen.nj;
    visited.insert(tile[ci][cj]);
  }
  cout << path << endl;
  return 0;
}