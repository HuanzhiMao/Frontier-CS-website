#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

struct Company {
    int id;
    int x, y, r;
};

std::vector<Company> companies_data;
int final_a[201], final_b[201], final_c[201], final_d[201];

void solve(int* p_indices_begin, int* p_indices_end, int a, int b, int c, int d) {
    int n_companies = p_indices_end - p_indices_begin;

    if (n_companies == 0) {
        return;
    }
    if (n_companies == 1) {
        int company_idx = *p_indices_begin;
        final_a[companies_data[company_idx].id] = a;
        final_b[companies_data[company_idx].id] = b;
        final_c[companies_data[company_idx].id] = c;
        final_d[companies_data[company_idx].id] = d;
        return;
    }

    long long total_r = 0;
    int min_x = 10001, max_x = -1, min_y = 10001, max_y = -1;
    for (int* it = p_indices_begin; it != p_indices_end; ++it) {
        int idx = *it;
        total_r += companies_data[idx].r;
        min_x = std::min(min_x, companies_data[idx].x);
        max_x = std::max(max_x, companies_data[idx].x);
        min_y = std::min(min_y, companies_data[idx].y);
        max_y = std::max(max_y, companies_data[idx].y);
    }
    
    bool can_split_v = (min_x < max_x);
    bool can_split_h = (min_y < max_y);
    
    bool split_v;
    if ((c - a) >= (d - b)) {
        if (can_split_v) split_v = true;
        else split_v = false;
    } else {
        if (can_split_h) split_v = false;
        else split_v = true;
    }

    if (split_v) {
        std::sort(p_indices_begin, p_indices_end, [&](int i, int j) {
            return companies_data[i].x < companies_data[j].x;
        });

        int best_k = -1;
        int best_cut = -1;
        long double min_cost = 1e18;

        long long current_r_l = 0;
        for (int k = 1; k < n_companies; ++k) {
            current_r_l += companies_data[p_indices_begin[k-1]].r;
            if (companies_data[p_indices_begin[k-1]].x < companies_data[p_indices_begin[k]].x) {
                long long min_cut = companies_data[p_indices_begin[k-1]].x + 1;
                long long max_cut = companies_data[p_indices_begin[k]].x;
                
                long double ideal_cut_double = (long double)a + (long double)(c - a) * current_r_l / total_r;
                
                long long cut = roundl(ideal_cut_double);
                cut = std::max(cut, min_cut);
                cut = std::min(cut, max_cut);

                long double cost = fabsl(ideal_cut_double - cut);
                
                if (cost < min_cost) {
                    min_cost = cost;
                    best_k = k;
                    best_cut = cut;
                }
            }
        }
        
        solve(p_indices_begin, p_indices_begin + best_k, a, b, best_cut, d);
        solve(p_indices_begin + best_k, p_indices_end, best_cut, b, c, d);

    } else { // split horizontally
        std::sort(p_indices_begin, p_indices_end, [&](int i, int j) {
            return companies_data[i].y < companies_data[j].y;
        });

        int best_k = -1;
        int best_cut = -1;
        long double min_cost = 1e18;

        long long current_r_b = 0;
        for (int k = 1; k < n_companies; ++k) {
            current_r_b += companies_data[p_indices_begin[k-1]].r;
            if (companies_data[p_indices_begin[k-1]].y < companies_data[p_indices_begin[k]].y) {
                long long min_cut = companies_data[p_indices_begin[k-1]].y + 1;
                long long max_cut = companies_data[p_indices_begin[k]].y;

                long double ideal_cut_double = (long double)b + (long double)(d - b) * current_r_b / total_r;

                long long cut = roundl(ideal_cut_double);
                cut = std::max(cut, min_cut);
                cut = std::min(cut, max_cut);

                long double cost = fabsl(ideal_cut_double - cut);
                
                if (cost < min_cost) {
                    min_cost = cost;
                    best_k = k;
                    best_cut = cut;
                }
            }
        }
        
        solve(p_indices_begin, p_indices_begin + best_k, a, b, c, best_cut);
        solve(p_indices_begin + best_k, p_indices_end, a, best_cut, c, d);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;
    companies_data.resize(n);
    for (int i = 0; i < n; ++i) {
        companies_data[i].id = i;
        std::cin >> companies_data[i].x >> companies_data[i].y >> companies_data[i].r;
    }
    
    std::vector<int> p_indices(n);
    std::iota(p_indices.begin(), p_indices.end(), 0);
    
    solve(p_indices.data(), p_indices.data() + n, 0, 0, 10000, 10000);
    
    for (int i = 0; i < n; ++i) {
        std::cout << final_a[i] << " " << final_b[i] << " " << final_c[i] << " " << final_d[i] << "\n";
    }
    
    return 0;
}