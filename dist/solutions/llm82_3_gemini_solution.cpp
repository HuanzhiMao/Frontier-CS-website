#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to perform a query and handle exit on error
int query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) {
        exit(0);
    }
    return result;
}

// Function to print the final answer
void answer(const std::vector<int>& p) {
    std::cout << "! ";
    for (size_t i = 0; i < p.size(); ++i) {
        std::cout << p[i] << (i == p.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n);
    std::vector<bool> found(n, false);
    
    // Step 1: Query p[0] with all other elements
    int p0_idx = 0;
    std::vector<int> or_with_p0(n);
    for (int i = 1; i < n; ++i) {
        or_with_p0[i] = query(p0_idx + 1, i + 1);
    }

    // Step 2: Compute p[0]
    p[p0_idx] = (1 << 12) - 1; // A mask with all relevant bits set
    for (int i = 1; i < n; ++i) {
        p[p0_idx] &= or_with_p0[i];
    }
    found[p0_idx] = true;

    // Step 3: Find the index of 0
    int zero_idx = -1;
    if (p[p0_idx] == 0) {
        zero_idx = p0_idx;
    } else {
        for (int i = 1; i < n; ++i) {
            if (or_with_p0[i] == p[p0_idx]) {
                zero_idx = i;
                break;
            }
        }
    }
    
    p[zero_idx] = 0;
    found[zero_idx] = true;

    // Step 4: Find all other elements using the index of 0
    for (int i = 0; i < n; ++i) {
        if (!found[i]) {
            p[i] = query(zero_idx + 1, i + 1);
        }
    }

    answer(p);

    return 0;
}