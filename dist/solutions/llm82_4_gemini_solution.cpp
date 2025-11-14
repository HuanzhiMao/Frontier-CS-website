#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

// Function to perform a query and handle exit on error.
int query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) {
        exit(0);
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    std::vector<int> p(n, -1); // -1 indicates unsolved
    std::vector<int> or_with_1(n + 1);
    
    // Step 1: Find p[0] by querying it with all other elements.
    p[0] = (1 << 11) - 1;
    for (int i = 2; i <= n; ++i) {
        or_with_1[i] = query(1, i);
        p[0] &= or_with_1[i];
    }
    
    std::vector<bool> val_used(n, false);
    val_used[p[0]] = true;
    int solved_count = 1;
    
    // Step 2: Identify and solve for unambiguous elements.
    std::map<int, std::vector<int>> groups;
    for (int i = 2; i <= n; ++i) {
        groups[or_with_1[i]].push_back(i);
    }
    
    int known_idx_besides_1 = -1;

    for (auto const& [val, indices] : groups) {
        std::vector<int> candidates;
        for (int v = 0; v < n; ++v) {
            if (!val_used[v] && (p[0] | v) == val) {
                candidates.push_back(v);
            }
        }
        if (candidates.size() == 1) {
            int idx = indices[0];
            p[idx - 1] = candidates[0];
            val_used[p[idx - 1]] = true;
            solved_count++;
            if (known_idx_besides_1 == -1) {
                known_idx_besides_1 = idx;
            }
        }
    }

    // Step 3: If necessary, find the index of 0 to get a second known element.
    if (known_idx_besides_1 == -1 && solved_count < n) {
        int or_val_p0 = p[0];
        std::vector<int> zero_cand_indices = groups[or_val_p0];
        
        int anchor_idx = -1;
        for (int i = 2; i <= n; ++i) {
            bool is_in_group = false;
            for (int z_cand : zero_cand_indices) {
                if (i == z_cand) { is_in_group = true; break; }
            }
            if (!is_in_group) {
                anchor_idx = i; break;
            }
        }
        
        int min_or = (1 << 11);
        int zero_idx = -1;

        for (int idx : zero_cand_indices) {
            int res = query(idx, anchor_idx);
            if (res < min_or) {
                min_or = res;
                zero_idx = idx;
            }
        }
        p[zero_idx - 1] = 0;
        val_used[0] = true;
        solved_count++;
        known_idx_besides_1 = zero_idx;
    }

    // Step 4: Solve remaining ambiguous cases using the second known element.
    for (int i = 2; i <= n; ++i) {
        if (p[i - 1] == -1) {
            int res = query(i, known_idx_besides_1);
            for (int v = 0; v < n; ++v) {
                if (!val_used[v] && (p[0] | v) == or_with_1[i] && (p[known_idx_besides_1 - 1] | v) == res) {
                    p[i - 1] = v;
                    val_used[v] = true;
                    solved_count++;
                    break;
                }
            }
        }
    }

    // Output the final permutation.
    std::cout << "! ";
    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}