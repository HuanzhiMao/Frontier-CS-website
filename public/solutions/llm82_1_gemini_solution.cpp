#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to perform a query
int query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) {
        exit(0); // Exit on error
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n + 1, -1); // Use 1-based indexing for permutation
    
    // Step 1: Pick a pivot and find its value
    int pivot_idx = 1;
    std::vector<int> or_with_pivot(n + 1);
    for (int i = 2; i <= n; ++i) {
        or_with_pivot[i] = query(pivot_idx, i);
    }
    
    p[pivot_idx] = (1 << 11) - 1; // All bits set for a number up to 2047
    for (int i = 2; i <= n; ++i) {
        p[pivot_idx] &= or_with_pivot[i];
    }
    
    // Step 2: Find the index of value 0
    std::vector<int> cand;
    cand.push_back(pivot_idx); // The pivot itself is a candidate
    for (int i = 2; i <= n; ++i) {
        if (or_with_pivot[i] == p[pivot_idx]) {
            cand.push_back(i);
        }
    }

    int zero_idx = -1;

    if (cand.size() == 1) {
        zero_idx = cand[0];
    } else {
        int anchor_idx = -1;
        for (int i = 1; i <= n; ++i) {
            bool is_candidate = false;
            for (int c : cand) {
                if (i == c) {
                    is_candidate = true;
                    break;
                }
            }
            if (!is_candidate) {
                anchor_idx = i;
                break;
            }
        }
        
        int min_or_val = -1;
        for (int cand_idx : cand) {
            int or_val = query(cand_idx, anchor_idx);
            if (zero_idx == -1 || or_val < min_or_val) {
                min_or_val = or_val;
                zero_idx = cand_idx;
            }
        }
    }
    p[zero_idx] = 0;

    // Step 3: Determine the rest of the permutation
    for (int i = 1; i <= n; ++i) {
        if (p[i] == -1) {
            p[i] = query(i, zero_idx);
        }
    }

    std::cout << "! ";
    for (int i = 1; i <= n; ++i) {
        std::cout << p[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}