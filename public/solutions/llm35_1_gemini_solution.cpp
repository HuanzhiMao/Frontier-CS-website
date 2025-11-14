#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

// Function to perform a query
bool ask(int x, const std::vector<int>& S) {
    if (S.empty()) {
        return false;
    }
    std::cout << "? " << x << " " << S.size();
    for (int i : S) {
        std::cout << " " << i;
    }
    std::cout << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) {
        // Exit on invalid query or error from interactor
        exit(0);
    }
    return response == 1;
}

void solve() {
    int n;
    std::cin >> n;
    if (n == -1) {
        exit(0);
    }

    int array_size = 2 * n - 1;
    int num_bits = 0;
    if (array_size > 0) {
        int temp = array_size;
        while(temp > 0){
            temp >>= 1;
            num_bits++;
        }
    }
    
    // Precompute sets for each bit position
    std::vector<std::vector<int>> sets_by_bit(num_bits);
    for (int k = 0; k < num_bits; ++k) {
        for (int i = 1; i <= array_size; ++i) {
            if ((i >> k) & 1) {
                sets_by_bit[k].push_back(i);
            }
        }
    }

    // For each number x, find the bitwise OR of its positions
    std::vector<int> p_masks(n + 1, 0);
    for (int x = 1; x <= n; ++x) {
        for (int k = 0; k < num_bits; ++k) {
            if (ask(x, sets_by_bit[k])) {
                p_masks[x] |= (1 << k);
            }
        }
    }

    // Group numbers by their position-OR-mask
    std::map<int, std::vector<int>> groups;
    for (int x = 1; x <= n; ++x) {
        groups[p_masks[x]].push_back(x);
    }

    // Find all numbers that belong to a group of odd size.
    // The unique number must be one of these candidates.
    std::vector<int> candidates;
    for (auto const& [mask, vals] : groups) {
        if (vals.size() % 2 != 0) {
            for (int val : vals) {
                candidates.push_back(val);
            }
        }
    }
    
    // For each candidate, check if it is at the position given by its mask.
    // The unique number z will be at position p_z, and its mask will be p_z.
    // A paired number y at (p1, p2) has mask p1|p2. It is unlikely to be at position p1|p2.
    for (int x : candidates) {
        int mask = p_masks[x];
        if (mask > 0 && mask <= array_size) {
            std::vector<int> S = {mask};
            if (ask(x, S)) {
                std::cout << "! " << x << std::endl;
                return;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}