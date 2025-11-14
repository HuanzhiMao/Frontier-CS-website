#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <random>

int n;
int p1_val, p2_val, p3_val;
int idx1, idx2, idx3;

// Function to perform a query
std::pair<int, int> ask_query(const std::vector<int>& indices) {
    std::cout << "0 " << indices.size();
    for (int idx : indices) {
        std::cout << " " << idx;
    }
    std::cout << std::endl;
    int m1, m2;
    std::cin >> m1 >> m2;
    return {m1, m2};
}

// Comparison function for elements with values smaller than p2_val
bool is_smaller_for_small_set(int c1, int c2) {
    auto medians = ask_query({idx2, idx3, c1, c2});
    int max_val = (medians.first == p2_val) ? medians.second : medians.first;
    
    auto medians_c1 = ask_query({idx2, idx3, c1, (c1 == 1 ? 2 : 1) });
    int val_c1 = (medians_c1.first == p2_val) ? medians_c1.second : medians_c1.first;

    return max_val != val_c1;
}

// Comparison function for elements with values larger than p2_val
bool is_smaller_for_large_set(int c1, int c2) {
    auto medians = ask_query({idx1, idx2, c1, c2});
    int min_val = (medians.first == p2_val) ? medians.second : medians.first;
    
    auto medians_c1 = ask_query({idx1, idx2, c1, (c1 == n ? n - 1 : n) });
    int val_c1 = (medians_c1.first == p2_val) ? medians_c1.second : medians_c1.first;
    
    return min_val == val_c1;
}

