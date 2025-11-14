#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>

// Global variables for convenience in a competitive programming context
int N_global;
int M1_global, M2_global;

// Function to perform a query and get the median values
std::pair<int, int> do_query(const std::vector<int>& indices) {
    std::cout << "0 " << indices.size();
    for (int idx : indices) {
        std::cout << " " << idx;
    }
    std::cout << std::endl;
    int m1, m2;
    std::cin >> m1 >> m2;
    return {m1, m2};
}

// Function to check if removing two indices changes the global medians
bool is_change(int i, int j) {
    std::vector<int> q_indices;
    for (int k = 1; k <= N_global; ++k) {
        if (k != i && k != j) {
            q_indices.push_back(k);
        }
    }
    std::pair<int, int> res = do_query(q_indices);
    if (res.first > res.second) std::swap(res.first, res.second);
    return !(res.first == M1_global && res.second == M2_global);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N_global;

    // Step 1: Query all elements to find the global median values M1, M2
    std::vector<int> all_indices(N_global);
    std::iota(all_indices.begin(), all_indices.end(), 1);
    std::pair<int, int> global_medians = do_query(all_indices);
    M1_global = global_medians.first;
    M2_global = global_medians.second;
    if (M1_global > M2_global) std::swap(M1_global, M2_global);

    // Step 2: Find two non-median indices b1, b2, or find the medians directly.
    // An index i is a median index iff for any non-median index j, is_change(i, j) is true.
    // is_change(i, j) is true iff i or j is a median index.
    int b1 = -1, b2 = -1;

    bool c12 = is_change(1, 2);
    if (!c12) {
        b1 = 1; b2 = 2;
    } else { // At least one of {1, 2} is a median
        bool c13 = is_change(1, 3);
        if (!c13) {
            b1 = 1; b2 = 3;
        } else { // At least one of {1, 3} is a median
            bool c23 = is_change(2, 3);
            if (!c23) {
                b1 = 2; b2 = 3;
            } else { // Two of {1, 2, 3} are median indices
                bool c14 = is_change(1, 4);
                if (!c14) { // 1 and 4 are not medians -> medians must be 2, 3
                    std::cout << "1 2 3" << std::endl;
                    return 0;
                }
                bool c24 = is_change(2, 4);
                if (!c24) { // 2 and 4 are not medians -> medians must be 1, 3
                    std::cout << "1 1 3" << std::endl;
                    return 0;
                }
                // By elimination, medians must be 1, 2
                std::cout << "1 1 2" << std::endl;
                return 0;
            }
        }
    }
    
    // Step 3: Use one non-median index `b1` to test all other indices.
    std::vector<int> median_indices;
    for (int i = 1; i <= N_global; ++i) {
        if (i == b1) continue;
        if (is_change(i, b1)) {
            median_indices.push_back(i);
        }
    }
    
    std::cout << "1 " << median_indices[0] << " " << median_indices[1] << std::endl;

    return 0;
}