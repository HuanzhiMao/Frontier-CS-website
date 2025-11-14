#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n, comps;
    vector<int> p, sz;
    DSU() {}
    DSU(int n): n(n), comps(n), p(n), sz(n,1) { iota(p.begin(), p.end(), 0); }
    int find(int a){ return p[a]==a?a:p[a]=find(p[a]); }
    bool unite(int a, int b){
        a=find(a); b=find(b);
        if(a==b) return false;
        if(sz[a]<sz[b]) swap(a,b);
        p[b]=a; sz[a]+=sz[b]; comps--;
        return true;
    }
};

struct Edge {
    int u, v;
    int d;
    bool is_mst = false;
};

static inline int round_dist(int x1, int y1, int x2, int y2){
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    double dd = sqrt((double)dx*dx + (double)dy*dy);
    return (int)(dd + 0.5);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 400;
    const int M = 1995;

    vector<int> x(N), y(N);
    for(int i=0;i<N;i++){
        if(!(cin>>x[i]>>y[i])) return 0;
    }

    vector<Edge> edges(M);
    for(int i=0;i<M;i++){
        int u,v; cin>>u>>v;
        edges[i].u = u;
        edges[i].v = v;
    }
    for(int i=0;i<M;i++){
        edges[i].d = round_dist(x[edges[i].u], y[edges[i].u], x[edges[i].v], y[edges[i].v]);
    }

    // Compute MST based on d using Kruskal
    vector<int> ord(M);
    iota(ord.begin(), ord.end(), 0);
    stable_sort(ord.begin(), ord.end(), [&](int a, int b){
        if(edges[a].d != edges[b].d) return edges[a].d < edges[b].d;
        return a < b;
    });
    DSU dsu_mst(N);
    int picked = 0;
    for(int idx: ord){
        if(dsu_mst.unite(edges[idx].u, edges[idx].v)){
            edges[idx].is_mst = true;
            picked++;
            if(picked == N-1) break;
        }
    }

    DSU cur(N);

    auto safe_to_skip = [&](int idx)->bool{
        // Check if remaining edges (idx+1..M-1) can connect the current components
        DSU tmp = cur;
        if(tmp.comps == 1) return true;
        for(int j = idx+1; j < M; j++){
            int u = edges[j].u, v = edges[j].v;
            if(tmp.unite(u,v)){
                if(tmp.comps == 1) return true;
            }
        }
        return tmp.comps == 1;
    };

    auto threshold_for = [&](int comps, int edges_left, bool is_mst)->double{
        if(comps <= 1) return 0.0;
        double k = edges_left / (double)(comps - 1); // edges per needed connection
        double t;
        // Map k in [3,20] to t in [1.85, 1.10], clamp outside
        if(k <= 3.0) t = 1.85;
        else if(k >= 20.0) t = 1.10;
        else t = 1.85 - (k - 3.0) * (0.75 / 17.0);
        if(is_mst) t += 0.20;
        if(t < 1.0) t = 1.0;
        if(t > 2.4) t = 2.4;
        return t;
    };

    for(int i=0;i<M;i++){
        int l;
        if(!(cin>>l)) return 0;

        int u = edges[i].u, v = edges[i].v;
        int du = cur.find(u), dv = cur.find(v);
        if(du == dv){
            cout << 0 << endl;
            continue;
        }

        int d = edges[i].d;
        // Always accept if at minimum possible length
        bool adopt = false;
        if(l <= d) {
            adopt = true;
        } else {
            int comps = cur.comps;
            int edges_left = M - i; // including current edge decision time
            double thr = threshold_for(comps, edges_left, edges[i].is_mst);
            double ratio = (double)l / (double)max(1, d);
            if(ratio <= thr){
                adopt = true;
            } else {
                // Only check connectivity if we plan to skip
                if(!safe_to_skip(i)){
                    adopt = true;
                }
            }
        }

        if(adopt){
            cout << 1 << endl;
            cur.unite(u, v);
        }else{
            cout << 0 << endl;
        }
    }

    return 0;
}