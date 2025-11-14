#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

struct DSU {
    std::vector<int> parent;
    std::vector<int> sz;
    DSU(int n) {
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
        sz.assign(n, 1);
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    int unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) std::swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            return root_i;
        }
        return root_i;
    }
};

int N, M, H;
std::vector<long long> A;
std::vector<std::pair<int, int>> edges;
std::vector<int> p;
std::vector<int> h;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> M >> H;
    A.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> A[i];
    }
    edges.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }
    for (int i = 0; i < N; ++i) {
        int x, y;
        std::cin >> x >> y;
    }

    p.assign(N, -1);
    h.assign(N, 0);

    DSU dsu(N);
    std::vector<long long> sum_A = A;
    std::vector<int> max_h(N, 0);
    std::vector<std::vector<int>> nodes(N);
    for(int i = 0; i < N; ++i) nodes[i] = {i};

    for (int iter = 0; iter < N - 1; ++iter) {
        long long best_gain = -1;
        int best_u = -1, best_v = -1;

        for (const auto& edge : edges) {
            int u_node = edge.first;
            int v_node = edge.second;
            int root_u = dsu.find(u_node);
            int root_v = dsu.find(v_node);

            if (root_u == root_v) continue;

            // Try p[u_node] = v_node
            if (max_h[root_u] + h[v_node] + 1 <= H) {
                long long current_gain = (long long)(h[v_node] + 1) * sum_A[root_u];
                if (current_gain > best_gain) {
                    best_gain = current_gain;
                    best_u = u_node;
                    best_v = v_node;
                }
            }

            // Try p[v_node] = u_node
            if (max_h[root_v] + h[u_node] + 1 <= H) {
                long long current_gain = (long long)(h[u_node] + 1) * sum_A[root_v];
                if (current_gain > best_gain) {
                    best_gain = current_gain;
                    best_u = v_node;
                    best_v = u_node;
                }
            }
        }
        
        if (best_u == -1) {
            break;
        }

        int u = best_u;
        int v = best_v;
        int root_u = dsu.find(u);
        int root_v = dsu.find(v);
        
        // Merge tree of u into tree of v
        int height_increase = h[v] + 1;
        
        p[u] = v;

        for (int node : nodes[root_u]) {
            h[node] += height_increase;
        }

        long long new_sum_A = sum_A[root_u] + sum_A[root_v];
        int new_max_h = std::max(max_h[root_v], max_h[root_u] + height_increase);
        
        int old_root_u = root_u;
        int old_root_v = root_v;
        int new_root = dsu.unite(u, v);

        if (new_root == old_root_u) {
            nodes[old_root_u].insert(nodes[old_root_u].end(), nodes[old_root_v].begin(), nodes[old_root_v].end());
            nodes[old_root_v].clear();
        } else { // new_root == old_root_v
            nodes[old_root_v].insert(nodes[old_root_v].end(), nodes[old_root_u].begin(), nodes[old_root_u].end());
            nodes[old_root_u].clear();
        }

        sum_A[new_root] = new_sum_A;
        max_h[new_root] = new_max_h;
    }

    for (int i = 0; i < N; ++i) {
        std::cout << p[i] << (i == N - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}