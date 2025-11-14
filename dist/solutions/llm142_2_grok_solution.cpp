#include <bits/stdc++.h>
using namespace std;

struct Edge {
  int to;
  long long cap, cost;
  int rev;
};

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> initial(n);
  for (int i = 0; i < n; i++) {
    initial[i].resize(m);
    for (int &x : initial[i]) cin >> x;
  }
  vector<vector<int>> cnt(n + 2, vector<int>(n + 2, 0));
  for (int p = 1; p <= n; p++) {
    for (int ball : initial[p - 1]) {
      cnt[p][ball]++;
    }
  }
  int V = 2 * n + 3;
  int S = 0, T = 2 * n + 2;
  vector<vector<Edge>> g(V);
  auto add_edge = [&](int from, int to, long long cap, long long cost) {
    g[from].push_back({to, cap, cost, (int)g[to].size()});
    g[to].push_back({from, 0, -cost, (int)g[from].size() - 1});
  };
  for (int c = 1; c <= n; c++) {
    add_edge(S, c, 1, 0);
  }
  for (int p = 1; p <= n + 1; p++) {
    add_edge(n + p, T, 1, 0);
  }
  for (int c = 1; c <= n; c++) {
    for (int p = 1; p <= n + 1; p++) {
      add_edge(c, n + p, 1, -cnt[p][c]);
    }
  }
  vector<long long> h(V, 0);
  const long long INF = 1LL << 60;
  vector<long long> dist(V, INF);
  dist[S] = 0;
  bool update = true;
  for (int iter = 0; iter < V && update; iter++) {
    update = false;
    for (int v = 0; v < V; v++) {
      if (dist[v] == INF) continue;
      for (int i = 0; i < g[v].size(); i++) {
        Edge &e = g[v][i];
        if (e.cap > 0 && dist[e.to] > dist[v] + e.cost) {
          dist[e.to] = dist[v] + e.cost;
          update = true;
        }
      }
    }
  }
  for (int v = 0; v < V; v++) h[v] = dist[v];
  auto min_cost_flow = [&](int f) -> long long {
    long long res = 0;
    while (f > 0) {
      vector<long long> ddist(V, INF);
      ddist[S] = 0;
      vector<int> prevv(V, -1), preve(V, -1);
      priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
      pq.push({0, S});
      while (!pq.empty()) {
        auto [costt, v] = pq.top();
        pq.pop();
        if (ddist[v] < costt) continue;
        for (int i = 0; i < g[v].size(); i++) {
          Edge &e = g[v][i];
          if (e.cap > 0) {
            long long nc = costt + e.cost + h[v] - h[e.to];
            if (nc < ddist[e.to]) {
              ddist[e.to] = nc;
              prevv[e.to] = v;
              preve[e.to] = i;
              pq.push({nc, e.to});
            }
          }
        }
      }
      if (ddist[T] == INF) return -1;
      for (int v = 0; v < V; v++)
        if (ddist[v] < INF) h[v] += ddist[v];
      long long dd = f;
      for (int v = T; v != S; v = prevv[v]) {
        dd = min(dd, g[prevv[v]][preve[v]].cap);
      }
      f -= dd;
      res += dd * h[T];
      for (int v = T; v != S; v = prevv[v]) {
        Edge &e = g[prevv[v]][preve[v]];
        e.cap -= dd;
        g[e.to][e.rev].cap += dd;
      }
    }
    return res;
  };
  min_cost_flow(n);
  vector<int> target(n + 2, 0);
  for (int c = 1; c <= n; c++) {
    for (auto &e : g[c]) {
      if (e.to > n && e.to <= 2 * n + 1 && e.cap == 0) {
        int p = e.to - n;
        target[c] = p;
        break;
      }
    }
  }
  int auxx = -1;
  set<int> used;
  for (int c = 1; c <= n; c++) used.insert(target[c]);
  for (int p = 1; p <= n + 1; p++)
    if (used.count(p) == 0) auxx = p;
  vector<vector<int>> poles(n + 2);
  for (int p = 1; p <= n; p++) {
    poles[p] = initial[p - 1];
  }
  vector<vector<int>> current_count(n + 2, vector<int>(n + 2, 0));
  vector<int> pole_size(n + 2, 0);
  for (int p = 1; p <= n + 1; p++) {
    pole_size[p] = poles[p].size();
    for (int ball : poles[p]) {
      current_count[p][ball]++;
    }
  }
  vector<set<int>> poles_with(n + 2);
  for (int c = 1; c <= n; c++) {
    for (int p = 1; p <= n + 1; p++) {
      if (current_count[p][c] > 0) poles_with[c].insert(p);
    }
  }
  vector<int> color_for_pole(n + 2, 0);
  for (int c = 1; c <= n; c++) color_for_pole[target[c]] = c;
  vector<pair<int, int>> moves_list;
  auto perform_move = [&](int x, int y) {
    int col = poles[x].back();
    poles[x].pop_back();
    poles[y].push_back(col);
    moves_list.push_back({x, y});
    pole_size[x]--;
    pole_size[y]++;
    current_count[x][col]--;
    if (current_count[x][col] == 0) poles_with[col].erase(x);
    current_count[y][col]++;
    if (current_count[y][col] == 1) poles_with[col].insert(y);
  };
  auto is_done_func = [&]() -> bool {
    for (int c = 1; c <= n; c++) {
      if (poles_with[c].size() != 1) return false;
      int p = *poles_with[c].begin();
      if (current_count[p][c] != m || pole_size[p] != current_count[p][c]) return false;
    }
    return true;
  };
  int safeguard = 0;
  while (safeguard++ < 3000000) {
    if (is_done_func()) break;
    bool did = false;
    for (int x = 1; x <= n + 1 && !did; x++) {
      if (pole_size[x] == 0) continue;
      int col = poles[x].back();
      int targg = target[col];
      if (targg != x && pole_size[targg] <= m - 1) {
        perform_move(x, targg);
        did = true;
      }
    }
    if (did) continue;
    for (int x = 1; x <= n + 1 && !did; x++) {
      if (x == auxx || pole_size[x] == 0) continue;
      int col = poles[x].back();
      int targg = target[col];
      if (targg != x && pole_size[auxx] <= m - 1) {
        perform_move(x, auxx);
        did = true;
      }
    }
    if (did) continue;
    if (pole_size[auxx] > m - 1) continue;
    int chosen = -1;
    int min_s = INT_MAX;
    for (int p = 1; p <= n + 1; p++) {
      if (p == auxx || pole_size[p] == 0) continue;
      int c = color_for_pole[p];
      if (c == 0) continue;
      int col = poles[p].back();
      if (col != c) continue;
      if (pole_size[p] > current_count[p][c]) {
        if (pole_size[p] < min_s) {
          min_s = pole_size[p];
          chosen = p;
        }
      }
    }
    if (chosen != -1) {
      perform_move(chosen, auxx);
      did = true;
    }
    if (!did) {
      break;
    }
  }
  cout << moves_list.size() << endl;
  for (auto [x, y] : moves_list) {
    cout << x << " " << y << endl;
  }
  return 0;
}