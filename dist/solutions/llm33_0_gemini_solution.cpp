#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

void solve() {
    long long k;
    std::cin >> k;

    std::vector<int> ops;
    long long temp_k = k;
    while (temp_k > 2) {
        if (temp_k % 2 != 0) { // Odd k, must come from k-1
            ops.push_back(1); // Represents a "+1" operation
            temp_k--;
        } else { // Even k, can come from k/2
            ops.push_back(2); // Represents a "*2" operation
            temp_k /= 2;
        }
    }

    std::reverse(ops.begin(), ops.end());

    int n = ops.size() + 1;
    int n_s = 0;
    for (int op : ops) {
        if (op == 1) {
            n_s++;
        }
    }

    std::vector<int> p(n);
    p[0] = n_s;

    int s_val = n_s - 1;
    int l_val = n_s + 1;

    for (size_t i = 0; i < ops.size(); ++i) {
        if (ops[i] == 1) {
            p[i + 1] = s_val--;
        } else {
            p[i + 1] = l_val++;
        }
    }

    std::cout << n << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int q;
    std::cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}