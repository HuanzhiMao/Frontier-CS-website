#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

void solve() {
    // The pivot value to distinguish small and large W.
    // A value around sqrt(100000) = 316 could be optimal, but 400 is also effective.
    const int PIVOT = 400;

    // Query 1: Check if W is small or large.
    std::cout << "? 1 " << PIVOT << std::endl;
    int L1;
    std::cin >> L1;

    if (L1 == 0) { // Case 1: W is small (W < PIVOT)
        int low = 1, high = PIVOT - 1;
        // The number of candidates for W is high - low + 1.
        int k = high - low + 1;

        // Query 2 for small W.
        // Article: [low - 1, 1, 1, ..., 1] (k ones).
        // The first word is low-1 so it's guaranteed to be <= W.
        // The number of 1s that fit on the first line depends on W.
        // Total line count will be unique for each W in [low, high].
        std::cout << "? " << k + 1 << " " << low - 1;
        for (int i = 0; i < k; ++i) {
            std::cout << " 1";
        }
        std::cout << std::endl;

        int L2;
        std::cin >> L2;

        // For a given w in [low, high], the number of lines is:
        // 1 + floor((high - 1) / w). This function is injective in this range.
        for (int w = low; w <= high; ++w) {
            long long expected_lines = 1 + (high - 1) / w;
            if (expected_lines == L2) {
                std::cout << "! " << w << std::endl;
                return;
            }
        }
    } else { // Case 2: W is large (W >= PIVOT)
        // Query 2 for large W.
        // An article with many 1s.
        int N = 100000;
        std::cout << "? " << N << " 1";
        for (int i = 1; i < N; ++i) {
            std::cout << " 1";
        }
        std::cout << std::endl;

        int L2;
        std::cin >> L2;

        // The number of lines L is ceil(N/W).
        // This gives a range for W: (N-1)/L < W <= (N-1)/(L-1).
        // For W >= PIVOT, this range contains a single integer.
        // That integer is floor((N-1)/(L-1)).
        if (L2 == 1) { // Special case, W >= N
             std::cout << "! " << 100000 << std::endl;
        } else {
             int w_candidate = (N - 1) / (L2 - 1);
             std::cout << "! " << w_candidate << std::endl;
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