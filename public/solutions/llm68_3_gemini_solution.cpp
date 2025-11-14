#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// This function solves a single test case.
void solve() {
    int n;
    std::cin >> n;

    // We start with all pens as candidates.
    std::vector<int> candidates(n);
    std::iota(candidates.begin(), candidates.end(), 0);

    // The strategy is to perform rounds of elimination. In each round, we query
    // every pen that is still a candidate. If a pen returns 0 (meaning it's out
    // of ink), we eliminate it. We continue this process until we are left with
    // a small number of candidates, from which we can make a final selection.

    // We continue eliminating pens as long as we have more than 3 candidates.
    // The reason for stopping at 3 (and not 2) is to allow for a more careful
    // selection process for the final pair, minimizing the ink used on the
    // pens we ultimately choose.
    while (candidates.size() > 3) {
        std::vector<int> next_candidates;
        for (int pen_idx : candidates) {
            // Query the pen.
            std::cout << "0 " << pen_idx << std::endl;
            int response;
            std::cin >> response;
            // If it still has ink, it survives to the next round.
            if (response == 1) {
                next_candidates.push_back(pen_idx);
            }
        }
        candidates = next_candidates;
    }
    
    // When exactly 3 candidates remain, we can identify the one with the least
    // ink among them by querying just two of them.
    if (candidates.size() == 3) {
        int u = candidates[0];
        int v = candidates[1];
        int w = candidates[2];
        
        int res_u, res_v;
        
        // Query two of the three candidates.
        std::cout << "0 " << u << std::endl;
        std::cin >> res_u;
        
        std::cout << "0 " << v << std::endl;
        std::cin >> res_v;

        if (res_u == 0) {
            // 'u' is empty, so it had the least ink of the three.
            // The other two, 'v' and 'w', must have more ink. We select them.
            std::cout << "1 " << v << " " << w << std::endl;
        } else if (res_v == 0) {
            // 'v' is empty, so it's the one to discard. Select 'u' and 'w'.
            std::cout << "1 " << u << " " << w << std::endl;
        } else {
            // Both 'u' and 'v' had ink. This means the third pen, 'w',
            // must have been the one with the least ink (which has now been depleted).
            // So we select 'u' and 'v'.
            std::cout << "1 " << u << " " << v << std::endl;
        }
        return;
    }

    // If, for some reason (e.g., small n), the loop terminates with 2 candidates,
    // we select them.
    if (candidates.size() == 2) {
        std::cout << "1 " << candidates[0] << " " << candidates[1] << std::endl;
    }
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}