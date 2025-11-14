#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

char query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    char response;
    std::cin >> response;
    if (response == 0) exit(1);
    return response;
}

void answer(int n, const std::vector<int>& p) {
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

std::vector<int> merge_sort_indices(const std::vector<int>& indices) {
    if (indices.size() <= 1) {
        return indices;
    }
    
    size_t mid = indices.size() / 2;
    std::vector<int> left_half(indices.begin(), indices.begin() + mid);
    std::vector<int> right_half(indices.begin() + mid, indices.end());
    
    std::vector<int> sorted_left = merge_sort_indices(left_half);
    std::vector<int> sorted_right = merge_sort_indices(right_half);
    
    std::vector<int> result;
    result.reserve(indices.size());
    
    size_t i = 0, j = 0;
    while (i < sorted_left.size() && j < sorted_right.size()) {
        if (query(sorted_left[i], sorted_right[j]) == '<') {
            result.push_back(sorted_left[i]);
            i++;
        } else {
            result.push_back(sorted_right[j]);
            j++;
        }
    }
    
    while (i < sorted_left.size()) {
        result.push_back(sorted_left[i]);
        i++;
    }
    
    while (j < sorted_right.size()) {
        result.push_back(sorted_right[j]);
        j++;
    }
    
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    std::vector<int> p(n + 1, 0);
    std::vector<bool> determined(n + 1, false);
    
    std::set<int> rem_vals;

    for (int i = 1; i <= n; i += 3) {
        std::vector<int> probe_indices;
        for (int j = 0; j < 5 && i + j <= n; ++j) {
            probe_indices.push_back(i + j);
        }

        if (probe_indices.empty()) {
            break;
        }

        std::vector<int> sorted_indices = merge_sort_indices(probe_indices);
        
        std::set<int> determined_values;
        for(int j = 1; j < i; ++j) {
            if (determined[j]) {
                determined_values.insert(p[j]);
            }
        }
        
        std::vector<int> cand_values;
        if (i == 1) {
            for (size_t j = 0; j < probe_indices.size(); ++j) {
                cand_values.push_back(j + 1);
            }
        } else {
            std::set<int> full_cand_set = rem_vals;
            for (int j = 0; j < 3 && i + j <= n; ++j) {
                full_cand_set.insert(i + j);
            }
            cand_values.assign(full_cand_set.begin(), full_cand_set.end());
        }
        
        std::sort(cand_values.begin(), cand_values.end());

        // This block handles cases where the number of available candidate values might not match
        // the number of probe_indices due to boundary conditions and logic for candidate set generation.
        // It tries to find the correct set of candidate values.
        if (i > 1) {
             std::set<int> all_known_p;
             for(int k=1; k<i; ++k) all_known_p.insert(p[k]);

             std::set<int> full_set;
             for(int k=1; k<=std::min(n, i+1+probe_indices.size()-3); ++k) full_set.insert(k);

             cand_values.clear();
             std::set_difference(full_set.begin(), full_set.end(),
                                all_known_p.begin(), all_known_p.end(),
                                std::back_inserter(cand_values));
        }


        for (size_t j = 0; j < sorted_indices.size(); ++j) {
            p[sorted_indices[j]] = cand_values[j];
        }

        rem_vals.clear();
        for (int j = 0; j < 3 && i + j <= n; ++j) {
            determined[i + j] = true;
        }

        for (int idx : sorted_indices) {
            if (!determined[idx]) {
                rem_vals.insert(p[idx]);
            }
        }
    }

    answer(n, p);

    return 0;
}