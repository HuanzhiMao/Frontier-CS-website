#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// Adjacency matrix, 1-indexed for convenience
int adj[101][101];
const int N = 100;

// Helper function to make a query and flush output
int query(int a, int b, int c) {
    std::cout << "? " << a << " " << b << " " << c << std::endl;
    int response;
    std::cin >> response;
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // --- Step 1: Query information relative to base vertices {1, 2, 3} ---
    int q123 = query(1, 2, 3);
    std::vector<int> q12k(N + 1), q13k(N + 1), q23k(N + 1);
    for (int k = 4; k <= N; ++k) {
        q12k[k] = query(1, 2, k);
        q13k[k] = query(1, 3, k);
        q23k[k] = query(2, 3, k);
    }
    
    // --- Step 2: Generate candidates for the base triangle {1, 2, 3} ---
    std::vector<std::vector<int>> candidates;
    if (q123 == 0) candidates.push_back({0, 0, 0});
    else if (q123 == 1) { candidates.push_back({1, 0, 0}); candidates.push_back({0, 1, 0}); candidates.push_back({0, 0, 1}); }
    else if (q123 == 2) { candidates.push_back({1, 1, 0}); candidates.push_back({1, 0, 1}); candidates.push_back({0, 1, 1}); }
    else candidates.push_back({1, 1, 1});

    int e12 = -1, e13 = -1, e23 = -1;

    // --- Step 3: Verify candidates to find the correct base triangle ---
    int check_q145 = query(1, 4, 5);
    int check_q245 = query(2, 4, 5);
    
    for (const auto& cand : candidates) {
        int cand_e12 = cand[0];
        int cand_e13 = cand[1];
        int cand_e23 = cand[2];

        // For this candidate, derive edges from {1,2} to {4,5} to use in the check
        int sum_k4 = (q12k[4] - cand_e12) + (q13k[4] - cand_e13) + (q23k[4] - cand_e23);
        if (sum_k4 % 2 != 0) continue;
        int s4 = sum_k4 / 2;
        int cand_e14 = s4 - (q23k[4] - cand_e23);
        int cand_e24 = s4 - (q13k[4] - cand_e13);
        
        int sum_k5 = (q12k[5] - cand_e12) + (q13k[5] - cand_e13) + (q23k[5] - cand_e23);
        if (sum_k5 % 2 != 0) continue;
        int s5 = sum_k5 / 2;
        int cand_e15 = s5 - (q23k[5] - cand_e23);
        int cand_e25 = s5 - (q13k[5] - cand_e13);

        int cand_e45 = check_q145 - cand_e14 - cand_e15;

        // Check if derived edge values are valid (0 or 1)
        if (cand_e14 < 0 || cand_e14 > 1 || cand_e24 < 0 || cand_e24 > 1 || 
            cand_e15 < 0 || cand_e15 > 1 || cand_e25 < 0 || cand_e25 > 1 || 
            cand_e45 < 0 || cand_e45 > 1) {
            continue;
        }

        // Perform the verification check using an independent query
        if (check_q245 == cand_e24 + cand_e25 + cand_e45) {
            e12 = cand_e12;
            e13 = cand_e13;
            e23 = cand_e23;
            break;
        }
    }

    adj[1][2] = adj[2][1] = e12;
    adj[1][3] = adj[3][1] = e13;
    adj[2][3] = adj[3][2] = e23;

    // --- Step 4: Determine all edges incident to {1, 2, 3} ---
    std::vector<int> e1k(N + 1);
    for (int k = 4; k <= N; ++k) {
        int sum_k = (q12k[k] - e12) + (q13k[k] - e13) + (q23k[k] - e23);
        int s = sum_k / 2;
        adj[1][k] = adj[k][1] = s - (q23k[k] - e23);
        adj[2][k] = adj[k][2] = s - (q13k[k] - e13);
        adj[3][k] = adj[k][3] = s - (q12k[k] - e12);
    }
    for (int k = 2; k <= N; ++k) e1k[k] = adj[1][k];
    
    // --- Step 5: Determine remaining edges using vertex 1 as pivot ---
    for (int i = 4; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            int q;
            if (i == 4 && j == 5) {
                q = check_q145; // Reuse already made query
            } else {
                q = query(1, i, j);
            }
            adj[i][j] = adj[j][i] = q - e1k[i] - e1k[j];
        }
    }

    // --- Output the final adjacency matrix ---
    std::cout << "!" << std::endl;
    for (int i = 1; i <= N; ++i) {
        std::string row = "";
        for (int j = 1; j <= N; ++j) {
            row += std::to_string(adj[i][j]);
        }
        std::cout << row << std::endl;
    }

    return 0;
}