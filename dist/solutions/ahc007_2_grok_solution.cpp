#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
  vector<int> par, rnk;
  UnionFind(int n) : par(n), rnk(n, 0) {
    for (int i = 0; i < n; i++) par[i] = i;
  }
  int find(int x) {
    if (par[x] != x) par[x] = find(par[x]);
    return par[x];
  }
  bool unite(int x, int y) {
    int px = find(x), py = find(y);
    if (px == py) return false;
    if (rnk[px] < rnk[py]) swap(px, py);
    par[py] = px;
    if (rnk[px] == rnk[py]) rnk[px]++;
    return true;
  }
};

int main() {
  const int N = 400, M = 1995;
  vector<pair<int, int>> pos(N);
  for (int i = 0; i < N; i++) {
    cin >> pos[i].first >> pos[i].second;
  }
  vector<pair<int, int>> edges(M);
  vector<int> d(M);
  for (int i = 0; i < M; i++) {
    int u, v;
    cin >> u >> v;
    edges[i] = {u, v};
    long long dx = pos[u].first - pos[v].first;
    long long dy = pos[u].second - pos[v].second;
    double dist = sqrt(dx * dx + dy * dy);
    d[i] = round(dist);
  }
  UnionFind real(N);
  for (int i = 0; i < M; i++) {
    int l;
    cin >> l;
    int u = edges[i].first, v = edges[i].second;
    int cu = real.find(u);
    int cv = real.find(v);
    if (cu == cv) {
      cout << 0 << endl;
      continue;
    }
    UnionFind meta(N);
    for (int j = i + 1; j < M; j++) {
      int au = real.find(edges[j].first);
      int av = real.find(edges[j].second);
      if (au != av) {
        meta.unite(au, av);
      }
    }
    int mcu = meta.find(cu);
    int mcv = meta.find(cv);
    bool forced = (mcu != mcv);
    bool adopt = forced;
    if (!forced) {
      if (l <= 2 * d[i]) adopt = true;
    }
    cout << (adopt ? 1 : 0) << endl;
    if (adopt) {
      real.unite(u, v);
    }
  }
  return 0;
}