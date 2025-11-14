#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// You should implement this procedure
std::vector<std::vector<int>> create_map(int N, int M,
    std::vector<int> A, std::vector<int> B) {
    
    std::vector<std::vector<bool>> adj(N, std::vector<bool>(N, false));
    for (int i = 0; i < M; ++i) {
        int u = A[i] - 1;
        int v = B[i] - 1;
        adj[u][v] = adj[v][u] = true;
    }

    int K = 2 * N;
    std::vector<std::vector<int>> C(K, std::vector<int>(K));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int country1 = i + 1;
            int country2 = j + 1;

            if (i == j) {
                // Diagonal block: fill with the country's color
                for (int row_offset = 0; row_offset < 2; ++row_offset) {
                    for (int col_offset = 0; col_offset < 2; ++col_offset) {
                        C[2 * i + row_offset][2 * j + col_offset] = country1;
                    }
                }
            } else {
                if (adj[i][j]) {
                    // Adjacent countries: use a checkerboard pattern
                    C[2 * i][2 * j] = country1;
                    C[2 * i][2 * j + 1] = country2;
                    C[2 * i + 1][2 * j] = country2;
                    C[2 * i + 1][2 * j + 1] = country1;
                } else {
                    // Non-adjacent countries: fill with a single color to avoid adjacency
                    // Use min to have a consistent symmetric choice
                    int fill_color = std::min(country1, country2);
                    for (int row_offset = 0; row_offset < 2; ++row_offset) {
                        for (int col_offset = 0; col_offset < 2; ++col_offset) {
                            C[2 * i + row_offset][2 * j + col_offset] = fill_color;
                        }
                    }
                }
            }
        }
    }

    return C;
}