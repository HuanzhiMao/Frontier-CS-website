#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to perform a query and get the result
int do_query(int x) {
    std::cout << "? " << x << std::endl;
    int r;
    std::cin >> r;
    if (r == -1) exit(0); // Safety check for judge error
    return r;
}

// Function to output an answer
void do_answer(int a, int b) {
    std::cout << "! " << a << " " << b << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<bool> paired(2 * n + 1, false);
    std::vector<int> st; // Represents singletons currently in the device

    for (int i = 1; i <= 2 * n; ++i) {
        if (paired[i]) {
            continue;
        }

        int k_before = st.size();
        int k_after = do_query(i);

        if (k_after > k_before) {
            st.push_back(i);
        } else { // k_after == k_before, so the partner of i is in st
            int partner = -1;
            std::vector<int> temp_removed;

            while (!st.empty()) {
                int top = st.back();
                st.pop_back();
                
                int r = do_query(top);
                // After removing top, device has st (after pop) and i.
                // Total elements: st.size() + 1. Total distinct kinds: r.
                // If top is partner, i is a singleton, and all in st are singletons.
                // Thus, r should be st.size() + 1.
                if (r == st.size() + 1) {
                    partner = top;
                    break;
                } else {
                    temp_removed.push_back(top);
                }
            }
            
            do_answer(i, partner);
            paired[i] = true;
            paired[partner] = true;

            // Remove i from device to complete the pair removal
            do_query(i);
            
            // Add back non-partner elements that were temporarily removed during search
            std::reverse(temp_removed.begin(), temp_removed.end());
            for (int x : temp_removed) {
                do_query(x);
                st.push_back(x);
            }
        }
    }

    return 0;
}