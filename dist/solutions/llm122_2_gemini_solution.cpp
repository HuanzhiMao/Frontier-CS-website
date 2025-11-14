#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

long long ceil_div(long long a, long long b) {
    if (b == 0) return 2e9; // Represents infinity for our purposes
    return (a + b - 1) / b;
}

void solve() {
    // Query 1 to get a primary range for W
    int n1 = 401;
    std::cout << "? " << n1;
    for (int i = 0; i < n1; ++i) {
        std::cout << " 1";
    }
    std::cout << std::endl;

    int l1;
    std::cin >> l1;

    // Based on L1, choose N2 for the second query
    int n2;
    if (l1 == 1) {
        n2 = 100000;
    } else {
        long long w_max_from_l1 = (n1 - 1) / (l1 - 1);
        n2 = w_max_from_l1;
    }

    // Query 2
    std::cout << "? " << n2;
    for (int i = 0; i < n2; ++i) {
        std::cout << " 1";
    }
    std::cout << std::endl;

    int l2;
    std::cin >> l2;

    // From l1 = ceil(n1/W), we have (l1-1) < n1/W <= l1
    // which gives n1/l1 <= W < n1/(l1-1) (for l1 > 1)
    long long w_min1 = ceil_div(n1, l1);
    long long w_max1 = 100000;
    if (l1 > 1) {
        w_max1 = (n1 - 1) / (l1 - 1);
    }

    // From l2 = ceil(n2/W), we have a similar range
    long long w_min2 = ceil_div(n2, l2);
    long long w_max2 = 100000;
    if (l2 > 1) {
        w_max2 = (n2 - 1) / (l2 - 1);
    }
    
    // Intersect the two ranges
    long long final_w_min = std::max(w_min1, w_min2);
    long long final_w_max = std::min(w_max1, w_max2);

    // Binary search for the largest W in the range that satisfies both conditions.
    // This W will be the unique answer.
    long long ans = final_w_min;
    long long low = final_w_min, high = final_w_max;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (mid == 0) { // Should not happen with W>=1
            low = 1;
            continue;
        }
        if (ceil_div(n1, mid) == l1 && ceil_div(n2, mid) == l2) {
            ans = mid;
            low = mid + 1; // Try to find a larger W that still works
        } else {
            // If mid doesn't work, all larger values won't work either.
            // A larger W would result in smaller or equal number of lines.
            // If ceil(N, mid) < L, then W must be smaller than mid.
            // If ceil(N, mid) > L, then W could be mid or larger.
            // The logic gets tricky. Let's check against L1.
            if (ceil_div(n1, mid) < l1) {
                // mid is too big, need fewer lines.
                high = mid - 1;
            } else {
                // mid might be too small, or just right.
                low = mid + 1;
            }
        }
    }

    std::cout << "! " << ans << std::endl;
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