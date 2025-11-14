#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// This function creates a map for the given set of countries and adjacencies.
// N: the number of countries.
// M: the number of pairs of adjacent countries.
// A and B: arrays of length M describing adjacent countries.
// The function returns a 2D vector representing the map.
std::vector<std::vector<int>> create_map(int N, int M,
    std::vector<int> A, std::vector<int> B) {
    
    // Adjacency matrix to quickly check if two countries are adjacent.
    // Using N+1 size for 1-based indexing of countries.
    std::vector<std::vector<bool>> adj(N + 1, std::vector<bool>(N + 1, false));
    for (int i = 0; i < M; ++i) {
        adj[A[i]][B[i]] = true;
        adj[B[i]][A[i]] = true;
    }

    // The map is constructed as a grid of N x N blocks, each of size 3x3.
    // This gives a total map size of K x K, where K = 3*N.
    int K = 3 * N;
    std::vector<std::vector<int>> C(K, std::vector<int>(K));

    // Iterate over each cell of the K x K map.
    for (int r = 0; r < K; ++r) {
        for (int c = 0; c < K; ++c) {
            // Determine which block (u, v) this cell belongs to.
            // u and v are 0-indexed, from 0 to N-1.
            int u = r / 3;
            int v = c / 3;
            
            // Determine the cell's position within the 3x3 block.
            int r_rem = r % 3;
            int c_rem = c % 3;

            // Diagonal blocks (u == v) are filled with a single color.
            // Block (i, i) is entirely colored with country i+1.
            // This ensures every country appears on the map.
            if (u == v) {
                C[r][c] = u + 1;
            } else {
                // For off-diagonal blocks, determine colors based on adjacency.
                // u_c and v_c are the 0-indexed country indices, sorted.
                int u_c = std::min(u, v);
                int v_c = std::max(u, v);

                // A 3x3 block has a 1x1 center and an 8-cell border.
                bool is_center = (r_rem == 1 && c_rem == 1);

                if (is_center) {
                    // The center cell's color depends on whether the two countries
                    // corresponding to this block are adjacent.
                    // Countries are 1-indexed, so we use u_c+1 and v_c+1.
                    if (adj[u_c + 1][v_c + 1]) {
                        // If countries u_c+1 and v_c+1 are adjacent, color the center
                        // with v_c+1. This creates an adjacency with the border.
                        C[r][c] = v_c + 1;
                    } else {
                        // If not adjacent, use the border color to avoid creating an edge.
                        C[r][c] = u_c + 1;
                    }
                } else { // This is a border cell of the 3x3 block.
                    // All border cells of an off-diagonal block (u,v) are colored
                    // with the color of the country with the smaller index (u_c+1).
                    // This creates a buffer zone that prevents unwanted adjacencies
                    // between different blocks.
                    C[r][c] = u_c + 1;
                }
            }
        }
    }

    return C;
}