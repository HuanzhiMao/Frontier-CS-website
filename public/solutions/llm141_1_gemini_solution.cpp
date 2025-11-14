#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// Function to perform a query
char query(int c) {
    std::cout << "? " << c << std::endl;
    char response;
    std::cin >> response;
    return response;
}

// Function to perform a reset
void reset() {
    std::cout << "R" << std::endl;
}

// Function to output the final answer
void answer(int d) {
    std::cout << "! " << d << std::endl;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, k;
    std::cin >> n >> k;

    std::vector<bool> is_unique(n + 1, true);
    
    // Divide bakeries into n/k blocks of size k
    int num_blocks = n / k;

    // Phase 1: Find duplicates within each block
    for (int i = 0; i < num_blocks; ++i) {
        reset();
        for (int j = 1; j <= k; ++j) {
            int bakery_idx = i * k + j;
            char res = query(bakery_idx);
            if (res == 'Y') {
                is_unique[bakery_idx] = false;
            }
        }
    }

    // Phase 2: Find duplicates between blocks
    for (int i = 0; i < num_blocks; ++i) {
        for (int j = i + 1; j < num_blocks; ++j) {
            reset();
            // Load unique candidates from block i into memory
            for (int l = 1; l <= k; ++l) {
                int bakery_idx = i * k + l;
                if (is_unique[bakery_idx]) {
                    query(bakery_idx);
                }
            }
            // Check unique candidates from block j against memory
            for (int l = 1; l <= k; ++l) {
                int bakery_idx = j * k + l;
                if (is_unique[bakery_idx]) {
                    char res = query(bakery_idx);
                    if (res == 'Y') {
                        is_unique[bakery_idx] = false;
                    }
                }
            }
        }
    }

    // Count the total number of unique bakeries
    int distinct_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (is_unique[i]) {
            distinct_count++;
        }
    }

    answer(distinct_count);

    return 0;
}