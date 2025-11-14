#include <iostream>
#include <vector>
#include <numeric>

// Function to perform a query
int ask(int l, int r) {
    std::cout << "0 " << l << " " << r << std::endl;
    int response;
    std::cin >> response;
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 1) {
        std::cout << "1 1" << std::endl;
        return 0;
    }

    // Precompute inversion parities for all subarrays [i, j] where i < j
    std::vector<std::vector<int>> I(n + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            I[i][j] = ask(i, j);
        }
    }

    std::vector<int> p(n + 1);
    std::vector<bool> used(n + 1, false);

    // Determine p_n, p_{n-1}, ..., p_1 in reverse order
    for (int k = n; k >= 1; --k) {
        int count_larger = 0;
        if (k > 1) {
            // For a fixed k, we determine how many elements in {p_1, ..., p_{k-1}} are larger than p_k.
            // Let C(i, k) = |{j in [i, k-1] : p_j > p_k}| mod 2.
            // We have [p_i > p_k] = (C(i, k) - C(i+1, k)) mod 2 for i < k-1, and [p_{k-1} > p_k] = C(k-1, k).
            
            // C(k-1, k) = (I(k-1, k) - I(k-1, k-1)) mod 2 = I(k-1, k) since I(k-1, k-1) is 0.
            int C_next = I[k - 1][k];
            // The contribution from p_{k-1}
            count_larger += C_next;

            // Iterate downwards to calculate contributions from p_{k-2}, ..., p_1
            for (int i = k - 2; i >= 1; --i) {
                // C(i, k) = (I(i, k) - I(i, k-1)) mod 2
                int C_curr = (I[i][k] - I[i][k - 1] + 2) % 2;
                
                // [p_i > p_k] = (C(i, k) - C(i+1, k)) mod 2
                int is_p_i_gt_p_k = (C_curr - C_next + 2) % 2;
                count_larger += is_p_i_gt_p_k;
                
                C_next = C_curr;
            }
        }
        
        // The set of values {p_1, ..., p_k} is the set of k remaining available numbers.
        // count_larger is the number of elements in {p_1, ..., p_k} greater than p_k.
        // So, p_k is the (k - count_larger)-th smallest among the available numbers.
        int rank_in_U = k - count_larger;
        int current_val = 0;
        int count_available = 0;
        for (int v = 1; v <= n; ++v) {
            if (!used[v]) {
                count_available++;
                if (count_available == rank_in_U) {
                    current_val = v;
                    break;
                }
            }
        }
        p[k] = current_val;
        used[current_val] = true;
    }

    // Output the final answer
    std::cout << "1";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;

    return 0;
}