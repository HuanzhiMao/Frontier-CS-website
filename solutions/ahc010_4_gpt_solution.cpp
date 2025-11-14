#include <bits/stdc++.h>
using namespace std;

static const int N = 30;
static const int di[4] = {0, -1, 0, 1};
static const int dj[4] = {-1, 0, 1, 0};
static const int TO[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1}
};
// rotation map for 90 degrees CCW: 0->1->2->3->0, 4<->5, 6<->7
static const int ROTMAP[8] = {1,2,3,0,5,4,7,6};

inline int rotateTile(int t, int r){
    int x = t;
    r &= 3;
    while(r--){
        x = ROTMAP[x];
    }
    return x;
}

struct RNG {
    uint64_t x;
    RNG(uint64_t seed=88172645463393265ull){ x = seed; }
    uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    int randint(int l, int r){ return l + (int)(next() % (uint64_t)(r-l+1)); }
    double rand01(){ return (next() >> 11) * (1.0/9007199254740992.0); }
} rng;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<string> sgrid(N);
    for(int i=0;i<N;i++){
        if(!(cin>>sgrid[i])) return 0;
    }
    // base tile types
    int base[N][N];
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            base[i][j] = sgrid[i][j]-'0';
        }
    }

    // rotation counts
    int rot[N][N];
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) rot[i][j]=0;

    // oriented tile after rotation
    int tile[N][N];
    auto applyAll = [&](){
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) tile[i][j] = rotateTile(base[i][j], rot[i][j]);
    };
    applyAll();

    auto inb = [&](int i,int j){ return (0<=i && i<N && 0<=j && j<N); };

    auto edgeScoreAround = [&](int i, int j, int t)->int{
        // compute matched edges contributions around (i,j) for oriented tile type t
        int sc=0;
        // left neighbor
        if(j-1>=0){
            int tn = tile[i][j-1];
            sc += (TO[t][0]!=-1 && TO[tn][2]!=-1);
        }
        // up neighbor
        if(i-1>=0){
            int tn = tile[i-1][j];
            sc += (TO[t][1]!=-1 && TO[tn][3]!=-1);
        }
        // right neighbor
        if(j+1<N){
            int tn = tile[i][j+1];
            sc += (TO[t][2]!=-1 && TO[tn][0]!=-1);
        }
        // down neighbor
        if(i+1<N){
            int tn = tile[i+1][j];
            sc += (TO[t][3]!=-1 && TO[tn][1]!=-1);
        }
        return sc;
    };

    // Initialize rotations to reduce boundary leaks
    // Greedy initial orientation: maximize matched edges around each tile, considering neighbors as current
    // Do several sweeps
    auto start_time = chrono::high_resolution_clock::now();
    double time_limit_ms = 1800.0;
    // random initial rotations
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            // bias: if on border, try to avoid edges that go outside by preferring orientations with fewer active border sides
            int best_r=0, best_sc=-1e9;
            for(int r=0;r<4;r++){
                int t = rotateTile(base[i][j], r);
                int sc = edgeScoreAround(i,j,t);
                // Also penalize active sides going outside
                int penalty = 0;
                if(i==0 && TO[t][1]!=-1) penalty++;
                if(i==N-1 && TO[t][3]!=-1) penalty++;
                if(j==0 && TO[t][0]!=-1) penalty++;
                if(j==N-1 && TO[t][2]!=-1) penalty++;
                sc -= penalty;
                if(sc>best_sc){
                    best_sc=sc;
                    best_r=r;
                }
            }
            rot[i][j]=best_r;
        }
    }
    applyAll();

    // Coordinate descent to maximize matched edges
    int iter=0;
    while(true){
        iter++;
        bool improved=false;
        vector<pair<int,int>> order;
        order.reserve(N*N);
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) order.emplace_back(i,j);
        // shuffle order
        for(int k=0;k<(int)order.size();k++){
            int p = rng.randint(k, (int)order.size()-1);
            swap(order[k], order[p]);
        }
        for(auto &p : order){
            int i=p.first, j=p.second;
            int cur_t = tile[i][j];
            int cur_sc = edgeScoreAround(i,j,cur_t);
            int best_r = rot[i][j];
            int best_sc = cur_sc;
            for(int add=1;add<=3;add++){
                int rnew = (rot[i][j]+add)&3;
                int tnew = rotateTile(base[i][j], rnew);
                int sc = edgeScoreAround(i,j,tnew);
                if(sc > best_sc){
                    best_sc = sc;
                    best_r = rnew;
                }
            }
            if(best_r != rot[i][j]){
                rot[i][j] = best_r;
                tile[i][j] = rotateTile(base[i][j], best_r);
                improved = true;
            }
        }
        auto now = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, std::milli>(now-start_time).count();
        if(!improved || ms > time_limit_ms*0.7) break;
    }

    // Compute top two cycle lengths function
    auto computeTopTwo = [&](){
        static bool visited[N][N][4];
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) for(int d=0;d<4;d++) visited[i][j][d]=false;
        vector<int> cycles;
        cycles.reserve(100);
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                for(int d=0;d<4;d++){
                    if(TO[tile[i][j]][d]==-1) continue;
                    if(visited[i][j][d]) continue;
                    // simulate from (i,j,d)
                    int si=i, sj=j, sd=d;
                    int x=i,y=j,dir=d;
                    int length=0;
                    bool broken=false;
                    vector<tuple<int,int,int>> path;
                    path.reserve(1024);
                    unordered_map<long long,int> idx;
                    idx.reserve(1024);
                    auto key = [&](int a,int b,int c)->long long{
                        return ((long long)a<<20) | ((long long)b<<10) | c;
                    };
                    while(true){
                        long long k = key(x,y,dir);
                        if(idx.find(k) != idx.end()){
                            int pos = idx[k];
                            int cycLen = (int)path.size() - pos;
                            cycles.push_back(cycLen);
                            break;
                        }
                        idx[k] = (int)path.size();
                        path.emplace_back(x,y,dir);
                        int t = tile[x][y];
                        int d2 = TO[t][dir];
                        if(d2 == -1){ broken=true; break; }
                        int nx = x + di[d2];
                        int ny = y + dj[d2];
                        if(!inb(nx,ny)){ broken=true; break; }
                        int ndir = (d2 + 2) & 3;
                        x=nx; y=ny; dir=ndir;
                        length++;
                        if(length>200000){ broken=true; break; } // safety
                    }
                    // mark visited
                    for(auto &tp : path){
                        int a,b,c;
                        tie(a,b,c)=tp;
                        visited[a][b][c]=true;
                    }
                }
            }
        }
        int L1=0,L2=0;
        for(int v:cycles){
            if(v>=L1){ L2=L1; L1=v; }
            else if(v>L2){ L2=v; }
        }
        return pair<int,int>(L1,L2);
    };

    // Small SA local search using real score
    auto scorePair = [&](int L1,int L2)->long long{
        return 1LL*L1*L2;
    };
    auto [L1_init, L2_init] = computeTopTwo();
    long long bestScore = scorePair(L1_init, L2_init);
    int best_rot[N][N];
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) best_rot[i][j]=rot[i][j];

    double T0=1.0, T1=0.01;
    int tries = 0;
    while(true){
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double, std::milli>(now-start_time).count();
        if(elapsed > time_limit_ms) break;
        double tfrac = min(1.0, max(0.0, (time_limit_ms - elapsed)/time_limit_ms));
        double T = T1 + (T0-T1)*tfrac;

        int i = rng.randint(0,N-1);
        int j = rng.randint(0,N-1);
        int oldr = rot[i][j];
        int newr = rng.randint(0,3);
        if(newr==oldr) continue;
        rot[i][j] = newr;
        tile[i][j] = rotateTile(base[i][j], newr);
        auto [L1, L2] = computeTopTwo();
        long long newScore = scorePair(L1,L2);
        long long delta = newScore - bestScore;
        bool accept = false;
        if(delta >= 0) accept = true;
        else {
            double prob = exp(delta / max(1e-9, T));
            if(rng.rand01() < prob) accept = true;
        }
        if(accept){
            if(newScore > bestScore){
                bestScore = newScore;
                for(int a=0;a<N;a++) for(int b=0;b<N;b++) best_rot[a][b]=rot[a][b];
            }
        } else {
            rot[i][j] = oldr;
            tile[i][j] = rotateTile(base[i][j], oldr);
        }
        tries++;
    }
    // restore best found
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) rot[i][j]=best_rot[i][j];

    // Output as one line string of 900 characters
    string out;
    out.reserve(N*N);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            out.push_back(char('0' + (rot[i][j] & 3)));
        }
    }
    cout << out << "\n";
    return 0;
}