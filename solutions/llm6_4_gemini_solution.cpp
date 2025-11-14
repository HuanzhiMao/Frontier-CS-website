#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

std::vector<std::vector<int>> create_map(int N, int M,
    std::vector<int> A, std::vector<int> B) {
    
    std::vector<std::vector<bool>> adj(N + 1, std::vector<bool>(N + 1, false));
    for (int i = 0; i < M; ++i) {
        adj[A[i]][B[i]] = true;
        adj[B[i]][A[i]] = true;
    }

    int K = 2 * N;
    std::vector<std::vector<int>> C(K, std::vector<int>(K));

    for (int i = 1; i <= N; ++i) {
        // Main diagonal block
        int r_start = 2 * (i - 1);
        int c_start = 2 * (i - 1);
        C[r_start][c_start] = i;
        C[r_start][c_start + 1] = i;
        C[r_start + 1][c_start] = i;
        C[r_start + 1][c_start + 1] = i;
    }

    for (int u = 1; u <= N; ++u) {
        for (int v = u + 1; v <= N; ++v) {
            int r_start_uv = 2 * (u - 1);
            int c_start_uv = 2 * (v - 1);
            
            int r_start_vu = 2 * (v - 1);
            int c_start_vu = 2 * (u - 1);

            if (adj[u][v]) {
                // Block for (u, v)
                C[r_start_uv][c_start_uv] = u;
                C[r_start_uv][c_start_uv + 1] = v;
                C[r_start_uv + 1][c_start_uv] = u;
                C[r_start_uv + 1][c_start_uv + 1] = v;

                // Block for (v, u)
                C[r_start_vu][c_start_vu] = v;
                C[r_start_vu][c_start_vu + 1] = u;
                C[r_start_vu + 1][c_start_vu] = v;
                C[r_start_vu + 1][c_start_vu + 1] = u;
            } else {
                // Block for (u, v)
                C[r_start_uv][c_start_uv] = u;
                C[r_start_uv][c_start_uv + 1] = u;
                C[r_start_uv + 1][c_start_uv] = u;
                C[r_start_uv + 1][c_start_uv + 1] = u;

                // Block for (v, u)
                C[r_start_vu][c_start_vu] = v;
                C[r_start_vu][c_start_vu + 1] = v;
                C[r_start_vu + 1][c_start_vu] = v;
                C[r_start_vu + 1][c_start_vu + 1] = v;
            }
        }
    }

    return C;
}