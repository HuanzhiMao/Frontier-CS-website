#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

void solve() {
    int n;
    std::cin >> n;

    std::vector<int> candidates;
    candidates.reserve(n);
    for (int i = 1; i <= n; ++i) {
        candidates.push_back(i);
    }
    std::vector<std::vector<int>> histories(n + 1);

    while (candidates.size() > 2) {
        int min_c = n + 1, max_c = 0;
        for (int c : candidates) {
            if (c < min_c) min_c = c;
            if (c > max_c) max_c = c;
        }

        int l = min_c;
        int r = (min_c + max_c) / 2;
        
        // This query range works well. 
        // If min_c == max_c, we would not be in the loop.
        // If max_c = min_c + 1, r becomes min_c. Query is ? min_c, min_c, which splits them.

        std::cout << "? " << l << " " << r << std::endl;
        int x;
        std::cin >> x;
        int len = r - l + 1;
        int info = (x == len);

        std::vector<int> next_candidates;
        next_candidates.reserve(candidates.size());
        for (int c : candidates) {
            bool is_in = (c >= l && c <= r);
            int D_k = (1 - info) ^ is_in;
            
            histories[c].push_back(D_k);
            if (histories[c].size() >= 3) {
                size_t k = histories[c].size();
                if (histories[c][k - 1] == histories[c][k - 2] && histories[c][k - 2] == histories[c][k - 3]) {
                    // This candidate is impossible, eliminate.
                } else {
                    next_candidates.push_back(c);
                }
            } else {
                next_candidates.push_back(c);
            }
        }
        candidates = next_candidates;
    }

    for (int c : candidates) {
        std::cout << "! " << c << std::endl;
        int y;
        std::cin >> y;
        if (y == 1) {
            break;
        }
    }
    
    std::cout << "#" << std::endl;
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