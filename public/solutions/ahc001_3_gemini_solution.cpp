#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

struct Company {
    int id;
    int x, y, r;
};

struct Rect {
    int x1, y1, x2, y2;
};

std::vector<Company> companies;
std::vector<int> p_indices;
std::vector<Rect> result_rects;

void solve(int begin, int end, int x1, int y1, int x2, int y2) {
    if (begin >= end) {
        return;
    }
    if (begin + 1 == end) {
        int company_idx = p_indices[begin];
        result_rects[company_idx] = {x1, y1, x2, y2};
        return;
    }

    int min_x = 10001, max_x = -1, min_y = 10001, max_y = -1;
    for (int i = begin; i < end; ++i) {
        int idx = p_indices[i];
        min_x = std::min(min_x, companies[idx].x);
        max_x = std::max(max_x, companies[idx].x);
        min_y = std::min(min_y, companies[idx].y);
        max_y = std::max(max_y, companies[idx].y);
    }
    bool can_split_vert = (min_x < max_x);
    bool can_split_horz = (min_y < max_y);

    long long total_r = 0;
    for(int i = begin; i < end; ++i) total_r += companies[p_indices[i]].r;

    int width = x2 - x1;
    int height = y2 - y1;
    
    int best_k_v = -1, best_x_split = -1;
    double min_badness_v = 1e18;
    
    if (can_split_vert) {
        std::sort(p_indices.begin() + begin, p_indices.begin() + end, [&](int a, int b){
            return companies[a].x < companies[b].x;
        });
        
        long long current_r_sum = 0;
        for (int k_offset = 1; k_offset < end - begin; ++k_offset) {
            int k_idx = begin + k_offset;
            current_r_sum += companies[p_indices[k_idx-1]].r;
            
            if (companies[p_indices[k_idx-1]].x < companies[p_indices[k_idx]].x) {
                long long R_left = current_r_sum;
                double x_split_ideal = x1 + (double)width * R_left / total_r;
                
                int x_bound_low = companies[p_indices[k_idx-1]].x + 1;
                int x_bound_high = companies[p_indices[k_idx]].x;
                
                int x_split = std::round(x_split_ideal);
                x_split = std::max(x_split, x_bound_low);
                x_split = std::min(x_split, x_bound_high);

                double badness = std::abs((double)(x_split - x1) / width - (double)R_left / total_r);
                if (badness < min_badness_v) {
                    min_badness_v = badness;
                    best_k_v = k_idx;
                    best_x_split = x_split;
                }
            }
        }
    }
    
    int best_k_h = -1, best_y_split = -1;
    double min_badness_h = 1e18;

    if (can_split_horz) {
        std::sort(p_indices.begin() + begin, p_indices.begin() + end, [&](int a, int b){
            return companies[a].y < companies[b].y;
        });
        
        long long current_r_sum = 0;
        for (int k_offset = 1; k_offset < end - begin; ++k_offset) {
            int k_idx = begin + k_offset;
            current_r_sum += companies[p_indices[k_idx-1]].r;
            
            if (companies[p_indices[k_idx-1]].y < companies[p_indices[k_idx]].y) {
                long long R_bottom = current_r_sum;
                double y_split_ideal = y1 + (double)height * R_bottom / total_r;
                
                int y_bound_low = companies[p_indices[k_idx-1]].y + 1;
                int y_bound_high = companies[p_indices[k_idx]].y;
                
                int y_split = std::round(y_split_ideal);
                y_split = std::max(y_split, y_bound_low);
                y_split = std::min(y_split, y_bound_high);

                double badness = std::abs((double)(y_split - y1) / height - (double)R_bottom / total_r);
                if (badness < min_badness_h) {
                    min_badness_h = badness;
                    best_k_h = k_idx;
                    best_y_split = y_split;
                }
            }
        }
    }
    
    bool split_vert;
    if (can_split_vert && !can_split_horz) {
        split_vert = true;
    } else if (!can_split_vert && can_split_horz) {
        split_vert = false;
    } else if (can_split_vert && can_split_horz) {
        split_vert = (min_badness_v <= min_badness_h);
    } else {
        return;
    }

    if (split_vert) {
        std::sort(p_indices.begin() + begin, p_indices.begin() + end, [&](int a, int b){
            return companies[a].x < companies[b].x;
        });
        solve(begin, best_k_v, x1, y1, best_x_split, y2);
        solve(best_k_v, end, best_x_split, y1, x2, y2);
    } else {
        std::sort(p_indices.begin() + begin, p_indices.begin() + end, [&](int a, int b){
            return companies[a].y < companies[b].y;
        });
        solve(begin, best_k_h, x1, y1, x2, best_y_split);
        solve(best_k_h, end, x1, best_y_split, x2, y2);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;
    companies.resize(n);
    result_rects.resize(n);
    p_indices.resize(n);
    for (int i = 0; i < n; ++i) {
        companies[i].id = i;
        std::cin >> companies[i].x >> companies[i].y >> companies[i].r;
        p_indices[i] = i;
    }

    solve(0, n, 0, 0, 10000, 10000);

    for (int i = 0; i < n; ++i) {
        std::cout << result_rects[i].x1 << " " << result_rects[i].y1 << " " << result_rects[i].x2 << " " << result_rects[i].y2 << "\n";
    }

    return 0;
}