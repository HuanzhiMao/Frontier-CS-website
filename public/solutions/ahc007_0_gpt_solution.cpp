#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n, comps;
    vector<int> p, sz;
    DSU() {}
    DSU(int n): n(n), comps(n), p(n), sz(n,1) { iota(p.begin(), p.end(), 0); }
    int find(int a){ return p[a]==a?a:p[a]=find(p[a]); }
    bool unite(int a, int b){
        a = find(a); b = find(b);
        if(a==b) return false;
        if(sz[a] < sz[b]) swap(a,b);
        p[b]=a; sz[a]+=sz[b]; comps--;
        return true;
    }
    int components() const { return comps; }
};

static inline int iround(double x){
    return (int)llround(x);
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

    vector<int> U(M), V(M);
    for(int i=0;i<M;i++){
        cin>>U[i]>>V[i];
    }

    // Precompute d_i
    vector<int> D(M);
    for(int i=0;i<M;i++){
        int dx = x[U[i]] - x[V[i]];
        int dy = y[U[i]] - y[V[i]];
        double dist = sqrt((double)dx*dx + (double)dy*dy);
        D[i] = iround(dist);
    }

    // Precompute MST on D (over given edges)
    vector<int> idx(M);
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(), [&](int a, int b){
        if(D[a] != D[b]) return D[a] < D[b];
        return a < b;
    });
    vector<char> inDiMST(M, 0);
    {
        DSU dsu(N);
        for(int id: idx){
            if(dsu.unite(U[id], V[id])) inDiMST[id] = 1;
        }
    }

    // Precompute compAt[k][v]: connected components using edges with index >= k
    // compAt is stored as 1D array of size (M+1)*N
    vector<int> compAt((M+1)*N);
    {
        DSU dsu(N);
        // Initially with no edges (k=M), each vertex alone
        for(int v=0; v<N; v++) compAt[M*N + v] = dsu.find(v);
        for(int k=M-1; k>=0; k--){
            dsu.unite(U[k], V[k]);
            for(int v=0; v<N; v++){
                compAt[k*N + v] = dsu.find(v);
            }
        }
    }

    // Accepted edges and DSU for final connectivity tracking
    vector<pair<int,int>> acceptedEdges; acceptedEdges.reserve(N-1);
    DSU dsuAccepted(N);

    // Helper arrays for necessary-accept check
    vector<char> presentRoot(N, 0);
    vector<int> presentList; presentList.reserve(N);
    vector<char> usedComp(N, 0);
    vector<int> usedCompList; usedCompList.reserve(N);

    long long totalCost = 0;

    for(int i=0;i<M;i++){
        int li;
        if(!(cin>>li)) li = D[i]; // Fallback, should not happen
        int u = U[i], v = V[i];

        // Necessary acceptance check
        bool mustAccept = false;
        {
            // Build DSU over suffix components (edges with index >= i+1)
            DSU dsuC(N);
            int base = (i+1)*N;

            // Mark present roots and collect unique ones
            presentList.clear();
            for(int vv=0; vv<N; vv++){
                int r = compAt[base + vv];
                if(!presentRoot[r]){
                    presentRoot[r] = 1;
                    presentList.push_back(r);
                }
            }

            // Unite accepted edges mapped to suffix components
            for(const auto &e: acceptedEdges){
                int a = e.first, b = e.second;
                int ra = compAt[base + a];
                int rb = compAt[base + b];
                if(ra != rb) dsuC.unite(ra, rb);
            }

            // Count number of connected components among present roots
            usedCompList.clear();
            int cc = 0;
            for(int r: presentList){
                int fr = dsuC.find(r);
                if(!usedComp[fr]){
                    usedComp[fr] = 1;
                    usedCompList.push_back(fr);
                    cc++;
                }
            }
            // Clear flags
            for(int r: presentList) presentRoot[r] = 0;
            for(int fr: usedCompList) usedComp[fr] = 0;

            if(cc > 1){
                // To keep possibility of connectivity, must accept this edge
                mustAccept = true;
            }
        }

        int decision = 0;

        if(mustAccept){
            decision = 1;
        } else {
            // Optional acceptance heuristic
            int fu = dsuAccepted.find(u);
            int fv = dsuAccepted.find(v);
            if(fu != fv){
                // Compute minimal D among future edges connecting current components fu and fv
                int dMinFuture = INT_MAX;
                for(int j=i; j<M; j++){
                    int a = U[j], b = V[j];
                    int fa = dsuAccepted.find(a);
                    int fb = dsuAccepted.find(b);
                    if((fa==fu && fb==fv) || (fa==fv && fb==fu)){
                        if(D[j] < dMinFuture) dMinFuture = D[j];
                    }
                }
                if(dMinFuture == INT_MAX) dMinFuture = D[i];

                double p = (double)i / (double)M;
                // Base alpha increases over time
                double baseAlpha = 1.08 + 1.36 * p; // [1.08, 2.44]
                // Adjust alpha based on how many components are left vs target
                double targetComps = 1.0 + (double)(N-1) * (1.0 - (double)(i+1) / (double)M);
                double compsNow = (double)dsuAccepted.components();
                double diff = (compsNow - targetComps) / (double)(N-1); // roughly in [-1,1]
                double cf = 1.0 + 0.9 * diff;
                if(cf < 0.7) cf = 0.7;
                if(cf > 1.3) cf = 1.3;
                double alpha = baseAlpha * cf;
                // Prefer edges in di-MST
                if(inDiMST[i]){
                    double favor = 1.0 - 0.15 * (1.0 - p); // from 0.85 early to ~1.0 late
                    alpha *= favor;
                }
                if(alpha < 1.02) alpha = 1.02;
                if(alpha > 2.9) alpha = 2.9;

                if((double)li <= (double)dMinFuture * alpha){
                    decision = 1;
                }
            } else {
                decision = 0; // avoid cycles unless necessary
            }
        }

        if(decision == 1){
            acceptedEdges.emplace_back(u, v);
            dsuAccepted.unite(u, v);
            totalCost += li;
        }

        cout << decision << '\n' << flush;
    }

    return 0;
}