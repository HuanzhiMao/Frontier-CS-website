#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int query(int x) {
    std::cout << "? " << x << std::endl;
    int r;
    std::cin >> r;
    return r;
}

void answer(int a, int b) {
    std::cout << "! " << a << " " << b << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> s;
    std::vector<bool> paired(2 * n + 1, false);
    int current_distinct_kinds = 0;

    for (int i = 1; i <= 2 * n; ++i) {
        if (paired[i]) {
            continue;
        }

        int next_distinct_kinds = query(i);
        if (next_distinct_kinds > current_distinct_kinds) {
            s.push_back(i);
            current_distinct_kinds = next_distinct_kinds;
        } else {
            // Pair of i is in s. Find it.
            // Device now contains s U {i}.
            // The number of distinct kinds is s.size(), which is the same as current_distinct_kinds
            
            int p = -1;
            int p_idx = -1;
            for (int j = 0; j < s.size(); ++j) {
                int candidate_p = s[j];
                // Temporarily remove candidate_p to check if kind count changes
                int kinds_after_removal = query(candidate_p);
                if (kinds_after_removal == current_distinct_kinds) {
                    // Count did not change, so kind of candidate_p is still present.
                    // This means kind(candidate_p) == kind(i). Found the pair.
                    p = candidate_p;
                    p_idx = j;
                    break;
                } else {
                    // Not the pair. Put it back.
                    query(candidate_p);
                }
            }
            
            answer(i, p);
            paired[i] = true;
            paired[p] = true;

            // p is already out of device from the successful check.
            // i is still in. Remove it.
            current_distinct_kinds = query(i);
            
            s.erase(s.begin() + p_idx);
        }
    }

    return 0;
}