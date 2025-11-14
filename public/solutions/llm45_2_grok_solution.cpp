#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false); cin.tie(0);
  int n, m, k; double eps;
  cin >> n >> m >> k >> eps;
  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    if (u == v) continue;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  for (int i = 1; i <= n; i++) {
    sort(adj[i].begin(), adj[i].end());
    auto it = unique(adj[i].begin(), adj[i].end());
    adj[i].erase(it, adj[i].end());
  }
  vector<int> dist(n + 1, -1);
  auto bfs_dist = [&](int start) -> int {
    fill(dist.begin(), dist.end(), -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    int last = start;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      last = u;
      for (int v : adj[u]) {
        if (dist[v] == -1) {
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
    }
    return last;
  };
  srand(time(0));
  int v1 = rand() % n + 1;
  int v2 = bfs_dist(v1);
  int v3 = bfs_dist(v2);
  vector<int> order;
  vector<bool> visited(n + 1, false);
  auto do_bfs = [&](int start) {
    if (visited[start]) return;
    queue<int> q;
    q.push(start);
    visited[start] = true;
    order.push_back(start);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : adj[u]) {
        if (!visited[v]) {
          visited[v] = true;
          order.push_back(v);
          q.push(v);
        }
      }
    }
  };
  do_bfs(v3);
  vector<int> remain;
  for (int i = 1; i <= n; i++) if (!visited[i]) remain.push_back(i);
  random_shuffle(remain.begin(), remain.end());
  for (int st : remain) {
    do_bfs(st);
  }
  vector<int> part(n + 1);
  int base = n / k;
  int remd = n % k;
  int idx = 0;
  for (int p = 1; p <= k; p++) {
    int sz = base + (p <= remd ? 1 : 0);
    for (int j = 0; j < sz; j++) {
      int v = order[idx++];
      part[v] = p;
    }
  }
  for (int i = 1; i <= n; i++) {
    if (i > 1) cout << " ";
    cout << part[i];
  }
  cout << endl;
}