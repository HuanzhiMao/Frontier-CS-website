#include <bits/stdc++.h>
using namespace std;

int n;

int query_induced(const vector<int>& S) {
  if (S.empty()) return 0;
  int k = S.size();
  if (k == 0) return 0; // though shouldn't happen
  cout << "? " << k << endl;
  for (int v : S) cout << v << " ";
  cout << endl;
  cout.flush();
  int m;
  cin >> m;
  if (m == -1) exit(0);
  return m;
}

int count_neighbors(int u, const vector<int>& T) {
  if (T.empty()) return 0;
  int e_t = query_induced(T);
  vector<int> ut = T;
  ut.push_back(u);
  int e_ut = query_induced(ut);
  return e_ut - e_t;
}

vector<int> get_path(int u, int v, const vector<int>& parent) {
  vector<int> path_u;
  int cu = u;
  while (cu != -1) {
    path_u.push_back(cu);
    cu = parent[cu];
  }
  vector<int> path_v;
  int cv = v;
  while (cv != -1) {
    path_v.push_back(cv);
    cv = parent[cv];
  }
  reverse(path_u.begin(), path_u.end());
  reverse(path_v.begin(), path_v.end());
  int minlen = min(path_u.size(), path_v.size());
  int lca_idx = 0;
  for (int i = 0; i < minlen; ++i) {
    if (path_u[i] == path_v[i]) lca_idx = i;
    else break;
  }
  vector<int> path;
  for (int i = path_u.size() - 1; i >= lca_idx; --i) {
    path.push_back(path_u[i]);
  }
  for (int i = lca_idx + 1; i < path_v.size(); ++i) {
    path.push_back(path_v[i]);
  }
  return path;
}

int main() {
  cin >> n;
  if (n == 1) {
    cout << "Y 1" << endl << "1" << endl;
    return 0;
  }
  vector<int> unvisited_vec;
  for (int i = 2; i <= n; ++i) unvisited_vec.push_back(i);
  vector<int> visited_vec = {1};
  vector<int> parent(n + 1, -1);
  vector<int> color(n + 1, -1);
  color[1] = 0;

  while (visited_vec.size() < n) {
    vector<int> U_vec = unvisited_vec;
    int e_U = query_induced(U_vec);

    // Find u in visited_vec with cross > 0 to U_vec
    vector<int> cand = visited_vec;
    while (cand.size() > 1) {
      int mid = cand.size() / 2;
      vector<int> left(cand.begin(), cand.begin() + mid);
      vector<int> wu = left;
      wu.insert(wu.end(), U_vec.begin(), U_vec.end());
      int e_wu = query_induced(wu);
      int e_w = query_induced(left);
      int cross = e_wu - e_w - e_U;
      if (cross > 0) {
        cand = left;
      } else {
        cand.assign(cand.begin() + mid, cand.end());
      }
    }
    int found_u = cand[0];

    // Find v in U_vec connected to found_u
    vector<int> candv = U_vec;
    while (candv.size() > 1) {
      int mid = candv.size() / 2;
      vector<int> left(candv.begin(), candv.begin() + mid);
      int cnt = count_neighbors(found_u, left);
      if (cnt > 0) {
        candv = left;
      } else {
        candv.assign(candv.begin() + mid, candv.end());
      }
    }
    int v_new = candv[0];

    // Add v_new to tree
    parent[v_new] = found_u;
    color[v_new] = 1 - color[found_u];
    visited_vec.push_back(v_new);

    // Remove v_new from unvisited_vec
    auto it = find(unvisited_vec.begin(), unvisited_vec.end(), v_new);
    if (it != unvisited_vec.end()) {
      *it = unvisited_vec.back();
      unvisited_vec.pop_back();
    }
  }

  // Now build partitions
  vector<int> part0, part1;
  for (int i = 1; i <= n; ++i) {
    if (color[i] == 0) part0.push_back(i);
    else part1.push_back(i);
  }

  int e0 = (part0.size() >= 1 ? query_induced(part0) : 0);
  int e1 = (part1.size() >= 1 ? query_induced(part1) : 0);

  if (e0 == 0 && e1 == 0) {
    // Bipartite
    if (part0.size() <= part1.size()) {
      cout << "Y " << part1.size() << endl;
      for (int x : part1) cout << x << " ";
      cout << endl;
    } else {
      cout << "Y " << part0.size() << endl;
      for (int x : part0) cout << x << " ";
      cout << endl;
    }
  } else {
    // Not bipartite, find odd cycle in the part with edges
    vector<int> AA;
    int e_AA;
    if (e0 > 0) {
      AA = part0;
      e_AA = e0;
    } else {
      AA = part1;
      e_AA = e1;
    }

    // Find u in AA with deg > 0 in AA
    vector<int> cand = AA;
    set<int> aa_set(AA.begin(), AA.end());
    while (cand.size() > 1) {
      int mid = cand.size() / 2;
      vector<int> left(cand.begin(), cand.begin() + mid);
      int e_left = query_induced(left);

      // Build minus_left = AA - left
      set<int> left_set(left.begin(), left.end());
      vector<int> minus_left;
      for (int x : AA) {
        if (left_set.find(x) == left_set.end()) minus_left.push_back(x);
      }
      int e_minus = query_induced(minus_left);

      int val = e_left + e_AA - e_minus;
      if (val > 0) {
        cand = left;
      } else {
        cand.assign(cand.begin() + mid, cand.end());
      }
    }
    int found_u = cand[0];

    // Find v in AA - {found_u} connected to found_u
    vector<int> S;
    for (int x : AA) if (x != found_u) S.push_back(x);
    vector<int> candv = S;
    while (candv.size() > 1) {
      int mid = candv.size() / 2;
      vector<int> left(candv.begin(), candv.begin() + mid);
      int cnt = count_neighbors(found_u, left);
      if (cnt > 0) {
        candv = left;
      } else {
        candv.assign(candv.begin() + mid, candv.end());
      }
    }
    int found_v = candv[0];

    // Get path from found_u to found_v
    vector<int> cycle_path = get_path(found_u, found_v, parent);

    // Output
    cout << "N " << cycle_path.size() << endl;
    for (int x : cycle_path) cout << x << " ";
    cout << endl;
  }
  return 0;
}