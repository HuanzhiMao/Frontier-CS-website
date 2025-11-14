#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

// Function to perform a query
int ask(int n, const std::vector<int>& q) {
    std::cout << "0";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << q[i];
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Function to submit the final guess
void guess(int n, const std::vector<int>& p) {
    std::cout << "1";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 1) {
        std::vector<int> p = {1};
        guess(n, p);
        return 0;
    }

    int B = 0;
    while ((1 << B) <= n) {
        B++;
    }

    // x_k1[i] will store k_1 XOR k_i, where k_v is the position of value v.
    std::vector<int> x_k1(n + 1, 0);

    // For each value i from 2 to n, find k_1 XOR k_i
    for (int i = 2; i <= n; ++i) {
        int current_x = 0;
        // Determine the XOR sum bit by bit
        for (int b = 0; b < B; ++b) {
            std::vector<int> q(n);
            // Construct the query based on the b-th bit of the position
            for (int j = 1; j <= n; ++j) {
                if ((j >> b) & 1) {
                    q[j - 1] = 1;
                } else {
                    q[j - 1] = i;
                }
            }
            int response = ask(n, q);
            // If response is not 1, the b-th bits of k_1 and k_i are different.
            if (response != 1) {
                current_x |= (1 << b);
            }
        }
        x_k1[i] = current_x;
    }

    // Find k_1 by testing all possible values from 1 to n.
    int k1 = -1;
    for (int g = 1; g <= n; ++g) {
        std::vector<int> s;
        s.push_back(g);
        for (int i = 2; i <= n; ++i) {
            s.push_back(g ^ x_k1[i]);
        }

        bool is_perm = true;
        std::vector<bool> seen(n + 1, false);
        for (int val : s) {
            if (val < 1 || val > n || seen[val]) {
                is_perm = false;
                break;
            }
            seen[val] = true;
        }

        if (is_perm) {
            k1 = g;
            break;
        }
    }

    // Reconstruct the inverse permutation, where p_inv[v] = position of value v.
    std::vector<int> p_inv(n + 1);
    p_inv[1] = k1;
    for (int i = 2; i <= n; ++i) {
        p_inv[i] = k1 ^ x_k1[i];
    }

    // Reconstruct the permutation p from its inverse.
    std::vector<int> p(n);
    for (int i = 1; i <= n; ++i) {
        // p_inv[i] is 1-based position of value i
        p[p_inv[i] - 1] = i;
    }

    guess(n, p);

    return 0;
}