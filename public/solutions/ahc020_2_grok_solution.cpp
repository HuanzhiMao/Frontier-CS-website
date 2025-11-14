#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
  int N, M, K;
  cin >> N >> M >> K;
  vector<pair<int,int>> st(N+1);
  for(int i=1; i<=N; i++){
    cin >> st[i].first >> st[i].second;
  }
  struct E {int u,v,w;};
  vector<E> el(M);
  map<pair<int,int>, int> edge_id;
  for(int j=0; j<M; j++){
    int u,v,w;
    cin >> u >> v >> w;
    el[j] = {u,v,w};
    if(u > v) swap(u,v);
    edge_id[{u,v}] = j;
  }
  vector<pair<int,int>> res(K);
  for(int k=0; k<K; k++){
    cin >> res[k].first >> res[k].second;
  }
  auto get_d2 = [](pair<int,int> p1, pair<int,int> p2) -> ll {
    ll dx = p1.first - p2.first;
    ll dy = p1.second - p2.second;
    return dx*dx + dy*dy;
  };
  auto get_req = [](ll d2) -> int {
    int lo=0, hi=5001;
    while(lo<hi){
      int mid = (lo+hi)/2;
      if((ll)mid*mid >= d2) hi=mid;
      else lo=mid+1;
    }
    return lo;
  };
  vector<pair<int, int>> res_sort(K);
  for(int k=0; k<K; k++){
    int minr = INT_MAX;
    for(int i=1; i<=N; i++){
      ll d2 = get_d2(st[i], res[k]);
      int r = get_req(d2);
      if(r < minr) minr = r;
    }
    res_sort[k] = {minr, k};
  }
  sort(res_sort.rbegin(), res_sort.rend());
  vector<int> P(N+1, 0);
  for(auto& pr : res_sort){
    int kk = pr.second;
    int best_inc = INT_MAX;
    int best_i = -1;
    int best_req = 0;
    for(int i=1; i<=N; i++){
      ll d2 = get_d2(st[i], res[kk]);
      int req = get_req(d2);
      int newp = max(P[i], req);
      int inc = newp*newp - P[i]*P[i];
      if(inc < best_inc || (inc == best_inc && i==1)){
        best_inc = inc;
        best_i = i;
        best_req = req;
      }
    }
    P[best_i] = max(P[best_i], best_req);
  }
  set<int> sset;
  for(int i=1; i<=N; i++){
    if(P[i] > 0) sset.insert(i);
  }
  sset.insert(1);
  vector<int> terms(sset.begin(), sset.end());
  int ts = terms.size();
  if(ts == 1){
    vector<int> B(M,0);
    for(int i=1; i<=N; i++){
      if(i>1) cout << " ";
      cout << P[i];
    }
    cout << "\n";
    for(int j=0; j<M; j++){
      if(j>0) cout << " ";
      cout << B[j];
    }
    cout << "\n";
    return 0;
  }
  const ll INF = LLONG_MAX / 4;
  vector<vector<ll>> dist(N+1, vector<ll>(N+1, INF));
  vector<vector<int>> nxt(N+1, vector<int>(N+1, -1));
  for(int i=1; i<=N; i++) dist[i][i] = 0;
  for(int j=0; j<M; j++){
    int u=el[j].u, v=el[j].v;
    ll ww = el[j].w;
    dist[u][v] = ww;
    dist[v][u] = ww;
    nxt[u][v] = v;
    nxt[v][u] = u;
  }
  for(int k=1; k<=N; k++){
    for(int i=1; i<=N; i++){
      for(int jj=1; jj<=N; jj++){
        if(dist[i][k] >= INF || dist[k][jj] >= INF) continue;
        ll newd = dist[i][k] + dist[k][jj];
        if(newd < dist[i][jj]){
          dist[i][jj] = newd;
          nxt[i][jj] = nxt[i][k];
        }
      }
    }
  }
  struct TEd {int a,b; ll w;};
  vector<TEd> t_el;
  for(int p=0; p<ts; p++){
    for(int q=p+1; q<ts; q++){
      int ii = terms[p], jj = terms[q];
      if(dist[ii][jj] < INF){
        t_el.push_back({p, q, dist[ii][jj]});
      }
    }
  }
  sort(t_el.begin(), t_el.end(), [](const TEd& aa, const TEd& bb){ return aa.w < bb.w; });
  vector<int> t_par(ts);
  iota(t_par.begin(), t_par.end(), 0);
  auto tfind = [&](auto& self, int x) -> int {
    return t_par[x] == x ? x : t_par[x] = self(self, t_par[x]);
  };
  vector<int> B(M,0);
  for(auto& ted : t_el){
    int pa = tfind(tfind, ted.a), pb = tfind(tfind, ted.b);
    if(pa == pb) continue;
    t_par[pa] = pb;
    int s = terms[ted.a];
    int tt = terms[ted.b];
    int cur = s;
    while(cur != tt){
      int nx = nxt[cur][tt];
      assert(nx != -1);
      int uu = min(cur, nx), vv = max(cur, nx);
      int jid = edge_id[{uu,vv}];
      B[jid] = 1;
      cur = nx;
    }
  }
  for(int i=1; i<=N; i++){
    if(i>1) cout << " ";
    cout << P[i];
  }
  cout << "\n";
  for(int j=0; j<M; j++){
    if(j>0) cout << " ";
    cout << B[j];
  }
  cout << "\n";
  return 0;
}