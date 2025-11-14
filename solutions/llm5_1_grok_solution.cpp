#include <bits/stdc++.h>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<int> a(10);
  for (int &x : a) cin >> x;
  vector<vector<int>> g(n + 1);
  vector<int> in_deg(n + 1, 0);
  vector<int> out_deg(n + 1, 0);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    in_deg[v]++;
    out_deg[u]++;
  }
  // Kahn for topo
  vector<int> topo;
  queue<int> q;
  vector<int> indeg_copy = in_deg;
  for (int i = 1; i <= n; i++) {
    if (indeg_copy[i] == 0) q.push(i);
  }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    topo.push_back(u);
    for (int v : g[u]) {
      indeg_copy[v]--;
      if (indeg_copy[v] == 0) q.push(v);
    }
  }
  vector<int> best_path;
  if ((int)topo.size() == n) {
    // DAG, compute longest path
    vector<int> max_len(n + 1, 0);
    vector<int> next_v(n + 1, -1);
    for (int i = n - 1; i >= 0; i--) {
      int u = topo[i];
      max_len[u] = 1;
      for (int v : g[u]) {
        if (max_len[v] + 1 > max_len[u]) {
          max_len[u] = max_len[v] + 1;
          next_v[u] = v;
        }
      }
    }
    // find start with max
    int start = -1;
    int mx = 0;
    for (int i = 1; i <= n; i++) {
      if (max_len[i] > mx) {
        mx = max_len[i];
        start = i;
      }
    }
    // reconstruct
    vector<int> path;
    int cur = start;
    while (cur != -1) {
      path.push_back(cur);
      cur = next_v[cur];
    }
    best_path = path;
  } else {
    // not DAG, use greedy
    vector<int> starts;
    int min_indeg = INT_MAX;
    for (int i = 1; i <= n; i++) {
      min_indeg = min(min_indeg, in_deg[i]);
    }
    for (int i = 1; i <= n; i++) {
      if (in_deg[i] == min_indeg) starts.push_back(i);
    }
    // sort starts by out_deg desc
    sort(starts.begin(), starts.end(), [&](int x, int y) {
      return out_deg[x] > out_deg[y];
    });
    int num_tries = min(10, (int)starts.size());
    int max_size = 0;
    for (int i = 0; i < num_tries; i++) {
      int s = starts[i];
      vector<int> path = {s};
      vector<bool> visited(n + 1, false);
      visited[s] = true;
      bool extended = true;
      while (extended) {
        extended = false;
        int cur = path.back();
        int best_v = -1;
        int best_score = -1;
        for (int v : g[cur]) {
          if (!visited[v] && out_deg[v] > best_score) {
            best_score = out_deg[v];
            best_v = v;
          }
        }
        if (best_v != -1) {
          path.push_back(best_v);
          visited[best_v] = true;
          extended = true;
        }
      }
      if (path.size() > max_size) {
        max_size = path.size();
        best_path = path;
      }
    }
  }
  if (best_path.empty()) {
    // fallback, at least one vertex
    best_path = {1};
  }
  cout << best_path.size() << endl;
  for (int x : best_path) cout << x << " ";
  cout << endl;
  return 0;
}