#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n;
    vector<int> p, r;
    DSU(int n=0): n(n), p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x? x : p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return false;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
        return true;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N,M,K;
    if(!(cin>>N>>M>>K)) return 0;
    vector<int> x(N), y(N);
    for(int i=0;i<N;i++) cin>>x[i]>>y[i];
    vector<int> U(M), V(M);
    vector<long long> W(M);
    vector<vector<pair<int,int>>> adj(N);
    for(int j=0;j<M;j++){
        int u,v; long long w;
        cin>>u>>v>>w;
        u--; v--;
        U[j]=u; V[j]=v; W[j]=w;
        adj[u].push_back({v,j});
        adj[v].push_back({u,j});
    }
    vector<int> a(K), b(K);
    for(int k=0;k<K;k++) cin>>a[k]>>b[k];

    // Assign each resident to nearest station
    vector<long long> max2(N, 0);
    for(int k=0;k<K;k++){
        long long bestd = (1LL<<62);
        int besti = 0;
        for(int i=0;i<N;i++){
            long long dx = (long long)a[k] - x[i];
            long long dy = (long long)b[k] - y[i];
            long long d2 = dx*dx + dy*dy;
            if(d2 < bestd){
                bestd = d2;
                besti = i;
            }
        }
        if(bestd > max2[besti]) max2[besti] = bestd;
    }

    // Set of broadcasting nodes (with at least one assigned) plus root (0)
    vector<int> S;
    vector<char> inS(N, false);
    inS[0] = true;
    S.push_back(0);
    for(int i=0;i<N;i++){
        if(max2[i] > 0 && !inS[i]){
            inS[i] = true;
            S.push_back(i);
        }
    }

    int Ssz = (int)S.size();
    // Dijkstra from each source in S
    const long long INF = (1LL<<62);
    vector<vector<long long>> distS(Ssz, vector<long long>(N, INF));
    vector<vector<int>> prevV(Ssz, vector<int>(N, -1));
    vector<vector<int>> prevE(Ssz, vector<int>(N, -1));

    for(int si=0; si<Ssz; si++){
        int s = S[si];
        auto &dist = distS[si];
        auto &pv = prevV[si];
        auto &pe = prevE[si];
        dist[s] = 0;
        using P = pair<long long,int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, s});
        while(!pq.empty()){
            auto [d,u] = pq.top(); pq.pop();
            if(d != dist[u]) continue;
            for(auto [v, eid] : adj[u]){
                long long nd = d + W[eid];
                if(nd < dist[v]){
                    dist[v] = nd;
                    pv[v] = u;
                    pe[v] = eid;
                    pq.push({nd, v});
                }
            }
        }
    }

    // Build MST on metric closure of S using distances distS
    struct EdgeM {
        int a, b;
        long long w;
        bool operator<(EdgeM const& other) const { return w < other.w; }
    };
    vector<EdgeM> metr;
    metr.reserve(Ssz*(Ssz-1)/2);
    for(int i=0;i<Ssz;i++){
        for(int j=i+1;j<Ssz;j++){
            long long w = distS[i][ S[j] ];
            metr.push_back({i,j,w});
        }
    }
    sort(metr.begin(), metr.end());
    DSU dsu(Ssz);
    vector<char> B(M, 0);
    int edges_used = 0;
    for(auto &e : metr){
        if(dsu.unite(e.a, e.b)){
            // reconstruct path from S[e.a] to S[e.b] using prev arrays from source e.a
            int src = S[e.a];
            int tgt = S[e.b];
            int cur = tgt;
            // In connected graphs this should always succeed
            while(cur != src){
                int eid = prevE[e.a][cur];
                if(eid < 0) break; // safety
                B[eid] = 1;
                cur = prevV[e.a][cur];
                if(cur < 0) break; // safety
            }
            edges_used++;
            if(edges_used == Ssz-1) break;
        }
    }

    // Compute P_i
    auto ceil_sqrt = [](long long v)->int{
        if(v <= 0) return 0;
        long long r = sqrtl((long double)v);
        while(r*r < v) r++;
        while((r-1)*(r-1) >= v) r--;
        return (int)r;
    };
    vector<int> P(N, 0);
    for(int i=0;i<N;i++){
        P[i] = ceil_sqrt(max2[i]);
        if(P[i] > 5000) P[i] = 5000; // safety, though guaranteed unnecessary
    }

    // Output
    for(int i=0;i<N;i++){
        if(i) cout << ' ';
        cout << P[i];
    }
    cout << '\n';
    for(int j=0;j<M;j++){
        if(j) cout << ' ';
        cout << (int)B[j];
    }
    cout << '\n';
    return 0;
}