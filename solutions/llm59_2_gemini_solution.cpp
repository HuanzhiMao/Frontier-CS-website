#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

char ask(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    char response;
    std::cin >> response;
    return response;
}

void sort_4_indices(std::vector<int>& indices) {
    if (ask(indices[0], indices[1]) == '>') std::swap(indices[0], indices[1]);
    if (ask(indices[2], indices[3]) == '>') std::swap(indices[2], indices[3]);
    if (ask(indices[0], indices[2]) == '>') std::swap(indices[0], indices[2]);
    if (ask(indices[1], indices[3]) == '>') std::swap(indices[1], indices[3]);
    if (ask(indices[1], indices[2]) == '>') std::swap(indices[1], indices[2]);
}

void sort_indices_custom(std::vector<int>& indices) {
    if (indices.size() <= 1) return;
    std::sort(indices.begin(), indices.end(), [](int i, int j) {
        return ask(i, j) == '<';
    });
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n + 1, 0);
    int n_solved = 0;
    int max_val_so_far = 0;
    int max_val_pos = -1;

    while (n_solved < n) {
        int block_size = std::min(5, n - n_solved);
        if (block_size == 0) break;

        std::vector<int> current_indices;
        for (int i = 0; i < block_size; ++i) {
            current_indices.push_back(n_solved + 1 + i);
        }

        std::vector<int> sorted_indices;
        
        if (block_size == 5) {
            std::vector<int> to_sort_4;
            for(int i = 0; i < 4; ++i) to_sort_4.push_back(current_indices[i]);
            sort_4_indices(to_sort_4);
            
            int elem_to_insert = current_indices[4];
            
            int low = 0, high = 4;
            while(low < high) {
                int mid = low + (high - low) / 2;
                if (ask(elem_to_insert, to_sort_4[mid]) == '<') {
                    high = mid;
                } else {
                    low = mid + 1;
                }
            }
            
            to_sort_4.insert(to_sort_4.begin() + low, elem_to_insert);
            sorted_indices = to_sort_4;
            
        } else {
            sorted_indices = current_indices;
            sort_indices_custom(sorted_indices);
        }
        
        if (n_solved == 0) {
            for (int i = 0; i < block_size; ++i) {
                p[sorted_indices[i]] = i + 1;
            }
            max_val_so_far = block_size;
            if (block_size > 0) {
                max_val_pos = sorted_indices.back();
            }
        } else {
            (void)ask(sorted_indices[0], max_val_pos);
            
            for (int i = 0; i < block_size; ++i) {
                p[sorted_indices[i]] = max_val_so_far + i + 1;
            }
            if (block_size > 0) {
                max_val_pos = sorted_indices.back();
            }
            max_val_so_far += block_size;
        }
        n_solved += block_size;
    }

    std::cout << "! ";
    for (int i = 1; i <= n; ++i) {
        std::cout << p[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}