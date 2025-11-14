#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <algorithm>

struct Point {
    long long x, y;
};

long long dist_sq(const Point& p1, const Point& p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

struct Edge {
    int u, v;
    int d;
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

const int N = 400;
const int M = 1995;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<Point> coords(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> coords[i].x >> coords[i].y;
    }

    std::vector<Edge> edges(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].u >> edges[i].v;
        edges[i].d = static_cast<int>(std::round(std::sqrt(dist_sq(coords[edges[i].u], coords[edges[i].v]))));
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
        } else {
            int future_connections = 0;
            for (int j = i + 1; j < M; ++j) {
                int u_j = edges[j].u;
                int v_j = edges[j].v;
                if ((dsu.find(u_j) == root_u && dsu.find(v_j) == root_v) ||
                    (dsu.find(u_j) == root_v && dsu.find(v_j) == root_u)) {
                    future_connections++;
                }
            }

            bool take_edge = false;
            if (future_connections == 0) {
                take_edge = true;
            } else {
                double p_i = (double)(l - edges[i].d) / (2.0 * edges[i].d);
                double p_threshold = 1.0 / (future_connections + 1.0);
                
                if (p_i < p_threshold) {
                   take_edge = true;
                }
            }

            if (take_edge) {
                std::cout << 1 << std::endl;
                dsu.unite(u, v);
            } else {
                std::cout << 0 << std::endl;
            }
        }
    }

    return 0;
}