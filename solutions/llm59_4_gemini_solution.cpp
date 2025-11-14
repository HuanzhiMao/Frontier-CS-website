#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <set>

// Function to perform a query
char query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    char response;
    std::cin >> response;
    return response;
}

// Function to output the final answer
void answer(int n, const std::vector<int>& a) {
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;
}

// Custom insertion sort on indices to control query count
void insertion_sort_indices(std::vector<int>& p) {
    for (size_t i = 1; i < p.size(); i++) {
        int key_pos = p[i];
        int j = i - 1;
        while (j >= 0 && query(p[j], key_pos) == '>') {
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = key_pos;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> a(n + 1, 0);

    int i = 1;
    while (i <= n) {
        int remaining_len = n - i + 1;
        if (remaining_len >= 5) {
            // Main case: process a block of 3, using 5 positions
            std::set<int> s_im1;
            for(int k=1; k<i; ++k) {
                s_im1.insert(a[k]);
            }
            
            std::vector<int> candidates;
            for (int k = 1; k <= i + 4; ++k) {
                if (s_im1.find(k) == s_im1.end()) {
                    candidates.push_back(k);
                }
            }
            
            std::vector<int> p = {i, i + 1, i + 2, i + 3, i + 4};
            
            insertion_sort_indices(p);

            for(int k=0; k<5; ++k) {
                a[p[k]] = candidates[k];
            }

            i += 3;
        } else {
            // Handle the tail end of the array
            std::vector<int> remaining_vals;
            std::set<int> s_im1;
            for(int k=1; k<i; ++k) {
                s_im1.insert(a[k]);
            }

            for (int k = 1; k <= n; ++k) {
                if (s_im1.find(k) == s_im1.end()) {
                    remaining_vals.push_back(k);
                }
            }

            std::vector<int> remaining_pos;
            for (int k = i; k <= n; ++k) {
                remaining_pos.push_back(k);
            }
            
            if (!remaining_pos.empty()) {
                insertion_sort_indices(remaining_pos);
                for (size_t k = 0; k < remaining_pos.size(); ++k) {
                    a[remaining_pos[k]] = remaining_vals[k];
                }
            }
            i = n + 1; // Exit loop
        }
    }

    answer(n, a);

    return 0;
}