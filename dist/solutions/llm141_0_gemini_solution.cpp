#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// Function to perform a query
bool query(int c) {
    std::cout << "? " << c << std::endl;
    char response;
    std::cin >> response;
    return response == 'Y';
}

// Function to reset memory
void reset() {
    std::cout << "R" << std::endl;
}

// Function to output the answer
void answer(int d) {
    std::cout << "! " << d << std::endl;
}

// Helper to find duplicates within a given set of bakeries
void find_dups_in_set(const std::vector<int>& bakeries, std::vector<bool>& is_dup, int k) {
    if (bakeries.size() <= 1) return;

    if (bakeries.size() <= k) {
        reset();
        for (int idx : bakeries) {
            if (!is_dup[idx] && query(idx)) {
                is_dup[idx] = true;
            }
        }
    } else {
        // For sets larger than k, simple forward/backward passes catch many duplicates.
        reset();
        for (int idx : bakeries) {
            if (!is_dup[idx] && query(idx)) {
                is_dup[idx] = true;
            }
        }

        std::vector<int> remaining_bakeries;
        for (int idx : bakeries) {
            if (!is_dup[idx]) {
                remaining_bakeries.push_back(idx);
            }
        }
        if (remaining_bakeries.size() <= 1) return;
        
        reset();
        std::reverse(remaining_bakeries.begin(), remaining_bakeries.end());
        for (int idx : remaining_bakeries) {
            if (!is_dup[idx] && query(idx)) {
                is_dup[idx] = true;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, k;
    std::cin >> n >> k;

    std::vector<bool> is_dup(n + 1, false);

    // Phase 1: Intra-block duplicates
    int num_blocks = n / k;
    for (int i = 0; i < num_blocks; ++i) {
        std::vector<int> block_bakeries;
        for (int j = 1; j <= k; ++j) {
            block_bakeries.push_back(i * k + j);
        }
        find_dups_in_set(block_bakeries, is_dup, k);
    }

    // Phase 2: Intra-stride duplicates
    int stride_len = n / k;
    if (stride_len > 1) {
        for (int i = 1; i <= k; ++i) {
            std::vector<int> stride_bakeries;
            for (int j = 0; j < stride_len; ++j) {
                int bakery_idx = j * k + i;
                if (!is_dup[bakery_idx]) {
                    stride_bakeries.push_back(bakery_idx);
                }
            }
            find_dups_in_set(stride_bakeries, is_dup, k);
        }
    }

    int distinct_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (!is_dup[i]) {
            distinct_count++;
        }
    }

    answer(distinct_count);

    return 0;
}