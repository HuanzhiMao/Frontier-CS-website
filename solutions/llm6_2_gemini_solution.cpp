#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

std::vector<std::vector<int>> create_map(int N, int M,
    std::vector<int> A, std::vector<int> B) {

    if (N == 0) {
        return {};
    }
    if (N == 1) {
        return {{1}};
    }

    std::vector<std::vector<bool>> adj_mat(N + 1, std::vector<bool>(N + 1, false));
    for (int i = 0; i < M; ++i) {
        adj_mat[A[i]][B[i]] = adj_mat[B[i]][A[i]] = true;
    }

    std::vector<int> nodes(N);
    std::iota(nodes.begin(), nodes.end(), 1);
    
    // A heuristic for a better greedy coloring order
    std::sort(nodes.begin(), nodes.end(), [&](int u, int v) {
        int deg_u = 0, deg_v = 0;
        for (int i = 1; i <= N; ++i) {
            if (!adj_mat[u][i]) deg_u++;
            if (!adj_mat[v][i]) deg_v++;
        }
        return deg_u > deg_v;
    });

    std::vector<int> color(N + 1, 0);
    int p = 0;
    for (int u : nodes) {
        std::set<int> used_colors;
        for (int v = 1; v <= N; ++v) {
            if (u == v) continue;
            if (!adj_mat[u][v]) { // edge in complement graph
                if (color[v] != 0) {
                    used_colors.insert(color[v]);
                }
            }
        }
        int c = 1;
        while (used_colors.count(c)) {
            c++;
        }
        color[u] = c;
        p = std::max(p, c);
    }

    std::vector<std::vector<int>> cliques(p);
    for (int i = 1; i <= N; ++i) {
        cliques[color[i] - 1].push_back(i);
    }

    int height = 0;
    for (const auto& cl : cliques) {
        height = std::max(height, (int)cl.size());
    }
    int width = p;
    
    std::vector<std::vector<int>> final_map(height, std::vector<int>(width));
    for (int j = 0; j < width; ++j) {
        const auto& cl = cliques[j];
        int s = cl.size();
        for (int i = 0; i < height; ++i) {
            final_map[i][j] = cl[i % s];
        }
    }
    
    // For the general case where not all pairs of cliques are adjacent,
    // this simple construction is not enough. A square map of cliques
    // provides more adjacencies.
    int d = 1;
    while(d*d < p) d++;

    std::vector<std::vector<int>> grid_map(d, std::vector<int>(d));
    std::vector<std::vector<int>> grid_map_final;

    if (double(height*width)/N > double(d*d)/N + 2.0 && N > 10) { // Heuristic to switch strategy
        int side = d;
        grid_map_final.assign(side, std::vector<int>(side));
        int c_idx = 0;
        for(int i=0; i<side; ++i) {
            for(int j=0; j<side; ++j) {
                if (c_idx < p) {
                    grid_map_final[i][j] = cliques[c_idx][(i+j) % cliques[c_idx].size()];
                } else {
                    grid_map_final[i][j] = cliques[0][0];
                }
                c_idx++;
            }
        }
        return grid_map_final;
    }

    return final_map;
}