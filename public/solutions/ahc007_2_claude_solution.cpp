#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
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
    
    const int N = 400, M = 1995;
    
    vector<pair<int, int>> vertices(N);
    for (int i = 0; i < N; i++) {
        cin >> vertices[i].first >> vertices[i].second;
    }
    
    vector<pair<int, int>> edges(M);
    vector<int> euclidean_dist(M);
    
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
        int u = edges[i].first, v = edges[i].second;
        int dx = vertices[u].first - vertices[v].first;
        int dy = vertices[u].second - vertices[v].second;
        euclidean_dist[i] = round(sqrt(dx * dx + dy * dy));
    }
    
    UnionFind uf(N);
    
    for (int i = 0; i < M; i++) {
        int length;
        cin >> length;
        
        int u = edges[i].first, v = edges[i].second;
        bool should_adopt = false;
        
        if (!uf.connected(u, v)) {
            // This edge would connect two components
            double expected_length = 2.0 * euclidean_dist[i];
            if (length <= expected_length || uf.components > N - M + i) {
                should_adopt = true;
                uf.unite(u, v);
            }
        }
        
        cout << (should_adopt ? 1 : 0) << endl;
        cout.flush();
    }
    
    return 0;
}