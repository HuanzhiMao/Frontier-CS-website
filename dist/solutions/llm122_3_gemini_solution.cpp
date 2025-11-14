#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

long long query(const std::vector<int>& a) {
    if (a.empty()) {
        return 1;
    }
    std::cout << "? " << a.size();
    for (int x : a) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    long long response;
    std::cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response;
}

void answer(int w) {
    std::cout << "! " << w << std::endl;
}

void solve() {
    // Query 1: Use a large number of words of length 1 to get a coarse range for W.
    int n1 = 100000;
    std::vector<int> q1(n1, 1);
    long long L1 = query(q1);

    if (L1 == 1) {
        // All words fit on one line. W must be at least 100000.
        // Given W <= 100000, W must be 100000.
        answer(100000);
        return;
    }

    // From L1 = ceil(n1 / W), we get a range for W:
    // (L1 - 1) < n1 / W <= L1  =>  n1 / L1 <= W < n1 / (L1 - 1)
    int low_w = (n1 + L1 - 1) / L1;
    int high_w = (n1 - 1) / (L1 - 1);

    // Query 2: Use words of length `low_w` to pinpoint W.
    int m = low_w;
    if (m == 0) m=1; // Should not happen with W>=1
    std::vector<int> q2(m, m);
    long long L2 = query(q2);

    // L2 = ceil(m / floor(W/m)). Let q = floor(W/m).
    // From L2, we get a range for q.
    // L2-1 < m/q <= L2 => m/L2 <= q < m/(L2-1)
    long long q_min = (L2 == 0) ? 1 : (m + L2 - 1) / L2;
    long long q_max = (L2 <= 1) ? m : (m - 1) / (L2 - 1);
    
    int final_ans = -1;
    // Iterate through possible q values to find the one consistent with the range from Q1.
    for (long long q = q_max; q >= q_min; --q) {
        int current_low = std::max(low_w, (int)(q * m));
        int current_high = std::min(high_w, (int)((q + 1) * m - 1));
        if (current_low <= current_high) {
            // Found a valid range for W. Pick the largest possible value.
            final_ans = current_high;
            break;
        }
    }
    answer(final_ans);
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