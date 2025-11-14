#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>

int n;
std::vector<std::vector<int>> cache;

int ask(int l, int r) {
    if (l > r) {
        return 0;
    }
    if (l == r) {
        return 0;
    }
    if (cache[l][r] != -1) {
        return cache[l][r];
    }
    std::cout << "0 " << l << " " << r << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) {
        exit(0);
    }
    return cache[l][r] = response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    cache.assign(n + 1, std::vector<int>(n + 1, -1));

    std::vector<int> p(n + 1);
    std::vector<bool> used(n + 1, false);

    for (int k = n; k >= 2; --k) {
        int K_k = 0;

        // Calculate K_k = sum over i=1..k-1 of [p_i > p_k]
        // Base case: [p_{k-1} > p_k]
        K_k += ask(k - 1, k);

        // Inductive step: [p_i > p_k] for i = 1..k-2
        for (int i = 1; i <= k - 2; ++i) {
            int q_i_k = ask(i, k);
            int q_i_km1 = ask(i, k - 1);
            int q_ip1_k = ask(i + 1, k);
            int q_ip1_km1 = ask(i + 1, k - 1);

            int R_i_k = (q_i_k - q_i_km1 + 2) % 2;
            int R_ip1_k = (q_ip1_k - q_ip1_km1 + 2) % 2;
            int p_i_gt_pk = (R_i_k - R_ip1_k + 2) % 2;
            K_k += p_i_gt_pk;
        }

        // p_k is the j-th smallest available number, where j = k - K_k
        int j = k - K_k;

        int count_unused = 0;
        int value_pk = -1;
        for (int val = 1; val <= n; ++val) {
            if (!used[val]) {
                count_unused++;
                if (count_unused == j) {
                    value_pk = val;
                    break;
                }
            }
        }
        p[k] = value_pk;
        used[p[k]] = true;
    }

    // p_1 is the last remaining unused number
    for (int val = 1; val <= n; ++val) {
        if (!used[val]) {
            p[1] = val;
            break;
        }
    }

    std::cout << "1 ";
    for (int i = 1; i <= n; ++i) {
        std::cout << p[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}