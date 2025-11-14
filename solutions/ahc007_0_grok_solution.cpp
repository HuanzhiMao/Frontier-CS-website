#include <bits/stdc++.h>
using namespace std;

const int MAXN = 405;
int N = 400, M = 1995;
vector<int> X(MAXN), Y(MAXN);
vector<pair<int, int>> edges;
vector<int> D;

struct UF {
  vector<int> par, sz;
  UF(int n) {
    par.resize(n);
    sz.resize(n, 1);
    for (int i = 0; i < n; i++) par[i] = i;
  }
  int find(int x) {
    if (par[x] != x) par[x] = find(par[x]);
    return par[x];
  }
  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (sz[a] < sz[b]) swap(a, b);
    par[b] = a;
    sz[a] += sz[b];
    return true;
  }
  bool same(int a, int b) { return find(a) == find(b); }
  int comps() {
    set<int> roots;
    for (int i = 0; i < N; i++) roots.insert(find(i));
    return roots.size();
  }
};

double compute_emin(const vector<int>& cross_d) {
  int k = cross_d.size();
  if (k == 0) return 0.0;
  int minl = *min_element(cross_d.begin(), cross_d.end());
  int maxu = 0;
  for (int dd : cross_d) maxu = max(maxu, 3 * dd);
  double e = 0.0;
  for (int t = 1; t <= maxu; t++) {
    double p = 1.0;
    for (int dd : cross_d) {
      int lj = dd;
      int uj = 3 * dd;
      int len = uj - lj + 1;
      int ge = 0;
      if (t <= lj) ge = len;
      else if (t <= uj) ge = uj - t + 1;
      p *= (double)ge / len;
      if (p < 1e-9) break;
    }
    e += p;
  }
  return e;
}

int main() {
  edges.resize(M);
  D.resize(M);
  for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];
  for (int i = 0; i < M; i++) {
    cin >> edges[i].first >> edges[i].second;
  }
  for (int i = 0; i < M; i++) {
    int a = edges[i].first, b = edges[i].second;
    double dist = sqrt(1.0 * (X[a] - X[b]) * (X[a] - X[b]) + 1.0 * (Y[a] - Y[b]) * (Y[a] - Y[b]));
    D[i] = round(dist);
  }
  UF mainuf(N);
  for (int i = 0; i < M; i++) {
    int li;
    cin >> li;
    int u = edges[i].first, v = edges[i].second;
    if (mainuf.same(u, v)) {
      cout << 0 << endl;
      continue;
    }
    UF temp = mainuf;
    for (int j = i + 1; j < M; j++) {
      temp.unite(edges[j].first, edges[j].second);
    }
    bool is_safe = (temp.comps() == 1);
    bool take = false;
    if (!is_safe) {
      take = true;
    } else {
      vector<int> comp_u, comp_v;
      int fu = mainuf.find(u);
      int fv = mainuf.find(v);
      for (int p = 0; p < N; p++) {
        int r = mainuf.find(p);
        if (r == fu) comp_u.push_back(p);
        if (r == fv) comp_v.push_back(p);
      }
      vector<int> cross;
      for (int j = i + 1; j < M; j++) {
        int a = edges[j].first, b = edges[j].second;
        int fa = mainuf.find(a);
        int fb = mainuf.find(b);
        if ((fa == fu && fb == fv) || (fa == fv && fb == fu)) cross.push_back(j);
      }
      int k = cross.size();
      if (k > 0) {
        vector<int> cross_d;
        for (int jj : cross) cross_d.push_back(D[jj]);
        double exp_min = compute_emin(cross_d);
        if (li < exp_min) take = true;
      } else {
        vector<vector<pair<int, int>>> g(N);
        for (int j = i + 1; j < M; j++) {
          int a = edges[j].first, b = edges[j].second;
          int w = D[j];
          g[a].push_back({b, w});
          g[b].push_back({a, w});
        }
        vector<long long> dist(N, LLONG_MAX / 2);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        for (int s : comp_u) {
          dist[s] = 0;
          pq.push({0, s});
        }
        while (!pq.empty()) {
          auto [cost, ver] = pq.top();
          pq.pop();
          if (cost > dist[ver]) continue;
          for (auto [to, w] : g[ver]) {
            if (dist[to] > cost + w) {
              dist[to] = cost + w;
              pq.push({dist[to], to});
            }
          }
        }
        long long min_dist = LLONG_MAX;
        for (int t : comp_v) {
          min_dist = min(min_dist, dist[t]);
        }
        double est = 2.0 * min_dist;
        if (li < est) take = true;
      }
    }
    cout << (take ? 1 : 0) << endl;
    if (take) mainuf.unite(u, v);
  }
  return 0;
}