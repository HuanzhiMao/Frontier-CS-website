#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

char query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    char response;
    std::cin >> response;
    return response;
}

void answer(int n, const std::vector<int>& p) {
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

void insertion_sort_indices(std::vector<int>& indices) {
    for (size_t i = 1; i < indices.size(); ++i) {
        int key = indices[i];
        int j = i - 1;
        while (j >= 0 && query(key, indices[j]) == '<') {
            indices[j + 1] = indices[j];
            j = j - 1;
        }
        indices[j + 1] = key;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n + 1, 0);
    std::vector<int> rem;
    int current_val = 1;

    for (int i = 1; i <= n; i += 3) {
        std::vector<int> current_block;
        for (int j = 0; j < 3 && i + j <= n; ++j) {
            current_block.push_back(i + j);
        }

        std::vector<int> pool = rem;
        for (int idx : current_block) {
            pool.push_back(idx);
        }
        
        insertion_sort_indices(pool);
        
        int to_assign_count = current_block.size();

        for (int k = 0; k < to_assign_count; ++k) {
            p[pool[k]] = current_val + k;
        }
        current_val += to_assign_count;

        rem.clear();
        for (size_t k = to_assign_count; k < pool.size(); ++k) {
            rem.push_back(pool[k]);
        }
    }
    
    if (!rem.empty()) {
        insertion_sort_indices(rem);
        for(size_t k = 0; k < rem.size(); ++k) {
            p[rem[k]] = current_val + k;
        }
    }

    answer(n, p);

    return 0;
}