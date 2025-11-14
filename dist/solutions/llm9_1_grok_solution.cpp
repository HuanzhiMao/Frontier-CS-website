#include <bits/stdc++.h>
using namespace std;

int main() {
  int T;
  cin >> T;
  for (int t = 0; t < T; t++) {
    int n;
    cin >> n;
    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i];
    vector<pair<int, int>> edge_list(n);
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 1; i <= n - 1; i++) {
      int u, v;
      cin >> u >> v;
      edge_list[i] = {u, v};
      adj[u].emplace_back(v, i);
      adj[v].emplace_back(u, i);
    }
    int LOG = 10;
    vector<vector<int>> anc(n + 1, vector<int>(LOG, 0));
    vector<int> dep(n + 1);
    function<void(int, int, int)> dfs = [&](int node, int par, int d) {
      dep[node] = d;
      anc[node][0] = par;
      for (int i = 1; i < LOG; i++) {
        if (anc[node][i - 1] != 0) anc[node][i] = anc[anc[node][i - 1]][i - 1];
        else anc[node][i] = 0;
      }
      for (auto [ch, idx] : adj[node])
        if (ch != par) dfs(ch, node, d + 1);
    };
    dfs(1, 0, 0);
    auto get_lca = [&](int a, int b) -> int {
      if (dep[a] > dep[b]) swap(a, b);
      int diff = dep[b] - dep[a];
      for (int i = 0; i < LOG; i++)
        if ((diff >> i) & 1) b = anc[b][i];
      if (a == b) return a;
      for (int i = LOG - 1; i >= 0; i--)
        if (anc[a][i] != anc[b][i]) {
          a = anc[a][i];
          b = anc[b][i];
        }
      return anc[a][0];
    };
    auto get_dist = [&](int a, int b) -> int {
      int l = get_lca(a, b);
      return dep[a] + dep[b] - 2 * dep[l];
    };
    vector<vector<int>> operations;
    vector<long long> U(n + 1), M(n + 1);
    vector<int> choice(n + 1, -1);
    const long long HIGH = 502;
    const long long LOW = 1;
    int max_steps = 10 * n;
    for (int step = 0; step < max_steps; step++) {
      bool issorted = true;
      for (int i = 1; i <= n; i++)
        if (p[i] != i) issorted = false;
      if (issorted) break;
      vector<vector<long long>> ww(n + 1, vector<long long>(n + 1, 0));
      for (int i = 1; i <= n - 1; i++) {
        int u = edge_list[i].first, v = edge_list[i].second;
        bool want_uv = (p[u] != u) && (get_dist(v, p[u]) == get_dist(u, p[u]) - 1);
        bool want_vu = (p[v] != v) && (get_dist(u, p[v]) == get_dist(v, p[v]) - 1);
        int cnt = want_uv + want_vu;
        long long val = 0;
        if (cnt == 2) val = HIGH;
        else if (cnt == 1) val = LOW;
        ww[u][v] = ww[v][u] = val;
      }
      function<void(int, int)> compute = [&](int node, int par) {
        vector<int> children;
        for (auto [ch, idx] : adj[node])
          if (ch != par) children.push_back(ch);
        long long sum_m = 0;
        for (int c : children) {
          compute(c, node);
          sum_m += M[c];
        }
        long long uu = sum_m;
        long long mm = uu;
        choice[node] = -1;
        for (int c : children) {
          long long opt = ww[node][c] + U[c] + sum_m - M[c];
          if (opt > mm) {
            mm = opt;
            choice[node] = c;
          }
        }
        U[node] = uu;
        M[node] = mm;
      };
      compute(1, 0);
      if (M[1] <= 0) break;
      vector<int> this_op;
      std::function<void(int, int)> collectM = [&](int node, int par) {
        if (choice[node] == -1) {
          for (auto [ch, idx] : adj[node])
            if (ch != par) collectM(ch, node);
          return;
        }
        int c = choice[node];
        for (auto [nb, idx] : adj[node])
          if (nb == c) {
            this_op.push_back(idx);
            break;
          }
        std::function<void(int, int)> collectU = [&](int node, int par) {
          for (auto [ch, idx] : adj[node])
            if (ch != par) collectM(ch, node);
        };
        for (auto [ch, idx] : adj[node])
          if (ch != par) {
            if (ch == c)
              collectU(ch, node);
            else
              collectM(ch, node);
          }
      };
      collectM(1, 0);
      operations.push_back(this_op);
      for (int eid : this_op) {
        int u = edge_list[eid].first, v = edge_list[eid].second;
        swap(p[u], p[v]);
      }
    }
    cout << operations.size() << endl;
    for (auto& op : operations) {
      cout << op.size();
      for (int e : op) cout << " " << e;
      cout << endl;
    }
  }
  return 0;
}