#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n;
    vector<int> p, sz;
    DSU(int n=0): n(n), p(n), sz(n,1) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return false;
        if(sz[a]<sz[b]) swap(a,b);
        p[b]=a; sz[a]+=sz[b];
        return true;
    }
};

struct RNG {
    uint64_t x;
    RNG(uint64_t seed=88172645463393265ull){ x = seed; }
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int nextInt(int n){ return (int)(next() % n); }
    inline double nextDouble(){ return (next() >> 11) * (1.0/9007199254740992.0); }
};

struct Board {
    int N;
    vector<int> a; // size N*N, 0..15
    int br, bc;
    Board(int N=0): N(N), a(N*N,0), br(0), bc(0) {}
    inline int idx(int r,int c) const { return r*N + c; }

    pair<int,int> score() const {
        int NN = N*N;
        DSU dsu(NN);
        // unite along matched edges
        // vertical
        for(int i=0;i<N-1;i++){
            for(int j=0;j<N;j++){
                int u = idx(i,j), v = idx(i+1,j);
                int au = a[u], av = a[v];
                if(au!=0 && av!=0){
                    if( (au & 8) && (av & 2) ){
                        dsu.unite(u,v);
                    }
                }
            }
        }
        // horizontal
        for(int i=0;i<N;i++){
            for(int j=0;j<N-1;j++){
                int u = idx(i,j), v = idx(i,j+1);
                int au = a[u], av = a[v];
                if(au!=0 && av!=0){
                    if( (au & 4) && (av & 1) ){
                        dsu.unite(u,v);
                    }
                }
            }
        }
        vector<int> V(NN,0), E(NN,0);
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int u = idx(i,j);
                if(a[u]!=0){
                    V[dsu.find(u)]++;
                }
            }
        }
        // count edges per component
        // vertical
        for(int i=0;i<N-1;i++){
            for(int j=0;j<N;j++){
                int u = idx(i,j), v = idx(i+1,j);
                int au = a[u], av = a[v];
                if(au!=0 && av!=0){
                    if( (au & 8) && (av & 2) ){
                        int r = dsu.find(u);
                        E[r]++;
                    }
                }
            }
        }
        // horizontal
        for(int i=0;i<N;i++){
            for(int j=0;j<N-1;j++){
                int u = idx(i,j), v = idx(i,j+1);
                int au = a[u], av = a[v];
                if(au!=0 && av!=0){
                    if( (au & 4) && (av & 1) ){
                        int r = dsu.find(u);
                        E[r]++;
                    }
                }
            }
        }
        int bestTree = 0;
        int totalEdges = 0;
        for(int u=0;u<NN;u++){
            if(V[u]>0){
                totalEdges += E[u];
                if(E[u] == V[u]-1){
                    if(V[u] > bestTree) bestTree = V[u];
                }
            }
        }
        return {bestTree, totalEdges};
    }

    inline void apply(char mv){
        if(mv=='U'){
            // tile at (br-1,bc) goes down to (br,bc)
            int r2 = br-1, c2 = bc;
            swap(a[idx(br,bc)], a[idx(r2,c2)]);
            br = r2; bc = c2;
        } else if(mv=='D'){
            int r2 = br+1, c2 = bc;
            swap(a[idx(br,bc)], a[idx(r2,c2)]);
            br = r2; bc = c2;
        } else if(mv=='L'){
            int r2 = br, c2 = bc-1;
            swap(a[idx(br,bc)], a[idx(r2,c2)]);
            br = r2; bc = c2;
        } else if(mv=='R'){
            int r2 = br, c2 = bc+1;
            swap(a[idx(br,bc)], a[idx(r2,c2)]);
            br = r2; bc = c2;
        }
    }
};

inline int hexd(char ch){
    if('0'<=ch && ch<='9') return ch-'0';
    return 10 + (ch-'a');
}
inline char opposite(char c){
    if(c=='U') return 'D';
    if(c=='D') return 'U';
    if(c=='L') return 'R';
    return 'L';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N; long long T;
    if(!(cin>>N>>T)) return 0;
    vector<string> s(N);
    Board B(N);
    for(int i=0;i<N;i++){
        cin>>s[i];
        for(int j=0;j<N;j++){
            int v = hexd(s[i][j]);
            B.a[i*N+j] = v;
            if(v==0){ B.br=i; B.bc=j; }
        }
    }
    RNG rng(chrono::high_resolution_clock::now().time_since_epoch().count() ^ 0x9e3779b97f4a7c15ull);
    auto sc0 = B.score();
    int curS = sc0.first, curM = sc0.second;
    string ops; ops.reserve(T);
    int bestS = curS, bestM = curM;
    size_t bestK = 0;
    char prev = 0;

    for(long long step=0; step<T; step++){
        vector<char> cand;
        if(B.br>0) cand.push_back('U');
        if(B.br<N-1) cand.push_back('D');
        if(B.bc>0) cand.push_back('L');
        if(B.bc<N-1) cand.push_back('R');
        if(prev && cand.size()>1){
            char rev = opposite(prev);
            // remove reverse if present
            for(size_t i=0;i<cand.size();i++){
                if(cand[i]==rev){
                    cand.erase(cand.begin()+i);
                    break;
                }
            }
        }
        // evaluate candidates
        int bestIdx = 0;
        long long bestW = LLONG_MIN;
        vector<pair<int,int>> candSc; candSc.reserve(cand.size());
        for(size_t i=0;i<cand.size();i++){
            char mv = cand[i];
            B.apply(mv);
            auto sc = B.score();
            B.apply(opposite(mv));
            candSc.push_back(sc);
            long long W = (long long)sc.first * 1000 + sc.second;
            // Add tiny random noise to break ties
            W = W*100 + (rng.nextInt(100));
            if(W > bestW){
                bestW = W;
                bestIdx = (int)i;
            }
        }
        int chooseIdx;
        // epsilon-greedy random move
        if((int)cand.size()>0 && rng.nextInt(100) < 15){
            chooseIdx = rng.nextInt((int)cand.size());
        }else{
            chooseIdx = bestIdx;
        }
        char mv = cand[chooseIdx];
        B.apply(mv);
        ops.push_back(mv);
        prev = mv;
        curS = candSc[chooseIdx].first;
        curM = candSc[chooseIdx].second;
        if(curS > bestS || (curS==bestS && curM > bestM)){
            bestS = curS; bestM = curM; bestK = ops.size();
        }
    }

    if(bestK > ops.size()) bestK = ops.size();
    cout << string(ops.begin(), ops.begin()+bestK) << '\n';
    return 0;
}