#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, M, H;
  cin >> N >> M >> H;
  vector<int> A(N);
  for (int &i : A) cin >> i;
  vector<vector<int>> G(N);
  for (int i = 0; i < M; i++) {
    int u, v;
    cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  vector<int> X(N), Y(N);
  for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];
  for (int u = 0; u < N; u++) {
    sort(G[u].begin(), G[u].end(), [&](int a, int b) {
      double ang1 = atan2(Y[a] - Y[u], X[a] - X[u]);
      double ang2 = atan2(Y[b] - Y[u], X[b] - X[u]);
      return ang1 < ang2;
    });
  }
  vector<int> p(N, -1);
  set<int> active;
  for (int i = 0; i < N; i++) active.insert(i);
  vector<vector<int>> paths;
  while (!active.empty()) {
    int start = *active.begin();
    vector<int> dist(N, -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    vector<int> par(N, -1);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : G[u]) {
        if (active.count(v) == 0) continue;
        if (dist[v] == -1) {
          dist[v] = dist[u] + 1;
          par[v] = u;
          q.push(v);
        }
      }
    }
    int maxd = 0, p1 = start;
    for (int i = 0; i < N; i++) if (dist[i] > maxd) {
      maxd = dist[i];
      p1 = i;
    }
    vector<bool> visited(N, false);
    vector<int> tree_par(N, -1);
    vector<vector<int>> tree_adj(N);
    auto dfs_func = [&](auto self, int u) -> void {
      visited[u] = true;
      for (int v : G[u]) {
        if (active.count(v) && !visited[v]) {
          tree_par[v] = u;
          tree_adj[u].push_back(v);
          tree_adj[v].push_back(u);
          self(self, v);
        }
      }
    };
    dfs_func(dfs_func, p1);
    vector<int> tdist(N, -1);
    queue<int> tq;
    tq.push(p1);
    tdist[p1] = 0;
    vector<int> tpar(N, -1);
    while (!tq.empty()) {
      int u = tq.front(); tq.pop();
      for (int v : tree_adj[u]) {
        if (tdist[v] == -1) {
          tdist[v] = tdist[u] + 1;
          tpar[v] = u;
          tq.push(v);
        }
      }
    }
    int tmaxd = 0, q1 = p1;
    for (int i = 0; i < N; i++) if (tdist[i] > tmaxd && visited[i]) {
      tmaxd = tdist[i];
      q1 = i;
    }
    fill(tdist.begin(), tdist.end(), -1);
    fill(tpar.begin(), tpar.end(), -1);
    tq.push(q1);
    tdist[q1] = 0;
    while (!tq.empty()) {
      int u = tq.front(); tq.pop();
      for (int v : tree_adj[u]) {
        if (tdist[v] == -1) {
          tdist[v] = tdist[u] + 1;
          tpar[v] = u;
          tq.push(v);
        }
      }
    }
    int tmaxd2 = 0, q2 = q1;
    for (int i = 0; i < N; i++) if (tdist[i] > tmaxd2 && visited[i]) {
      tmaxd2 = tdist[i];
      q2 = i;
    }
    vector<int> path;
    int curr = q2;
    while (curr != -1) {
      path.push_back(curr);
      curr = tpar[curr];
    }
    reverse(path.begin(), path.end());
    paths.push_back(path);
    for (int v : path) active.erase(v);
  }
  for (auto &path : paths) {
    int s = path.size();
    if (s == 1) {
      p[path[0]] = -1;
      continue;
    }
    int max_size = 2 * H + 1;
    vector<vector<int>> segments;
    int idx = 0;
    while (idx < s) {
      int len = min(s - idx, max_size);
      vector<int> seg(path.begin() + idx, path.begin() + idx + len);
      segments.push_back(seg);
      idx += len;
    }
    for (auto &seg : segments) {
      int t = seg.size();
      long long best_sum = -1;
      int best_k = -1;
      for (int k = 0; k < t; k++) {
        if (k <= H && (t - 1 - k) <= H) {
          long long cursum = 0;
          for (int i = 0; i < t; i++) {
            cursum += (long long)abs(i - k) * A[seg[i]];
          }
          if (cursum > best_sum) {
            best_sum = cursum;
            best_k = k;
          }
        }
      }
      int root = seg[best_k];
      p[root] = -1;
      for (int i = best_k - 1; i >= 0; i--) {
        p[seg[i]] = seg[i + 1];
      }
      for (int i = best_k + 1; i < t; i++) {
        p[seg[i]] = seg[i - 1];
      }
    }
  }
  for (int i = 0; i < N; i++) {
    if (i) cout << " ";
    cout << p[i];
  }
  cout << endl;
  return 0;
}