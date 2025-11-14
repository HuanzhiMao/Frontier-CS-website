#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

struct Point {
    int x, y;
};

struct Edge {
    int u, v, d, id;
};

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
            if (sz[root_i] < sz[root_j]) std::swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
        }
    }
};

long long dist_sq(const Point& p1, const Point& p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int N = 400;
    const int M = 1995;

    std::vector<Point> V(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> V[i].x >> V[i].y;
    }

    std::vector<Edge> E(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> E[i].u >> E[i].v;
        E[i].id = i;
        E[i].d = static_cast<int>(round(sqrt(dist_sq(V[E[i].u], V[E[i].v]))));
    }

    DSU dsu(N);

    for (int i = 0; i < M; ++i) {
        int l;
        std::cin >> l;
        
        int u = E[i].u;
        int v = E[i].v;

        int root_u = dsu.find(u);
        int root_v = dsu.find(v);

        if (root_u == root_v) {
            std::cout << 0 << std::endl;
            continue;
        }

        std::vector<int> future_ds;
        for (int j = i + 1; j < M; ++j) {
            int u_j = E[j].u;
            int v_j = E[j].v;
            int root_u_j = dsu.find(u_j);
            int root_v_j = dsu.find(v_j);
            if ((root_u_j == root_u && root_v_j == root_v) || (root_u_j == root_v && root_v_j == root_u)) {
                future_ds.push_back(E[j].d);
            }
        }

        bool adopt = false;
        if (future_ds.empty()) {
            adopt = true;
        } else {
            long double current_norm_cost = static_cast<long double>(l) / E[i].d;

            long double T_absolute = 1.05;
            if (current_norm_cost < T_absolute) {
                adopt = true;
            } else {
                int d_min_future = future_ds[0];
                for (size_t k = 1; k < future_ds.size(); ++k) {
                    if (future_ds[k] < d_min_future) {
                        d_min_future = future_ds[k];
                    }
                }
                
                long double p_eff = 0;
                long double c = 2.0;
                for (int d_j : future_ds) {
                    p_eff += pow(static_cast<long double>(d_min_future) / d_j, c);
                }

                long double T_relative = 1.0 + 2.0 / (p_eff + 1.0);
                
                if (current_norm_cost < T_relative) {
                    adopt = true;
                }
            }
        }

        if (adopt) {
            std::cout << 1 << std::endl;
            dsu.unite(u, v);
        } else {
            std::cout << 0 << std::endl;
        }
    }

    return 0;
}