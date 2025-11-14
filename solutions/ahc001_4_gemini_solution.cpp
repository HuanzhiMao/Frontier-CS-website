#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <tuple>

struct Company {
    int id;
    int x, y, r;
};

int N;
std::vector<Company> companies;
std::vector<std::tuple<int, int, int, int>> result;

void solve(std::vector<int>& indices, int x1, int y1, int x2, int y2) {
    if (indices.empty()) {
        return;
    }

    if (indices.size() == 1) {
        int company_id = companies[indices[0]].id;
        result[company_id] = {x1, y1, x2, y2};
        return;
    }

    int min_x = 10001, max_x = -1, min_y = 10001, max_y = -1;
    for (int idx : indices) {
        min_x = std::min(min_x, companies[idx].x);
        max_x = std::max(max_x, companies[idx].x);
        min_y = std::min(min_y, companies[idx].y);
        max_y = std::max(max_y, companies[idx].y);
    }

    bool can_v_split = (min_x < max_x);
    bool can_h_split = (min_y < max_y);
    
    bool vertical_cut = false;
    if (can_v_split && (!can_h_split || (x2 - x1) >= (y2 - y1))) {
        vertical_cut = true;
    } else if (can_h_split) {
        vertical_cut = false;
    } else { 
        vertical_cut = ((x2 - x1) >= (y2 - y1));
    }


    if (vertical_cut) {
        std::sort(indices.begin(), indices.end(), [&](int i, int j) {
            if (companies[i].x != companies[j].x) return companies[i].x < companies[j].x;
            return companies[i].y < companies[j].y;
        });

        long long total_r = 0;
        for (int idx : indices) {
            total_r += companies[idx].r;
        }

        int best_m = -1;
        double min_abs_diff = 1e18;

        long long current_r_sum = 0;
        for (size_t m = 1; m < indices.size(); ++m) {
            current_r_sum += companies[indices[m-1]].r;
            if (companies[indices[m-1]].x < companies[indices[m]].x) {
                double ideal_cut = x1 + (double)(x2 - x1) * current_r_sum / total_r;
                
                int min_cut_coord = companies[indices[m-1]].x + 1;
                int max_cut_coord = companies[indices[m]].x;

                int cut = round(ideal_cut);
                cut = std::max(min_cut_coord, std::min(max_cut_coord, cut));
                
                double diff = std::abs(cut - ideal_cut);
                if (diff < min_abs_diff) {
                    min_abs_diff = diff;
                    best_m = m;
                }
            }
        }
        
        long long left_r = 0;
        for (int i = 0; i < best_m; ++i) {
            left_r += companies[indices[i]].r;
        }
        
        double ideal_cut_x = x1 + (double)(x2 - x1) * left_r / total_r;
        int min_cut_coord = companies[indices[best_m-1]].x + 1;
        int max_cut_coord = companies[indices[best_m]].x;
        
        int cut_x = round(ideal_cut_x);
        cut_x = std::max(min_cut_coord, std::min(max_cut_coord, cut_x));
        
        std::vector<int> left_indices(indices.begin(), indices.begin() + best_m);
        std::vector<int> right_indices(indices.begin() + best_m, indices.end());

        solve(left_indices, x1, y1, cut_x, y2);
        solve(right_indices, cut_x, y1, x2, y2);

    } else { // Horizontal cut
        std::sort(indices.begin(), indices.end(), [&](int i, int j) {
            if (companies[i].y != companies[j].y) return companies[i].y < companies[j].y;
            return companies[i].x < companies[j].x;
        });

        long long total_r = 0;
        for (int idx : indices) {
            total_r += companies[idx].r;
        }

        int best_m = -1;
        double min_abs_diff = 1e18;

        long long current_r_sum = 0;
        for (size_t m = 1; m < indices.size(); ++m) {
            current_r_sum += companies[indices[m-1]].r;
            if (companies[indices[m-1]].y < companies[indices[m]].y) {
                double ideal_cut = y1 + (double)(y2 - y1) * current_r_sum / total_r;
                
                int min_cut_coord = companies[indices[m-1]].y + 1;
                int max_cut_coord = companies[indices[m]].y;

                int cut = round(ideal_cut);
                cut = std::max(min_cut_coord, std::min(max_cut_coord, cut));
                
                double diff = std::abs(cut - ideal_cut);
                if (diff < min_abs_diff) {
                    min_abs_diff = diff;
                    best_m = m;
                }
            }
        }
        
        long long bottom_r = 0;
        for (int i = 0; i < best_m; ++i) {
            bottom_r += companies[indices[i]].r;
        }

        double ideal_cut_y = y1 + (double)(y2 - y1) * bottom_r / total_r;
        int min_cut_coord = companies[indices[best_m-1]].y + 1;
        int max_cut_coord = companies[indices[best_m]].y;
        
        int cut_y = round(ideal_cut_y);
        cut_y = std::max(min_cut_coord, std::min(max_cut_coord, cut_y));
        
        std::vector<int> bottom_indices(indices.begin(), indices.begin() + best_m);
        std::vector<int> top_indices(indices.begin() + best_m, indices.end());

        solve(bottom_indices, x1, y1, x2, cut_y);
        solve(top_indices, x1, cut_y, x2, y2);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N;
    companies.resize(N);
    result.resize(N);

    for (int i = 0; i < N; ++i) {
        companies[i].id = i;
        std::cin >> companies[i].x >> companies[i].y >> companies[i].r;
    }

    std::vector<int> initial_indices(N);
    std::iota(initial_indices.begin(), initial_indices.end(), 0);

    solve(initial_indices, 0, 0, 10000, 10000);

    for (int i = 0; i < N; ++i) {
        auto [a, b, c, d] = result[i];
        std::cout << a << " " << b << " " << c << " " << d << "\n";
    }

    return 0;
}