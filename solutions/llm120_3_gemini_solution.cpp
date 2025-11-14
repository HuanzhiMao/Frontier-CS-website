#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// Function to perform a query and read the response.
// Vertices are 0-indexed in the code and 1-indexed for the interactor.
int query(int a, int b, int c) {
    std::cout << "? " << a + 1 << " " << b + 1 << " " << c + 1 << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) exit(0); // Exit on error
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int n = 100;
    std::vector<std::vector<int>> adj(n, std::vector<int>(n, 0));

    // Step 1: Find a 3-vertex basis {p1, p2, p3}
    // By Ramsey's theorem R(3,3)=6, such a basis must exist among any 6 vertices.
    int p1 = -1, p2 = -1, p3 = -1;
    for (int i = 0; i < 6 && p1 == -1; ++i) {
        for (int j = i + 1; j < 6 && p1 == -1; ++j) {
            for (int k = j + 1; k < 6 && p1 == -1; ++k) {
                int res = query(i, j, k);
                if (res == 0 || res == 3) {
                    p1 = i;
                    p2 = j;
                    p3 = k;
                    int edge_val = (res == 3);
                    adj[p1][p2] = adj[p2][p1] = edge_val;
                    adj[p1][p3] = adj[p3][p1] = edge_val;
                    adj[p2][p3] = adj[p3][p2] = edge_val;
                }
            }
        }
    }

    // Step 2: Determine all edges incident to the basis vertices.
    std::vector<int> other_vertices;
    for (int i = 0; i < n; ++i) {
        if (i != p1 && i != p2 && i != p3) {
            other_vertices.push_back(i);
        }
    }

    for (int k : other_vertices) {
        int q12k = query(p1, p2, k);
        int q13k = query(p1, p3, k);
        int q23k = query(p2, p3, k);

        int sum12 = q12k - adj[p1][p2];
        int sum13 = q13k - adj[p1][p3];
        int sum23 = q23k - adj[p2][p3];

        // We have:
        // E(p1,k) + E(p2,k) = sum12
        // E(p1,k) + E(p3,k) = sum13
        // E(p2,k) + E(p3,k) = sum23
        // Summing these gives: 2 * (E(p1,k) + E(p2,k) + E(p3,k)) = sum12 + sum13 + sum23
        int total_sum_of_edges = (sum12 + sum13 + sum23) / 2;
        
        int e_p1_k = total_sum_of_edges - sum23;
        int e_p2_k = total_sum_of_edges - sum13;
        int e_p3_k = total_sum_of_edges - sum12;

        adj[p1][k] = adj[k][p1] = e_p1_k;
        adj[p2][k] = adj[k][p2] = e_p2_k;
        adj[p3][k] = adj[k][p3] = e_p3_k;
    }
    
    // Step 3: Determine the remaining edges between non-basis vertices.
    // We can use p1 as a pivot for all remaining queries.
    for (size_t i = 0; i < other_vertices.size(); ++i) {
        for (size_t j = i + 1; j < other_vertices.size(); ++j) {
            int u = other_vertices[i];
            int v = other_vertices[j];
            int res = query(p1, u, v);
            // res = E(p1,u) + E(p1,v) + E(u,v). We know E(p1,u) and E(p1,v).
            adj[u][v] = adj[v][u] = res - adj[p1][u] - adj[p1][v];
        }
    }

    // Output the reconstructed graph.
    std::cout << "!" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::string row = "";
        for (int j = 0; j < n; ++j) {
            row += std::to_string(adj[i][j]);
        }
        std::cout << row << std::endl;
    }

    return 0;
}