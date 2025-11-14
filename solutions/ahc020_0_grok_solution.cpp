#include <bits/stdc++.h>
using namespace std;

struct Edge {
  int u, v, w, idx;
};

int main() {
  int N, M, K;
  cin >> N >> M >> K;
  vector<pair<int, int>> st(N + 1);
  for (int i = 1; i <= N; i++) {
    cin >> st[i].first >> st[i].second;
  }
  vector<Edge> edgs(M);
  for (int j = 0; j < M; j++) {
    cin >> edgs[j].u >> edgs[j].v >> edgs[j].w;
    edgs[j].idx = j + 1;
  }
  vector<pair<int, int>> residents(K);
  for (int k = 0; k < K; k++) {
    cin >> residents[k].first >> residents[k].second;
  }
  sort(edgs.begin(), edgs.end(), [](const Edge& a, const Edge& b) {
    return a.w < b.w;
  });
  vector<int> parent(N + 1);
  for (int i = 1; i <= N; i++) parent[i] = i;
  function<int(int)> find = [&](int x) -> int {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
  };
  vector<int> B(M + 1, 0);
  vector<vector<pair<int, int>>> tree(N + 1);
  for (auto& e : edgs) {
    int pu = find(e.u), pv = find(e.v);
    if (pu != pv) {
      parent[pu] = pv;
      B[e.idx] = 1;
      tree[e.u].emplace_back(e.v, e.idx);
      tree[e.v].emplace_back(e.u, e.idx);
    }
  }
  vector<vector<int>> min_p(N + 1, vector<int>(K, 0));
  for (int i = 1; i <= N; i++) {
    for (int k = 0; k < K; k++) {
      long long dx = (long long)st[i].first - residents[k].first;
      long long dy = (long long)st[i].second - residents[k].second;
      long long dd = dx * dx + dy * dy;
      long long low = 0, high = 5001;
      while (low < high) {
        long long m = (low + high) / 2;
        if (m * m >= dd) high = m;
        else low = m + 1;
      }
      min_p[i][k] = low;
    }
  }
  vector<int> assign_to(K);
  vector<multiset<long long>> reqs(N + 1);
  for (int k = 0; k < K; k++) {
    int best = 1;
    int min_c = 5001;
    for (int i = 1; i <= N; i++) {
      int c = min_p[i][k];
      if (c < min_c) {
        min_c = c;
        best = i;
      }
    }
    assign_to[k] = best;
    reqs[best].insert(min_c);
  }
  vector<int> P(N + 1, 0);
  for (int i = 1; i <= N; i++) {
    if (!reqs[i].empty()) P[i] = *reqs[i].rbegin();
  }
  vector<bool> in_tree(N + 1, true);
  bool changed = true;
  while (changed) {
    changed = false;
    vector<int> leaves;
    for (int ii = 2; ii <= N; ii++) {
      if (in_tree[ii] && tree[ii].size() == 1) leaves.push_back(ii);
    }
    for (int leaf : leaves) {
      vector<int> affected;
      for (int kk = 0; kk < K; kk++) {
        if (assign_to[kk] == leaf) affected.push_back(kk);
      }
      if (affected.size() > 100) continue;
      map<int, long long> additional_max_for;
      vector<pair<int, long long>> new_assigns;
      bool can = true;
      for (int kk : affected) {
        int new_best = -1;
        long long min_c = 5001;
        for (int j = 1; j <= N; j++) {
          if (in_tree[j] && j != leaf) {
            long long c = min_p[j][kk];
            if (c < min_c) {
              min_c = c;
              new_best = j;
            }
          }
        }
        if (new_best == -1 || min_c > 5000) {
          can = false;
          break;
        }
        new_assigns.emplace_back(new_best, min_c);
        additional_max_for[new_best] = max(additional_max_for[new_best], min_c);
      }
      if (!can) continue;
      long long delta = - (long long)P[leaf] * P[leaf];
      for (auto [j, extra] : additional_max_for) {
        long long old_p = P[j];
        long long new_p = max(old_p, extra);
        delta += new_p * new_p - old_p * old_p;
      }
      int edge_id = tree[leaf][0].second;
      long long saved = edgs[edge_id - 1].w; // idx 1 to M, edgs[0] idx=1
      if (delta < saved) {
        changed = true;
        B[edge_id] = 0;
        int par = tree[leaf][0].first;
        tree[leaf].clear();
        auto& tpar = tree[par];
        for (size_t pp = 0; pp < tpar.size(); pp++) {
          if (tpar[pp].first == leaf) {
            tpar.erase(tpar.begin() + pp);
            break;
          }
        }
        reqs[leaf].clear();
        P[leaf] = 0;
        in_tree[leaf] = false;
        set<int> to_update;
        for (size_t aa = 0; aa < affected.size(); aa++) {
          int kk = affected[aa];
          int nb = new_assigns[aa].first;
          long long nc = new_assigns[aa].second;
          assign_to[kk] = nb;
          reqs[nb].insert(nc);
          to_update.insert(nb);
        }
        for (int j : to_update) {
          P[j] = reqs[j].empty() ? 0 : *reqs[j].rbegin();
        }
      }
    }
  }
  for (int i = 1; i <= N; i++) {
    if (i > 1) cout << " ";
    cout << P[i];
  }
  cout << endl;
  for (int j = 1; j <= M; j++) {
    if (j > 1) cout << " ";
    cout << B[j];
  }
  cout << endl;
  return 0;
}