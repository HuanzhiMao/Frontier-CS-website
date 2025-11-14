#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0);
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n);
    int pivot_idx = 1;

    std::vector<int> or_with_pivot(n + 1);
    int pivot_val = -1; // Represents all bits set

    for (int i = 1; i <= n; ++i) {
        if (i == pivot_idx) continue;
        or_with_pivot[i] = query(pivot_idx, i);
        if (pivot_val == -1) {
            pivot_val = or_with_pivot[i];
        } else {
            pivot_val &= or_with_pivot[i];
        }
    }

    p[pivot_idx - 1] = pivot_val;

    std::vector<bool> used(n, false);
    used[pivot_val] = true;

    std::map<int, int> val_map;
    for (int v = 0; v < n; ++v) {
        if (!used[v]) {
            val_map[pivot_val | v] = v;
        }
    }

    for (int i = 1; i <= n; ++i) {
        if (i == pivot_idx) continue;
        p[i - 1] = val_map[or_with_pivot[i]];
    }

    std::cout << "! ";
    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}