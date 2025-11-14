#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    DSU(int n=0){ init(n); }
    void init(int n){
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x){
        while(p[x]!=x){ p[x]=p[p[x]]; x=p[x]; }
        return x;
    }
    bool same(int a,int b){ return find(a)==find(b); }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 400;
    const int M = 1995;

    vector<int> x(N), y(N);
    for(int i=0;i<N;i++){
        if(!(cin>>x[i]>>y[i])) return 0;
    }
    vector<int> U(M), V(M);
    for(int i=0;i<M;i++){
        cin>>U[i]>>V[i];
    }
    vector<int> D(M);
    for(int i=0;i<M;i++){
        long long dx = (long long)x[U[i]] - x[V[i]];
        long long dy = (long long)y[U[i]] - y[V[i]];
        D[i] = (int) llround(sqrt((long double)(dx*dx + dy*dy)));
    }

    DSU dsu(N);
    vector<unordered_map<int,int>> adj(N);
    for(int i=0;i<N;i++) adj[i].reserve(16);

    // Initialize adjacency counts for unseen edges (between initial singleton components)
    for(int i=0;i<M;i++){
        int u = U[i], v = V[i];
        adj[u][v] += 1;
        adj[v][u] += 1;
    }

    auto decrement_pair = [&](int a, int b){
        if(a==b) return;
        auto it = adj[a].find(b);
        if(it != adj[a].end()){
            if(--(it->second) == 0) adj[a].erase(it);
        }
        auto it2 = adj[b].find(a);
        if(it2 != adj[b].end()){
            if(--(it2->second) == 0) adj[b].erase(it2);
        }
    };

    auto merge_components = [&](int a, int b){
        a = dsu.find(a);
        b = dsu.find(b);
        if(a==b) return a;
        // Merge smaller adjacency map into larger
        if(adj[a].size() < adj[b].size()) swap(a, b);
        dsu.p[b] = a;
        dsu.sz[a] += dsu.sz[b];

        vector<pair<int,int>> neighbors;
        neighbors.reserve(adj[b].size());
        for(auto &kv : adj[b]) neighbors.emplace_back(kv.first, kv.second);
        adj[b].clear();

        for(auto &kv : neighbors){
            int c = kv.first;
            int cnt = kv.second;
            c = dsu.find(c);

            // Remove back-link from c to b
            auto itcb = adj[c].find(b);
            if(itcb != adj[c].end()){
                itcb->second -= cnt;
                if(itcb->second <= 0) adj[c].erase(itcb);
            }

            if(c == a){
                // Edges between a and b become internal after merge
                continue;
            }
            adj[a][c] += cnt;
            adj[c][a] += cnt;
        }
        return a;
    };

    auto is_connected_after_remove = [&](int ra, int rb, int cntAB) -> bool {
        int start = -1;
        int leaderCount = 0;
        static vector<char> vis;
        vis.assign(N, 0);
        for(int i=0;i<N;i++){
            if(dsu.find(i)==i){
                leaderCount++;
                if(start==-1) start = i;
            }
        }
        if(leaderCount <= 1) return true;

        deque<int> dq;
        dq.push_back(start);
        vis[start] = 1;
        int visited = 0;
        while(!dq.empty()){
            int u = dq.front(); dq.pop_front();
            visited++;
            for(auto &kv : adj[u]){
                int v = kv.first;
                int cnt = kv.second;
                if(cnt <= 0) continue;
                if(cntAB == 1 && ((u==ra && v==rb) || (u==rb && v==ra))) continue;
                if(!vis[v]){
                    vis[v] = 1;
                    dq.push_back(v);
                }
            }
        }
        return visited == leaderCount;
    };

    for(int i=0;i<M;i++){
        long long L;
        if(!(cin>>L)) return 0;

        int u = U[i], v = V[i];
        int ru = dsu.find(u), rv = dsu.find(v);

        bool accept = false;

        if(ru == rv){
            accept = false;
        } else {
            int cntAB = 0;
            auto it = adj[ru].find(rv);
            if(it != adj[ru].end()) cntAB = it->second;

            double progress = (double)i / (double)M;
            double lamStart = 1.15;
            double lamEnd = 2.20;
            double lam = lamStart + (lamEnd - lamStart) * pow(progress, 1.2);
            if(cntAB >= 3) lam -= 0.10;
            if(cntAB >= 5) lam -= 0.05;
            if(lam < 1.0) lam = 1.0;

            if((double)L <= lam * (double)D[i]){
                accept = true;
            }

            if(!accept){
                bool safe = true;
                if(cntAB <= 1){
                    safe = is_connected_after_remove(ru, rv, cntAB);
                }
                if(!safe) accept = true;
            }
        }

        cout << (accept ? 1 : 0) << '\n' << flush;

        if(ru != rv){
            decrement_pair(ru, rv);
            if(accept){
                merge_components(ru, rv);
            }
        }
    }

    return 0;
}