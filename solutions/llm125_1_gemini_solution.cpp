#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

void query(int x) {
    std::cout << "? " << x << std::endl;
}

void answer(int a, int b) {
    std::cout << "! " << a << " " << b << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> unpaired_reps;
    std::vector<bool> is_paired(2 * n + 1, false);
    int current_distinct_kinds = 0;

    std::vector<int> p(2 * n);
    std::iota(p.begin(), p.end(), 1);
    
    // Process slices in a random order to improve average performance
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::shuffle(p.begin(), p.end(), rng);

    for (int i_val : p) {
        if (is_paired[i_val]) {
            continue;
        }

        query(i_val);
        int new_distinct_kinds;
        std::cin >> new_distinct_kinds;

        if (new_distinct_kinds > current_distinct_kinds) {
            unpaired_reps.push_back(i_val);
            current_distinct_kinds = new_distinct_kinds;
        } else {
            // Partner for i_val is in unpaired_reps.
            // Device contains {unpaired_reps, i_val}. Kinds = |unpaired_reps|.
            int s = unpaired_reps.size();
            
            std::vector<int> search_candidates = unpaired_reps;
            
            while (search_candidates.size() > 1) {
                int mid_idx = search_candidates.size() / 2;
                std::vector<int> first_half(search_candidates.begin(), search_candidates.begin() + mid_idx);
                
                // Remove first half from device
                for (int slice : first_half) {
                    query(slice);
                    std::cin >> new_distinct_kinds;
                }
                
                if (new_distinct_kinds == s - mid_idx + 1) {
                    // Partner was in the removed first half.
                    // Put the second half back.
                    for (size_t k = mid_idx; k < search_candidates.size(); ++k) {
                        query(search_candidates[k]);
                        std::cin >> new_distinct_kinds;
                    }
                    search_candidates.resize(mid_idx);
                } else {
                    // Partner was in the second half, which is still in the device.
                    s -= mid_idx;
                    search_candidates.erase(search_candidates.begin(), search_candidates.begin() + mid_idx);
                }
            }
            
            int partner = search_candidates[0];
            answer(i_val, partner);
            is_paired[i_val] = true;
            is_paired[partner] = true;

            // At this point, the device contains {partner, i_val} and possibly other elements.
            // All other old representatives are outside the device.
            // We need to restore state.
            
            // Remove i_val and partner from device.
            query(i_val); std::cin >> new_distinct_kinds;
            query(partner); std::cin >> new_distinct_kinds;
            
            // Rebuild the stack of representatives.
            unpaired_reps.erase(std::remove(unpaired_reps.begin(), unpaired_reps.end(), partner), unpaired_reps.end());
            
            // All elements in the updated unpaired_reps are currently outside the device. Put them back.
            for (int rep : unpaired_reps) {
                query(rep);
                std::cin >> new_distinct_kinds;
            }
            current_distinct_kinds = unpaired_reps.size();
        }
    }

    return 0;
}