// Find k-th smallest element in a set
int find_kth(std::vector<int>& S, int k, bool is_large_set) {
    if (S.size() == 1) return S[0];
    
    std::mt19937 rng(1337);
    std::shuffle(S.begin(), S.end(), rng);
    int pivot_idx = S.back();
    S.pop_back();

    std::vector<int> smaller, larger;
    for (int idx : S) {
        bool res;
        if (is_large_set) {
            res = is_smaller_for_large_set(idx, pivot_idx);
        } else {
            res = is_smaller_for_small_set(idx, pivot_idx);
        }
        if (res) {
            smaller.push_back(idx);
        } else {
            larger.push_back(idx);
        }
    }

    if (smaller.size() == k - 1) {
        return pivot_idx;
    } else if (smaller.size() >= k) {
        return find_kth(smaller, k, is_large_set);
    } else {
        return find_kth(larger, k - 1 - smaller.size(), is_large_set);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    // Phase 1: Initial Partitioning
    std::vector<int> p_indices = {1, 2, 3};
    std::vector<int> other_indices;
    for (int i = 4; i <= n; ++i) {
        other_indices.push_back(i);
    }

    std::map<int, int> median_counts;
    std::vector<std::pair<int, int>> query_results;
    for (int i : other_indices) {
        auto medians = ask_query({p_indices[0], p_indices[1], p_indices[2], i});
        median_counts[medians.first]++;
        median_counts[medians.second]++;
        query_results.push_back(medians);
    }

    for (auto const& [val, count] : median_counts) {
        if (count == n - 3) {
            p2_val = val;
        }
    }

    std::vector<int> p1p3_candidates;
    for (auto const& [val, count] : median_counts) {
        if (val != p2_val) {
            p1p3_candidates.push_back(val);
        }
    }
    
    if (p1p3_candidates.size() < 2) {
        // This can happen if all other elements are between p1 and p3
        auto medians = ask_query({1,2,4,5});
        p1_val = std::min(p1p3_candidates.empty() ? medians.first : p1p3_candidates[0], medians.second);
        p3_val = std::max(p1p3_candidates.empty() ? medians.first : p1p3_candidates[0], medians.second);
    } else {
         p1_val = std::min(p1p3_candidates[0], p1p3_candidates[1]);
         p3_val = std::max(p1p3_candidates[0], p1p3_candidates[1]);
    }
    
    auto med_124 = ask_query({1,2,4});
    if (med_124.first != p2_val && med_124.second != p2_val) { // p[3] is p2_val
        idx2 = 3;
        auto med_12 = ask_query({1,2,4,5});
        if (med_12.first > med_12.second) std::swap(med_12.first, med_12.second);
        if (med_12.first == p1_val || med_12.second == p1_val) { idx1 = 1; idx3 = 2; }
        else { idx1 = 2; idx3 = 1; }
    }
    auto med_134 = ask_query({1,3,4});
    if (med_134.first != p2_val && med_134.second != p2_val) { // p[2] is p2_val
        idx2 = 2;
        auto med_13 = ask_query({1,3,4,5});
        if (med_13.first > med_13.second) std::swap(med_13.first, med_13.second);
        if (med_13.first == p1_val || med_13.second == p1_val) { idx1 = 1; idx3 = 3; }
        else { idx1 = 3; idx3 = 1; }
    }
    auto med_234 = ask_query({2,3,4});
    if (med_234.first != p2_val && med_234.second != p2_val) { // p[1] is p2_val
        idx2 = 1;
        auto med_23 = ask_query({2,3,4,5});
        if (med_23.first > med_23.second) std::swap(med_23.first, med_23.second);
        if (med_23.first == p1_val || med_23.second == p1_val) { idx1 = 2; idx3 = 3; }
        else { idx1 = 3; idx3 = 2; }
    }

    std::vector<int> I_small, I_between, I_large;
    for (size_t i = 0; i < other_indices.size(); ++i) {
        int idx = other_indices[i];
        auto medians = query_results[i];
        if (medians.first != p2_val && medians.second != p2_val) {
            if (medians.first < p2_val) I_small.push_back(idx);
            else I_large.push_back(idx);
        } else {
            I_between.push_back(idx);
        }
    }
    
    // Phase 2: Refine Partition
    std::vector<int> I_between_small, I_between_large;
    int anchor = -1;
    bool use_small_anchor = false;
    if (!I_small.empty()) {
        anchor = I_small[0];
        use_small_anchor = true;
    } else if (!I_large.empty()) {
        anchor = I_large[0];
        use_small_anchor = false;
    } else { // both empty
        if (!I_between.empty()) anchor = I_between.back();
    }

    if (anchor != -1) {
        for (int i : I_between) {
            if (i == anchor && (I_small.empty() && I_large.empty())) continue;
            if (use_small_anchor) {
                auto medians = ask_query({idx1, idx2, i, anchor});
                if (medians.first == p1_val && medians.second == p2_val) {
                    I_between_large.push_back(i);
                } else {
                    I_between_small.push_back(i);
                }
            } else { // use large anchor or from I_between
                auto medians = ask_query({idx2, idx3, i, anchor});
                if (medians.first == p2_val && medians.second == p3_val) {
                    I_between_small.push_back(i);
                } else {
                    I_between_large.push_back(i);
                }
            }
        }
        if (I_small.empty() && I_large.empty()) { // Classify anchor itself
            auto medians = ask_query({idx2, idx3, anchor, I_between[0]});
             if (medians.first == p2_val && medians.second == p3_val) {
                I_between_small.push_back(anchor);
            } else {
                I_between_large.push_back(anchor);
            }
        }
    }


    // Phase 3: Find Medians
    int rank = I_small.size() + I_between_small.size() + 2;
    int ans1 = -1, ans2 = -1;

    if (rank == n / 2) {
        ans1 = idx2;
        std::vector<int> S_large_all = I_between_large;
        S_large_all.push_back(idx3);
        S_large_all.insert(S_large_all.end(), I_large.begin(), I_large.end());
        ans2 = find_kth(S_large_all, 1, true);
    } else if (rank == n / 2 + 1) {
        ans1 = idx2;
        std::vector<int> S_small_all = I_between_small;
        S_small_all.push_back(idx1);
        S_small_all.insert(S_small_all.end(), I_small.begin(), I_small.end());
        ans2 = find_kth(S_small_all, S_small_all.size(), false);
    } else if (rank < n / 2) {
        std::vector<int> S_large_all = I_between_large;
        S_large_all.push_back(idx3);
        S_large_all.insert(S_large_all.end(), I_large.begin(), I_large.end());
        int k1 = n / 2 - rank + 1;
        int k2 = n / 2 + 1 - rank + 1;
        ans1 = find_kth(S_large_all, k1, true);
        
        S_large_all = I_between_large; // Refill S_large_all
        S_large_all.push_back(idx3);
        S_large_all.insert(S_large_all.end(), I_large.begin(), I_large.end());
        ans2 = find_kth(S_large_all, k2, true);
    } else { // rank > n / 2 + 1
        std::vector<int> S_small_all = I_between_small;
        S_small_all.push_back(idx1);
        S_small_all.insert(S_small_all.end(), I_small.begin(), I_small.end());
        int k1 = n / 2;
        int k2 = n / 2 + 1;
        ans1 = find_kth(S_small_all, k1, false);
        
        S_small_all = I_between_small; // Refill
        S_small_all.push_back(idx1);
        S_small_all.insert(S_small_all.end(), I_small.begin(), I_small.end());
        ans2 = find_kth(S_small_all, k2, false);
    }

    if (ans1 > ans2) std::swap(ans1, ans2);
    std::cout << "1 " << ans1 << " " << ans2 << std::endl;

    return 0;
}