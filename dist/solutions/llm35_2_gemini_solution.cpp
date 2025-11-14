#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Function to ask a query on a subsegment of a vector of indices.
// It asks if value x exists in the hidden array at any of the given indices.
bool ask_query(int x, const std::vector<int>& S, int start_idx, int count) {
    if (count <= 0) {
        return false;
    }
    std::cout << "? " << x << " " << count;
    for (int i = 0; i < count; ++i) {
        std::cout << " " << S[start_idx + i];
    }
    std::cout << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response == 1;
}

// Binary search to find the first position of x in a sorted list of indices U.
// Returns the index from the array `a` or -1 if not found.
int find_pos(int x, const std::vector<int>& U) {
    if (U.empty()) {
        return -1;
    }

    int low = 0, high = U.size() - 1;
    int first_occurrence_sub_idx = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (ask_query(x, U, low, mid - low + 1)) {
            first_occurrence_sub_idx = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (first_occurrence_sub_idx != -1) {
        return U[first_occurrence_sub_idx];
    }
    return -1;
}

void solve() {
    int n;
    std::cin >> n;
    if (n == -1) exit(0);

    std::vector<int> unpaired_indices;
    unpaired_indices.reserve(2 * n - 1);
    for (int i = 1; i <= 2 * n - 1; ++i) {
        unpaired_indices.push_back(i);
    }

    for (int x = 1; x <= n; ++x) {
        int p1 = find_pos(x, unpaired_indices);
        
        if (p1 == -1) {
            // This value x is not present among the remaining unpaired indices.
            // This means it has already been fully paired.
            continue;
        }

        unpaired_indices.erase(std::remove(unpaired_indices.begin(), unpaired_indices.end(), p1), unpaired_indices.end());

        int p2 = find_pos(x, unpaired_indices);
        
        if (p2 == -1) {
            // Found only one occurrence of x, so it's the unique number.
            std::cout << "! " << x << std::endl;
            return;
        }
        
        unpaired_indices.erase(std::remove(unpaired_indices.begin(), unpaired_indices.end(), p2), unpaired_indices.end());
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