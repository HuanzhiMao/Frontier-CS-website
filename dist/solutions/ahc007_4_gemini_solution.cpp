#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <tuple>

// N and M are fixed for this problem.
const int N = 400;
const int M = 1995;

struct Point {
    int x, y;
};

// Disjoint Set Union (DSU) data structure to manage connected components.
// It supports union-by-size and path compression for efficiency.
struct DSU {
    std::vector<int> parent;
    std::vector<int> sz;
    int components;

    DSU(int n) {
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
        sz.assign(n, 1);
        components = n;
    }

    DSU(const DSU& other) = default;

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
            components--;
        }
    }
};

// Global arrays to store edge information for easy access.
int u_all[M], v_all[M];
long long d_all[M];
std::vector<int> sorted_indices;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Read vertex coordinates.
    std::vector<Point> coords(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> coords[i].x >> coords[i].y;
    }

    // Read all edge endpoints and pre-calculate their Euclidean distances.
    for (int i = 0; i < M; ++i) {
        std::cin >> u_all[i] >> v_all[i];
        long long dx = coords[u_all[i]].x - coords[v_all[i]].x;
        long long dy = coords[u_all[i]].y - coords[v_all[i]].y;
        d_all[i] = round(sqrt(dx * dx + dy * dy));
    }

    // Create a list of edge indices and sort them based on Euclidean distance `d`.
    // This pre-sorting is crucial for the simulation step.
    sorted_indices.resize(M);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return d_all[a] < d_all[b];
    });

    DSU main_dsu(N);

    // Process edges one by one as they are revealed.
    for (int i = 0; i < M; ++i) {
        int l_i;
        std::cin >> l_i;

        int u_i = u_all[i];
        int v_i = v_all[i];

        // If the edge connects two vertices already in the same component,
        // accepting it would create a cycle. So, always reject.
        if (main_dsu.find(u_i) == main_dsu.find(v_i)) {
            std::cout << 0 << std::endl;
            continue;
        }

        // --- Core Decision Logic ---
        // Find d_alt: the Euclidean distance of the best alternative future edge.
        long long d_alt = -1;
        DSU temp_dsu = main_dsu;

        // Simulate Kruskal's algorithm on future edges (k > i)
        // to find the bottleneck edge that connects u_i's and v_i's components.
        for (int k_idx : sorted_indices) {
            if (k_idx <= i) continue;

            temp_dsu.unite(u_all[k_idx], v_all[k_idx]);
            if (temp_dsu.find(u_i) == temp_dsu.find(v_i)) {
                d_alt = d_all[k_idx];
                break;
            }
        }
        
        bool accept = false;
        if (d_alt == -1) {
            // No future path exists to connect these components, so we must accept.
            accept = true;
        } else {
            // Adaptive strategy: adjust decision threshold based on urgency.
            int edges_needed = main_dsu.components - 1;
            int edges_left = M - 1 - i;
            
            double urgency = 0.0;
            if (edges_left > 0) {
                urgency = static_cast<double>(edges_needed) / edges_left;
            } else if (edges_needed > 0) {
                // If edges are needed but none are left, urgency is maximal.
                urgency = 1e9;
            }

            // The comparison factor `c` increases with urgency.
            // Baseline is 2.0, as 2*d is the expected length.
            double c = 2.0 + 1.5 * urgency;
            
            if (l_i < c * d_alt) {
                accept = true;
            }
        }

        if (accept) {
            std::cout << 1 << std::endl;
            main_dsu.unite(u_i, v_i);
        } else {
            std::cout << 0 << std::endl;
        }
    }

    return 0;
}