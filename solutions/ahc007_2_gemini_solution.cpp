#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iterator>

const int N_VAL = 400;
const int M_VAL = 1995;

struct Point {
    int x, y;
};

struct EdgeInfo {
    int u, v, id;
    long long d;
};

struct AdjEdge {
    int v, id;
    long long d;
};

std::vector<Point> vertices(N_VAL);
std::vector<EdgeInfo> edges(M_VAL);
std::vector<std::vector<AdjEdge>> adj(N_VAL);

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

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j])
                std::swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < N_VAL; ++i) {
        std::cin >> vertices[i].x >> vertices[i].y;
    }

    for (int i = 0; i < M_VAL; ++i) {
        edges[i].id = i;
        std::cin >> edges[i].u >> edges[i].v;
        long long dx = vertices[edges[i].u].x - vertices[edges[i].v].x;
        long long dy = vertices[edges[i].u].y - vertices[edges[i].v].y;
        edges[i].d = round(sqrt(dx * dx + dy * dy));
        adj[edges[i].u].push_back({edges[i].v, i, edges[i].d});
        adj[edges[i].v].push_back({edges[i].u, i, edges[i].d});
    }

    DSU dsu(N_VAL);
    std::vector<std::vector<int>> component_members(N_VAL);
    for(int i = 0; i < N_VAL; ++i) {
        component_members[i].push_back(i);
    }

    for (int i = 0; i < M_VAL; ++i) {
        long long l;
        std::cin >> l;
        
        int u = edges[i].u;
        int v = edges[i].v;

        int root_u = dsu.find(u);
        int root_v = dsu.find(v);

        if (root_u == root_v) {
            std::cout << 0 << std::endl;
            continue;
        }

        int smaller_comp_root = root_u;
        int larger_comp_root = root_v;
        if (dsu.sz[root_u] > dsu.sz[root_v]) {
            std::swap(smaller_comp_root, larger_comp_root);
        }

        long long min_alt_d = -1;

        for (int member_node : component_members[smaller_comp_root]) {
            for (const auto& edge : adj[member_node]) {
                if (dsu.find(edge.v) == larger_comp_root) {
                    if (edge.id != i) {
                        if (min_alt_d == -1 || edge.d < min_alt_d) {
                            min_alt_d = edge.d;
                        }
                    }
                }
            }
        }
        
        bool take = false;
        if (min_alt_d == -1) {
            take = true;
        } else {
            const double C = 1.8 + 0.4 * (i / (double)(M_VAL - 1));
            if (l <= C * min_alt_d) {
                take = true;
            }
        }
        
        std::cout << (take ? 1 : 0) << std::endl;
        
        if (take) {
            int old_root_u = dsu.find(u);
            int old_root_v = dsu.find(v);
            
            int root_to_merge, target_root;
            if (dsu.sz[old_root_u] < dsu.sz[old_root_v]) {
                root_to_merge = old_root_u;
                target_root = old_root_v;
            } else {
                root_to_merge = old_root_v;
                target_root = old_root_u;
            }
            
            dsu.unite(u, v);
            
            component_members[target_root].insert(
                component_members[target_root].end(),
                std::make_move_iterator(component_members[root_to_merge].begin()),
                std::make_move_iterator(component_members[root_to_merge].end())
            );
            std::vector<int>().swap(component_members[root_to_merge]);
        }
    }

    return 0;
}