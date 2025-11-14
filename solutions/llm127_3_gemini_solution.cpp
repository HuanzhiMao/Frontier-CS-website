#include <iostream>
#include <vector>
#include <utility>
#include <numeric>
#include <algorithm>
#include <random>
#include <map>
#include <chrono>

using namespace std;

int n;
map<int, pair<int, int>> query_cache;
int s_count = 0;
vector<int> found_special_indices;

pair<int, int> do_query(int i) {
    if (query_cache.count(i)) {
        return query_cache[i];
    }
    cout << "? " << i << endl;
    int a0, a1;
    cin >> a0 >> a1;
    query_cache[i] = {a0, a1};
    if (s_count > 0 && a0 + a1 < s_count) {
        bool found = false;
        for (int idx : found_special_indices) if (idx == i) found = true;
        if (!found) found_special_indices.push_back(i);
    }
    return {a0, a1};
}

map<int, int> prefix_special_counts;

int get_prefix_count(int k) {
    if (k < 0) return 0;
    if (prefix_special_counts.count(k)) {
        return prefix_special_counts[k];
    }

    // Search outwards from k+1 to find a common prize
    for (int d = 0; ; ++d) {
        int up_idx = k + 1 + d;
        if (up_idx < n) {
            pair<int, int> res = do_query(up_idx);
            if (res.first + res.second == s_count) { // Common prize found
                int count_before_up = res.first;
                int specials_in_between = 0;
                for (int i = k + 1; i < up_idx; ++i) {
                    pair<int, int> r = do_query(i);
                    if (r.first + r.second < s_count) {
                        specials_in_between++;
                    }
                }
                int result = count_before_up - specials_in_between;
                prefix_special_counts[k] = result;
                return result;
            }
        }

        if (d > 0) {
            int down_idx = k + 1 - d;
            if (down_idx >= 0) {
                 pair<int, int> res = do_query(down_idx);
                 if (res.first + res.second == s_count) { // Common prize found
                    int count_after_down = res.second;
                    int special_count_total = s_count;
                    int special_in_between = 0;
                     for (int i = down_idx + 1; i <= k; ++i) {
                         pair<int, int> r = do_query(i);
                         if(r.first + r.second < s_count) {
                             special_in_between++;
                         }
                     }
                    int result = special_count_total - count_after_down - special_in_between;
                    prefix_special_counts[k] = result;
                    return result;
                }
            }
        }
        if (up_idx >= n && k + 1 -d < 0) break; // Should not happen
    }
    return 0; // Should be unreachable
}


void find_special_in_range(int l, int r) {
    if (l > r || found_special_indices.size() == s_count) return;

    int count_before_l = get_prefix_count(l - 1);
    int count_before_r = get_prefix_count(r);
    int k = count_before_r - count_before_l;

    if (k == 0) return;

    if (r - l + 1 == k) {
        for (int i = l; i <= r; ++i) {
             do_query(i); // This will add to found_special_indices
        }
        return;
    }

    int m = l + (r - l) / 2;
    find_special_in_range(l, m);
    find_special_in_range(m + 1, r);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    // Phase 1: Find s_count
    int num_initial_queries = min(n, 60);
    for (int k = 0; k < num_initial_queries; ++k) {
        int i = uniform_int_distribution<int>(0, n - 1)(rng);
        if (query_cache.count(i)) {
            k--; 
            continue;
        }
        pair<int, int> res = do_query(i);
        s_count = max(s_count, res.first + res.second);
    }
    
    // Repopulate found_special_indices with correct s_count
    vector<int> temp_special;
    for(auto const& [idx, res] : query_cache) {
        if (res.first + res.second < s_count) {
            temp_special.push_back(idx);
        }
    }
    found_special_indices = temp_special;

    if (s_count == 0) {
        int diamond_idx = 0;
         for(auto const& [idx, res] : query_cache) {
            if (res.first + res.second == 0) {
                diamond_idx = idx;
                break;
            }
        }
        cout << "! " << diamond_idx << endl;
        return 0;
    }

    // Phase 2: Find all special items
    find_special_in_range(0, n - 1);

    // Phase 3: Find the diamond
    int diamond_idx = -1;
    int min_sum = n + 1;

    sort(found_special_indices.begin(), found_special_indices.end());
    found_special_indices.erase(unique(found_special_indices.begin(), found_special_indices.end()), found_special_indices.end());

    for (int idx : found_special_indices) {
        pair<int, int> res = do_query(idx);
        if (res.first + res.second == 0) {
            diamond_idx = idx;
            break;
        }
        if (res.first + res.second < min_sum) {
            min_sum = res.first + res.second;
            diamond_idx = idx;
        }
    }
    
    if (diamond_idx == -1) {
        if (!found_special_indices.empty()) {
           diamond_idx = found_special_indices[0];
        } else {
           // This case should be rare. If no special items found, but s_count > 0
           // there must be at least one. The search might have failed.
           // As a last resort, check all boxes. Or guess 0.
           diamond_idx = 0;
        }
    }

    cout << "! " << diamond_idx << endl;

    return 0;
}