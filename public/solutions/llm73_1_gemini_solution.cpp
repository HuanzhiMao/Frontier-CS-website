#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

std::vector<std::vector<int>> query_cache;
std::vector<std::vector<bool>> has_queried;

int do_query(int l, int r) {
    if (l > r) return 0;
    if (has_queried[l][r]) {
        return query_cache[l][r];
    }
    std::cout << "0 " << l << " " << r << std::endl;
    int response;
    std::cin >> response;
    has_queried[l][r] = true;
    query_cache[l][r] = response;
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    query_cache.assign(n + 2, std::vector<int>(n + 2, 0));
    has_queried.assign(n + 2, std::vector<bool>(n + 2, false));

    std::vector<int> p(n + 1, 0);
    std::vector<bool> used_values(n + 1, false);

    for (int k = 1; k <= n; ++k) {
        std::vector<std::pair<int, int>> sorted_prefix;
        for (int i = 1; i < k; ++i) {
            sorted_prefix.push_back({p[i], i});
        }
        std::sort(sorted_prefix.begin(), sorted_prefix.end());
        
        int rank = 0; 

        if (k > 1) {
            int l = 0, r = k - 2;
            int final_pos = k - 1;
            while(l <= r){
                int mid = l + (r - l) / 2;
                int j = sorted_prefix[mid].second;
                
                int inv_j_k = do_query(j, k);
                int inv_j_k_minus_1 = do_query(j, k - 1);
                int inv_j1_k = do_query(j + 1, k);
                int inv_j1_k_minus_1 = do_query(j + 1, k - 1);

                int sum_j_mod2 = (inv_j_k - inv_j_k_minus_1 + 2) % 2;
                int sum_j1_mod2 = (inv_j1_k - inv_j1_k_minus_1 + 2) % 2;
                
                int is_pj_greater = (sum_j_mod2 - sum_j1_mod2 + 2) % 2;
                
                if (is_pj_greater) { // p[k] < p[j]
                    final_pos = mid;
                    r = mid - 1;
                } else { // p[k] > p[j]
                    l = mid + 1;
                }
            }
            rank = l;
        }

        int val = 0;
        int current_rank = 0;
        for (int v = 1; v <= n; ++v) {
            if (!used_values[v]) {
                if (current_rank == rank) {
                    val = v;
                    break;
                }
                current_rank++;
            }
        }
        p[k] = val;
        used_values[p[k]] = true;
    }

    std::cout << "1";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;

    return 0;
}