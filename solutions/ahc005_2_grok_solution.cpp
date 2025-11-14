#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
  int N, si, sj;
  cin >> N >> si >> sj;
  vector<string> g(N);
  for(int i=0; i<N; i++) cin >> g[i];
  vector<vector<pair<int,int>>> horiz_segs, vert_segs;
  for(int i=0; i<N; i+=2) {
    int jj=0;
    while(jj < N) {
      if(g[i][jj] == '#') { jj++; continue; }
      int l = jj;
      while(jj < N && g[i][jj] != '#') jj++;
      int r = jj-1;
      if(r - l +1 >= 2) {
        vector<pair<int,int>> seg;
        for(int c=l; c<=r; c++) seg.emplace_back(i,c);
        horiz_segs.push_back(seg);
      }
    }
  }
  for(int j=0; j<N; j+=2) {
    int ii=0;
    while(ii < N) {
      if(g[ii][j] == '#') { ii++; continue; }
      int u = ii;
      while(ii < N && g[ii][j] != '#') ii++;
      int d = ii-1;
      if(d - u +1 >= 2) {
        vector<pair<int,int>> seg;
        for(int r=u; r<=d; r++) seg.emplace_back(r,j);
        vert_segs.push_back(seg);
      }
    }
  }
  set<pair<int,int>> must_visit_set;
  must_visit_set.insert({si, sj});
  auto add_rep = [&](const vector<vector<pair<int,int>>>& segs) {
    for(const auto& seg : segs) {
      bool has_start = false;
      for(const auto& p : seg) {
        if(p.first == si && p.second == sj) { has_start = true; break; }
      }
      if(!has_start) {
        int sz = seg.size();
        auto rep = seg[sz/2];
        must_visit_set.insert(rep);
      }
    }
  };
  add_rep(horiz_segs);
  add_rep(vert_segs);
  vector<pair<int,int>> cities(must_visit_set.begin(), must_visit_set.end());
  int VV = cities.size();
  int start_idx = -1;
  for(int k=0; k<VV; k++) {
    if(cities[k].first == si && cities[k].second == sj) {
      start_idx = k;
      break;
    }
  }
  vector<pair<int,int>> all_roads;
  int ID[70][70];
  memset(ID, -1, sizeof(ID));
  for(int i=0; i<N; i++) for(int j=0; j<N; j++) if(g[i][j] != '#') {
    ID[i][j] = all_roads.size();
    all_roads.emplace_back(i,j);
  }
  int r = all_roads.size();
  vector<vector<pair<int,int>>> graph(r);
  int dx[4] = {-1,0,1,0};
  int dy[4] = {0,1,0,-1};
  char dir[4] = {'U','R','D','L'};
  for(int u=0; u<r; u++) {
    auto [x,y] = all_roads[u];
    for(int d=0; d<4; d++) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if(nx>=0 && nx<N && ny>=0 && ny<N && g[nx][ny] != '#') {
        int v = ID[nx][ny];
        int cst = g[nx][ny] - '0';
        graph[u].emplace_back(v, cst);
      }
    }
  }
  vector<vector<ll>> city_dist(VV, vector<ll>(VV, LLONG_MAX/2));
  vector<vector<int>> par(VV, vector<int>(r, -1));
  for(int src=0; src<VV; src++) {
    int s_node = ID[ cities[src].first ][ cities[src].second ];
    vector<ll> dist(r, LLONG_MAX/2);
    vector<int> prev(r, -1);
    priority_queue< pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>> > pq;
    dist[s_node] = 0;
    pq.push({0, s_node});
    while(!pq.empty()) {
      auto [cost, u] = pq.top(); pq.pop();
      if(cost > dist[u]) continue;
      for(auto [v, c] : graph[u]) {
        if(dist[v] > dist[u] + c) {
          dist[v] = dist[u] + c;
          prev[v] = u;
          pq.push({dist[v], v});
        }
      }
    }
    par[src] = prev;
    for(int tgt=0; tgt<VV; tgt++) {
      int t_node = ID[ cities[tgt].first ][ cities[tgt].second ];
      city_dist[src][tgt] = dist[t_node];
    }
  }
  vector<ll> min_e(VV, LLONG_MAX/2);
  vector<int> mst_par(VV, -1);
  vector<bool> in_mst(VV, false);
  priority_queue< pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>> > pq_mst;
  min_e[start_idx] = 0;
  pq_mst.push({0, start_idx});
  while(!pq_mst.empty()) {
    auto [cost, u] = pq_mst.top(); pq_mst.pop();
    if(in_mst[u]) continue;
    in_mst[u] = true;
    for(int v=0; v<VV; v++) {
      if(!in_mst[v] && city_dist[u][v] < min_e[v]) {
        min_e[v] = city_dist[u][v];
        mst_par[v] = u;
        pq_mst.push({min_e[v], v});
      }
    }
  }
  vector<vector<int>> mgraph(VV);
  auto add_edge = [&](int a, int b) {
    mgraph[a].push_back(b);
    mgraph[b].push_back(a);
  };
  for(int v=0; v<VV; v++) {
    if(mst_par[v] != -1) {
      add_edge(mst_par[v], v);
      add_edge(mst_par[v], v);
    }
  }
  vector<int> euler;
  function<void(int)> dfs_euler = [&](int u) {
    while(!mgraph[u].empty()) {
      int v = mgraph[u].back();
      mgraph[u].pop_back();
      auto it = find(mgraph[v].begin(), mgraph[v].end(), u);
      if(it != mgraph[v].end()) {
        mgraph[v].erase(it);
      }
      dfs_euler(v);
    }
    euler.push_back(u);
  };
  dfs_euler(start_idx);
  reverse(euler.begin(), euler.end());
  vector<int> tsp_tour;
  unordered_set<int> vis_tsp;
  for(int c : euler) {
    if(vis_tsp.count(c) == 0) {
      tsp_tour.push_back(c);
      vis_tsp.insert(c);
    }
  }
  tsp_tour.push_back(start_idx);
  string route = "";
  int opposite[4] = {2,3,0,1};
  for(size_t p=0; p < tsp_tour.size() - 1; p++) {
    int src = tsp_tour[p];
    int tgt = tsp_tour[p+1];
    int s_node = ID[ cities[src].first ][ cities[src].second ];
    int t_node = ID[ cities[tgt].first ][ cities[tgt].second ];
    vector<int> path_nodes;
    int curr = t_node;
    while(curr != -1) {
      path_nodes.push_back(curr);
      curr = par[src][curr];
    }
    if(path_nodes.back() != s_node) {
      assert(false);
    }
    reverse(path_nodes.begin(), path_nodes.end());
    for(size_t q=0; q+1 < path_nodes.size(); q++) {
      auto [fx, fy] = all_roads[ path_nodes[q] ];
      auto [tx, ty] = all_roads[ path_nodes[q+1] ];
      int dd = -1;
      if(tx == fx-1 && ty == fy) dd=0;
      else if(tx == fx && ty == fy+1) dd=1;
      else if(tx == fx+1 && ty == fy) dd=2;
      else if(tx == fx && ty == fy-1) dd=3;
      assert(dd != -1);
      route += dir[dd];
    }
  }
  if(route.empty()) {
    for(int d=0; d<4; d++) {
      int nx = si + dx[d];
      int ny = sj + dy[d];
      if(nx>=0 && nx<N && ny>=0 && ny<N && g[nx][ny] != '#') {
        route += dir[d];
        int back_d = opposite[d];
        route += dir[back_d];
        break;
      }
    }
  }
  cout << route << endl;
  return 0;
}