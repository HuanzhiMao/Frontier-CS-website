#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <map>

// DSU with path compression and union by size
struct DSU {
    std::vector<int> parent;
    std::vector<int> sz;
    DSU(int n) {
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
        sz.assign(n, 1);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) std::swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
        }
    }
};

const int N = 400;
const int M = 1995;

struct Point { int x, y; };
struct Edge { int u, v; };

Point coords[N];
Edge edges[M];
int d[M];

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < N; ++i) {
        std::cin >> coords[i].x >> coords[i].y;
    }
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].u >> edges[i].v;
        double dx = coords[edges[i].u].x - coords[edges[i].v].x;
        double dy = coords[edges[i].u].y - coords[edges[i].v].y;
        d[i] = static_cast<int>(round(sqrt(dx*dx + dy*dy)));
    }

    DSU dsu(N);

    for (int i = 0; i < M; ++i) {
        int l;
        std::cin >> l;

        int u = edges[i].u;
        int v = edges[i].v;
        int root_u = dsu.find(u);
        int root_v = dsu.find(v);

        if (root_u == root_v) {
            std::cout << 0 << std::endl;
            continue;
        }

        bool is_bridge = true;
        
        std::map<int, int> root_to_idx;
        int V_comp = 0;
        for (int j = 0; j < N; ++j) {
            int root = dsu.find(j);
            if (root_to_idx.find(root) == root_to_idx.end()) {
                root_to_idx[root] = V_comp++;
            }
        }
        
        if (V_comp > 1) {
            std::vector<std::vector<int>> adj_comp(V_comp);
            for (int j = i + 1; j < M; ++j) {
                int r_u_j = dsu.find(edges[j].u);
                int r_v_j = dsu.find(edges[j].v);
                if (r_u_j != r_v_j) {
                    int idx1 = root_to_idx[r_u_j];
                    int idx2 = root_to_idx[r_v_j];
                    adj_comp[idx1].push_back(idx2);
                    adj_comp[idx2].push_back(idx1);
                }
            }
            
            int start_node = root_to_idx[root_u];
            int end_node = root_to_idx[root_v];
            
            std::vector<bool> visited(V_comp, false);
            std::vector<int> q;
            q.push_back(start_node);
            visited[start_node] = true;
            int head = 0;
            while(head < q.size()){
                int curr = q[head++];
                if (curr == end_node) {
                    is_bridge = false;
                    break;
                }
                for (int neighbor : adj_comp[curr]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push_back(neighbor);
                    }
                }
            }
        }

        if (is_bridge) {
            std::cout << 1 << std::endl;
            dsu.unite(u, v);
            continue;
        }

        int min_future_d = 1e9 + 7;
        int k_prime = 0;
        for (int j = i + 1; j < M; ++j) {
            if ((dsu.find(edges[j].u) == root_u && dsu.find(edges[j].v) == root_v) ||
                (dsu.find(edges[j].u) == root_v && dsu.find(edges[j].v) == root_u)) {
                if (d[j] < min_future_d) {
                    min_future_d = d[j];
                    k_prime = 1;
                } else if (d[j] == min_future_d) {
                    k_prime++;
                }
            }
        }

        double threshold = (double)min_future_d * (1.0 + 2.0 / (k_prime + 1.0));

        if (l < threshold) {
            std::cout << 1 << std::endl;
            dsu.unite(u, v);
        } else {
            std::cout << 0 << std::endl;
        }
    }

    return 0;
}