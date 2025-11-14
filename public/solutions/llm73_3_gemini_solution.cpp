#include <iostream>
#include <vector>
#include <numeric>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

// Function to perform a query
int ask(int l, int r) {
    // The problem statement guarantees 1 <= l <= r <= n for queries.
    // For l=r, the number of inversions is 0.
    if (l >= r) {
        return 0;
    }
    std::cout << "0 " << l << " " << r << std::endl;
    int res;
    std::cin >> res;
    return res;
}

int main() {
    fast_io();

    int n;
    std::cin >> n;

    if (n == 1) {
        std::cout << "1 1" << std::endl;
        return 0;
    }

    // Store results of all necessary queries.
    // Padded with an extra row/column for easier calculations later.
    std::vector<std::vector<int>> inversions(n + 2, std::vector<int>(n + 2, 0));
    for (int l = 1; l <= n; ++l) {
        for (int r = l + 1; r <= n; ++r) {
            inversions[l][r] = ask(l, r);
        }
    }

    // Determine pairwise comparisons [p_i > p_j] for all i < j.
    std::vector<std::vector<int>> is_greater(n + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            // The parity of inversions involving p_i in p_{i+1...r} is D(i,r).
            // D(i,r) mod 2 = (I(i,r) - I(i+1,r)) mod 2
            // [p_i > p_j] mod 2 = (D(i,j) - D(i, j-1)) mod 2
            int d_ij = inversions[i][j] ^ inversions[i+1][j];
            int d_i_j_minus_1 = inversions[i][j-1] ^ inversions[i+1][j-1];
            is_greater[i][j] = d_ij ^ d_i_j_minus_1;
        }
    }
    
    // Reconstruct the permutation from pairwise comparisons.
    std::vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i) {
        int count_smaller = 0;
        // Count elements p_j with j < i that are smaller than p_i
        for (int j = 1; j < i; ++j) {
            // is_greater[j][i] is [p_j > p_i].
            // If p_j < p_i, then [p_j > p_i] is 0.
            count_smaller += (1 - is_greater[j][i]);
        }
        // Count elements p_j with j > i that are smaller than p_i
        for (int j = i + 1; j <= n; ++j) {
            // is_greater[i][j] is [p_i > p_j].
            // If p_j < p_i, then [p_i > p_j] is 1.
            count_smaller += is_greater[i][j];
        }
        p[i] = count_smaller + 1;
    }

    // Print the final answer.
    std::cout << "1";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;

    return 0;
}