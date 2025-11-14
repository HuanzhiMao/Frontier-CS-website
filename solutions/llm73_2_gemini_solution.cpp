#include <iostream>
#include <vector>

// Using 1-based indexing for arrays to match problem statement
int n;
// A[l][r] stores the parity of inversions in p[l...r]
std::vector<std::vector<int>> A;

// Helper to get from A, returns 0 if l >= r as inv count is 0
int get_A(int l, int r) {
    if (l >= r) {
        return 0;
    }
    return A[l][r];
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    // Resize storage for query results
    A.assign(n + 1, std::vector<int>(n + 1, 0));

    // Make all n*(n-1)/2 queries for l < r
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            std::cout << "0 " << i << " " << j << std::endl;
            std::cin >> A[i][j];
        }
    }

    // gt[i][j] will store [p_i > p_j] for i < j
    std::vector<std::vector<int>> gt(n + 1, std::vector<int>(n + 1, 0));
    
    // For every pair (i, j) with i < j, determine if p_i > p_j
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            // [p_i > p_j] mod 2 = (A(i,j) - A(i,j-1) - A(i+1,j) + A(i+1,j-1)) mod 2
            int val = get_A(i, j) - get_A(i, j - 1) - get_A(i + 1, j) + get_A(i + 1, j - 1);
            // Handle negative results of modulo correctly
            gt[i][j] = (val % 2 + 2) % 2;
        }
    }

    std::vector<int> p(n + 1);
    // For each position i, determine p_i
    for (int i = 1; i <= n; ++i) {
        // Count how many other elements p_i is greater than
        int greater_count = 0;
        for (int j = 1; j < i; ++j) {
            // We need [p_i > p_j], which is 1 - [p_j > p_i] = 1 - gt[j][i]
            greater_count += (1 - gt[j][i]);
        }
        for (int j = i + 1; j <= n; ++j) {
            greater_count += gt[i][j];
        }
        // The value of p_i is determined by its rank.
        // The rank is 1 + (number of other elements it is greater than).
        // In the set {1,...,n}, an element of rank k has value k.
        p[i] = greater_count + 1;
    }

    // Print the final answer
    std::cout << "1";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;

    return 0;
}