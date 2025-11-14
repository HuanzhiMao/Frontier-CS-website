#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n=0){init(n);}
    void init(int n){
        p.resize(n+1);
        r.assign(n+1,0);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
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
    
    vector<long long> tok;
    tok.reserve(200000);
    long long x;
    while (cin >> x) tok.push_back(x);
    if (tok.empty()) return 0;

    int t = (int)tok[0];
    size_t pos = 1;

    auto can_parse_at = [&](size_t j, int &n_out, vector<pair<int,int>>& edges_out)->bool{
        if (j >= tok.size()) return false;
        long long nll = tok[j];
        if (nll < 2 || nll > 1000) return false;
        int n = (int)nll;
        size_t need = j + 1 + 2ull * (n - 1);
        if (need > tok.size()) return false;
        vector<pair<int,int>> edges;
        edges.reserve(n-1);
        DSU dsu(n);
        int merges = 0;
        for (int i = 0; i < n - 1; ++i) {
            long long u = tok[j + 1 + 2*i];
            long long v = tok[j + 1 + 2*i + 1];
            if (u < 1 || u > n || v < 1 || v > n) return false;
            edges.emplace_back((int)u, (int)v);
            if (!dsu.unite((int)u, (int)v)) {
                // cycle detected
                // still could be tree if duplicates? No, tree must be acyclic
                return false;
            } else {
                merges++;
            }
        }
        // Check connected: exactly one component
        int root = dsu.find(1);
        for (int i = 2; i <= n; ++i) if (dsu.find(i) != root) return false;
        if (merges != n - 1) return false;

        n_out = n;
        edges_out = move(edges);
        return true;
    };

    for (int tc = 0; tc < t; ++tc) {
        // Find next valid test start
        int n = -1;
        vector<pair<int,int>> edges;
        bool ok = false;
        for (size_t j = pos; j < tok.size(); ++j) {
            if (can_parse_at(j, n, edges)) {
                pos = j + 1 + 2ull * (n - 1);
                ok = true;
                break;
            }
        }
        if (!ok) {
            // Cannot parse further; just stop
            break;
        }
        // Output a dummy answer: all ones
        cout << "! ";
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << 1;
        }
        cout << "\n";
        cout.flush();
    }

    return 0;
}