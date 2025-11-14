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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N = 400, M = 1995;
    vector<pair<int, int>> vertices(N);
    vector<pair<int, int>> edges(M);
    vector<int> distances(M);
    
    for (int i = 0; i < N; i++) {
        cin >> vertices[i].first >> vertices[i].second;
    }
    
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
        int dx = vertices[edges[i].first].first - vertices[edges[i].second].first;
        int dy = vertices[edges[i].first].second - vertices[edges[i].second].second;
        distances[i] = round(sqrt(dx * dx + dy * dy));
    }
    
    UnionFind uf(N);
    
    for (int i = 0; i < M; i++) {
        int length;
        cin >> length;
        
        int u = edges[i].first;
        int v = edges[i].second;
        
        // Strategy: Accept edge if it connects different components
        // and the length is reasonable (within 1.5 * distance)
        bool should_accept = false;
        
        if (!uf.connected(u, v)) {
            // If it connects different components, consider accepting
            double ratio = (double)length / distances[i];
            
            // Accept if it's a good deal (length <= 2 * euclidean distance)
            // or if we need more edges to ensure connectivity
            if (ratio <= 2.0 || uf.components > N - i / 5) {
                should_accept = true;
            }
        }
        
        if (should_accept && uf.unite(u, v)) {
            cout << "1\n";
        } else {
            cout << "0\n";
        }
        cout.flush();
    }
    
    return 0;
}