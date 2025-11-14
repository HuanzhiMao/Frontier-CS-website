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
    
    const int N = 400, M = 1995;
    
    vector<pair<int, int>> coords(N);
    for (int i = 0; i < N; i++) {
        cin >> coords[i].first >> coords[i].second;
    }
    
    vector<pair<int, int>> edges(M);
    vector<int> distances(M);
    
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
        int dx = coords[edges[i].first].first - coords[edges[i].second].first;
        int dy = coords[edges[i].first].second - coords[edges[i].second].second;
        distances[i] = round(sqrt(dx * dx + dy * dy));
    }
    
    UnionFind uf(N);
    
    for (int i = 0; i < M; i++) {
        int length;
        cin >> length;
        
        int u = edges[i].first;
        int v = edges[i].second;
        
        bool should_take = false;
        
        if (!uf.connected(u, v)) {
            int expected_min = distances[i];
            int expected_max = 3 * distances[i];
            double threshold = expected_min + 0.5 * (expected_max - expected_min);
            
            if (length <= threshold || uf.components > N - (M - i)) {
                should_take = true;
            }
        }
        
        if (should_take) {
            uf.unite(u, v);
            cout << "1\n";
        } else {
            cout << "0\n";
        }
        cout.flush();
    }
    
    return 0;
}