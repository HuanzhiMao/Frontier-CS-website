#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Use long long for costs and sums to avoid overflow
using ll = long long;
const ll INF = 1e18;

// Fenwick Tree for Range Maximum Query
template <typename T>
struct FenwickTree {
    int n;
    std::vector<T> tree;

    FenwickTree(int size) : n(size), tree(size + 1, -INF) {}

    void update(int idx, T val) {
        for (++idx; idx <= n; idx += idx & -idx) {
            tree[idx] = std::max(tree[idx], val);
        }
    }

    T query(int idx) {
        T s = -INF;
        for (++idx; idx > 0; idx -= idx & -idx) {
            s = std::max(s, tree[idx]);
        }
        return s;
    }
};

// Standard O(n log n) algorithm to find the length of the LIS
int get_lis_length(int n, const std::vector<int>& v) {
    if (n == 0) return 0;
    std::vector<int> tail;
    tail.push_back(v[0]);
    for (int i = 1; i < n; i++) {
        if (v[i] > tail.back()) {
            tail.push_back(v[i]);
        } else {
            *std::lower_bound(tail.begin(), tail.end(), v[i]) = v[i];
        }
    }
    return tail.size();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }

    int lis_len = get_lis_length(n, v);

    std::vector<FenwickTree<ll>> fts;
    for (int i = 0; i <= lis_len; ++i) {
        fts.emplace_back(n + 1);
    }

    std::vector<std::vector<ll>> max_sum_is(n, std::vector<ll>(lis_len + 1, -INF));

    for (int i = 0; i < n; ++i) {
        int val = v[i];
        
        max_sum_is[i][1] = val;
        fts[1].update(val, val);

        for (int l = 2; l <= lis_len; ++l) {
            ll prev_sum = fts[l - 1].query(val - 1);
            if (prev_sum > -INF / 2) {
                max_sum_is[i][l] = val + prev_sum;
                fts[l].update(val, max_sum_is[i][l]);
            }
        }
    }

    std::vector<ll> max_s(lis_len + 1, 0);
    for (int l = 1; l <= lis_len; ++l) {
        max_s[l] = fts[l].query(n);
        if (max_s[l] < -INF / 2) max_s[l] = 0;
    }

    ll min_final_cost = -1;
    int best_l = 0;

    for (int l = 0; l <= lis_len; ++l) {
        if (l > 0 && max_s[l] == 0) continue;
        
        ll k = n - l;
        ll total_sum_1_to_n = (ll)n * (n + 1) / 2;
        ll sum_m = total_sum_1_to_n - max_s[l];
        ll sum_m_minus_i = sum_m - k * (k + 1) / 2;
        
        ll total_cost = k + sum_m_minus_i;
        ll final_cost = (total_cost + 1) * (k + 1);

        if (min_final_cost == -1 || final_cost < min_final_cost) {
            min_final_cost = final_cost;
            best_l = l;
        }
    }
    
    if (n > 0 && best_l == 0) {
        ll k = n;
        ll total_sum_1_to_n = (ll)n * (n + 1) / 2;
        ll sum_m = total_sum_1_to_n;
        ll sum_m_minus_i = sum_m - k * (k + 1) / 2;
        ll total_cost = k + sum_m_minus_i;
        ll current_final_cost = (total_cost + 1) * (k + 1);
        if (min_final_cost == -1 || current_final_cost < min_final_cost) {
             min_final_cost = current_final_cost;
        }
    }

    std::cout << min_final_cost << " " << n - best_l << std::endl;

    if (n - best_l > 0) {
        std::vector<bool> to_keep(n + 1, false);
        if (best_l > 0) {
            ll current_sum = max_s[best_l];
            int current_val = -1;
            int last_pos = n;
            
            for (int i = n - 1; i >= 0; --i) {
                if(max_sum_is[i][best_l] == current_sum) {
                    current_val = v[i];
                    last_pos = i;
                    break;
                }
            }

            for (int l = best_l; l >= 1; --l) {
                to_keep[current_val] = true;
                if (l > 1) {
                    ll prev_sum = current_sum - current_val;
                    int next_val = -1;
                    for (int i = last_pos - 1; i >= 0; --i) {
                        if (v[i] < current_val && max_sum_is[i][l-1] == prev_sum) {
                             next_val = v[i];
                             last_pos = i;
                             break;
                        }
                    }
                    current_sum = prev_sum;
                    current_val = next_val;
                }
            }
        }

        std::vector<int> to_move;
        for (int i = 1; i <= n; ++i) {
            if (!to_keep[i]) {
                to_move.push_back(i);
            }
        }
        std::sort(to_move.rbegin(), to_move.rend());

        std::vector<int> p = v;

        for (int val : to_move) {
            int current_pos = -1;
            for (int i = 0; i < p.size(); ++i) {
                if (p[i] == val) {
                    current_pos = i + 1;
                    break;
                }
            }

            int dest_pos = 1;
            for (int i = 1; i <= n; ++i) {
                if (to_keep[i] && i < val) {
                    dest_pos++;
                }
            }
            
            std::cout << current_pos << " " << dest_pos << std::endl;

            int element_val = p[current_pos - 1];
            p.erase(p.begin() + current_pos - 1);
            p.insert(p.begin() + dest_pos - 1, element_val);
        }
    }

    return 0;
}