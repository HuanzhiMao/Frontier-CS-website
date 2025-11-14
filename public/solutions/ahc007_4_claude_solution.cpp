#include <bits/stdc++.h>
using namespace std;

class UnionFind {
public:
    vector<int> parent, rank;
    int components;
    
    UnionFind(int n) : parent(n), rank(n, 0), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        components--;
        return true;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N = 400, M = 1995;
    vector<int> x(N), y(N);
    vector<int> u(M), v(M);
    vector<int> d(M);
    
    for (int i = 0; i < N; i++) {
        cin >> x[i] >> y[i];
    }
    
    for (int i = 0; i < M; i++) {
        cin >> u[i] >> v[i];
        int dx = x[u[i]] - x[v[i]];
        int dy = y[u[i]] - y[v[i]];
        d[i] = round(sqrt(dx * dx + dy * dy));
    }
    
    UnionFind uf(N);
    
    for (int i = 0; i < M; i++) {
        int l;
        cin >> l;
        
        bool shouldAdopt = false;
        
        // Always adopt if it connects different components
        if (!uf.connected(u[i], v[i])) {
            shouldAdopt = true;
        } else {
            // For edges that create cycles, use a probabilistic approach
            // Adopt if the edge is significantly better than expected
            double expectedCost = d[i] * 2.0; // Expected value between d[i] and 3*d[i]
            double threshold = d[i] * 1.3; // More conservative threshold
            
            if (l < threshold) {
                shouldAdopt = true;
            }
        }
        
        if (shouldAdopt) {
            uf.unite(u[i], v[i]);
            cout << "1\n";
        } else {
            cout << "0\n";
        }
        cout.flush();
    }
    
    return 0;
}