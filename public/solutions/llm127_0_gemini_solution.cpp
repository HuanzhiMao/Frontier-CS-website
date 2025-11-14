#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Function to perform a query
std::pair<int, int> ask(int i) {
    std::cout << "? " << i << std::endl;
    int a0, a1;
    std::cin >> a0 >> a1;
    return {a0, a1};
}

// Function to submit the answer
void answer(int i) {
    std::cout << "! " << i << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    int k_best = n + 1;
    int final_ans = -1;

    // The number of distinct prize values is very small.
    // We can iterate, each time finding a better prize type,
    // until we find the diamond (type 1).
    while (true) {
        int l = 0, r = n - 1;
        int current_best_idx = -1;

        // Binary search for the leftmost prize that is better than k_best
        while (l <= r) {
            int mid = l + (r - l) / 2;
            std::pair<int, int> res = ask(mid);
            int s = res.first + res.second;

            if (s < k_best) {
                current_best_idx = mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        
        if (current_best_idx != -1) {
            // We found a better prize. Update k_best.
            // This index is our new best guess for the diamond's location.
            final_ans = current_best_idx;
            std::pair<int, int> res = ask(final_ans);
            k_best = res.first + res.second;
        } else {
            // No prize found is better than our current k_best.
            // This means final_ans holds a prize of the best type.
            // Since the diamond is unique, this must be it.
            break;
        }

        if (k_best == 0) {
            // We've found the diamond for sure.
            break;
        }
    }

    answer(final_ans);

    return 0;
}