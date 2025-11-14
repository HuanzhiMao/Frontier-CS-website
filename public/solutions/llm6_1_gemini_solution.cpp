#include <iostream>
#include <vector>
#include <numeric>

std::vector<std::vector<int>> create_map(int N, int M,
    std::vector<int> A, std::vector<int> B) {
    
    std::vector<std::vector<bool>> adj(N + 1, std::vector<bool>(N + 1, false));
    for (int i = 0; i < M; ++i) {
        adj[A[i]][B[i]] = true;
        adj[B[i]][A[i]] = true;
    }

    int K = 2 * N;
    std::vector<std::vector<int>> C(K, std::vector<int>(K));

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int country1 = r + 1;
            int country2 = c + 1;

            C[2 * r][2 * c] = country1;
            C[2 * r + 1][2 * c + 1] = country1;

            int filler_color;
            if (country1 == country2 || adj[country1][country2]) {
                filler_color = country2;
            } else {
                filler_color = country1;
            }
            C[2 * r][2 * c + 1] = filler_color;
            C[2 * r + 1][2 * c] = filler_color;
        }
    }

    return C;
}