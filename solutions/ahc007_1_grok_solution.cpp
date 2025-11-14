#include <bits/stdc++.h>
using namespace std;

const int MAXN = 405;
struct UF {
  vector<int> parent, rnk;
  UF(int n) {
    parent.resize(n); rnk.resize(n, 0);
    for (int i = 0; i < n; i++) parent[i] = i;
  }
  int find(int v) {
    if (parent[v] == v) return v;
    return parent[v] = find(parent[v]);
  }
  void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) swap(a, b);
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
  }
};

int main() {
  int N = 400, M = 1995;
  vector<int> x(N), y(N);
  for (int i = 0; i < N; i++) {
    cin >> x[i] >> y[i];
  }
  vector<int> u(M), v(M);
  for (int i = 0; i < M; i++) {
    cin >> u[i] >> v[i];
  }
  vector<int> d(M);
  for (int i = 0; i < M; i++) {
    double dx = x[u[i]] - x[v[i]];
    double dy = y[u[i]] - y[v[i]];
    double dist = sqrt(dx * dx + dy * dy);
    d[i] = round(dist);
  }
  UF current(N);
  for (int i = 0; i < M; i++) {
    int li;
    cin >> li;
    int pu = current.find(u[i]);
    int pv = current.find(v[i]);
    if (pu == pv) {
      cout << 0 << endl;
      continue;
    }
    vector<int> comp_id(N, -1);
    int cnt = 0;
    for (int j = 0; j < N; j++) {
      int p = current.find(j);
      if (comp_id[p] == -1) comp_id[p] = cnt++;
    }
    int sidu = comp_id[pu];
    int sidv = comp_id[pv];
    UF temp(cnt);
    for (int j = i + 1; j < M; j++) {
      int au = current.find(u[j]);
      int av = current.find(v[j]);
      if (au != av) {
        int s1 = comp_id[au];
        int s2 = comp_id[av];
        temp.unite(s1, s2);
      }
    }
    if (temp.find(sidu) != temp.find(sidv)) {
      cout << 1 << endl;
      current.unite(pu, pv);
      continue;
    }
    vector<vector<pair<int, double>>> g(cnt);
    for (int j = i + 1; j < M; j++) {
      int au = current.find(u[j]);
      int av = current.find(v[j]);
      if (au != av) {
        int s1 = comp_id[au];
        int s2 = comp_id[av];
        double w = 2.0 * d[j];
        g[s1].push_back({s2, w});
        g[s2].push_back({s1, w});
      }
    }
    vector<double> dist(cnt, 1e18);
    dist[sidu] = 0;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, sidu});
    while (!pq.empty()) {
      auto [cost, ver] = pq.top(); pq.pop();
      if (cost > dist[ver]) continue;
      for (auto [to, w] : g[ver]) {
        if (dist[to] > dist[ver] + w) {
          dist[to] = dist[ver] + w;
          pq.push({dist[to], to});
        }
      }
    }
    double est = dist[sidv];
    int adopt = (li < est) ? 1 : 0;
    cout << adopt << endl;
    if (adopt) current.unite(pu, pv);
  }
  return 0;
}