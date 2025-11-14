#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

struct Company {
    int x, y;
    long long r;
};

struct Rectangle {
    int x1, y1, x2, y2;
};

std::vector<Company> companies;
std::vector<Rectangle> result_rects;
std::vector<int> p_indices;

void solve(int x1, int y1, int x2, int y2, int begin_idx, int end_idx) {
    if (begin_idx >= end_idx) {
        return;
    }

    if (end_idx - begin_idx == 1) {
        result_rects[p_indices[begin_idx]] = {x1, y1, x2, y2};
        return;
    }

    bool try_vertical = (x2 - x1) >= (y2 - y1);

    auto attempt_split = [&](bool vertical) -> bool {
        if (vertical) {
            std::sort(p_indices.begin() + begin_idx, p_indices.begin() + end_idx, [&](int a, int b) {
                if (companies[a].x != companies[b].x) return companies[a].x < companies[b].x;
                return companies[a].y < companies[b].y;
            });
        } else {
            std::sort(p_indices.begin() + begin_idx, p_indices.begin() + end_idx, [&](int a, int b) {
                if (companies[a].y != companies[b].y) return companies[a].y < companies[b].y;
                return companies[a].x < companies[b].x;
            });
        }

        long long total_r = 0;
        for (int i = begin_idx; i < end_idx; ++i) {
            total_r += companies[p_indices[i]].r;
        }

        long long current_r = 0;
        int best_k = -1;
        long long min_diff = -1;
        int num_pts = end_idx - begin_idx;

        for (int k = 1; k < num_pts; ++k) {
            current_r += companies[p_indices[begin_idx + k - 1]].r;
            
            bool is_valid_split;
            if (vertical) {
                is_valid_split = companies[p_indices[begin_idx + k - 1]].x < companies[p_indices[begin_idx + k]].x;
            } else {
                is_valid_split = companies[p_indices[begin_idx + k - 1]].y < companies[p_indices[begin_idx + k]].y;
            }
            if (!is_valid_split) continue;

            long long diff = std::abs((total_r - current_r) - current_r);
            if (best_k == -1 || diff < min_diff) {
                min_diff = diff;
                best_k = k;
            }
        }
        
        if (best_k == -1) {
            return false;
        }
        
        long long r1 = 0;
        for (int i = 0; i < best_k; ++i) {
            r1 += companies[p_indices[begin_idx + i]].r;
        }

        int split_idx = begin_idx + best_k;
        
        if (vertical) {
            double split_ideal = static_cast<double>(x1) + static_cast<double>(x2 - x1) * r1 / total_r;
            int split_coord = round(split_ideal);
            
            int p1_max_x = companies[p_indices[split_idx - 1]].x;
            int p2_min_x = companies[p_indices[split_idx]].x;

            split_coord = std::max(split_coord, p1_max_x + 1);
            split_coord = std::min(split_coord, p2_min_x);
            
            solve(x1, y1, split_coord, y2, begin_idx, split_idx);
            solve(split_coord, y1, x2, y2, split_idx, end_idx);
        } else {
            double split_ideal = static_cast<double>(y1) + static_cast<double>(y2 - y1) * r1 / total_r;
            int split_coord = round(split_ideal);
            
            int p1_max_y = companies[p_indices[split_idx - 1]].y;
            int p2_min_y = companies[p_indices[split_idx]].y;

            split_coord = std::max(split_coord, p1_max_y + 1);
            split_coord = std::min(split_coord, p2_min_y);

            solve(x1, y1, x2, split_coord, begin_idx, split_idx);
            solve(x1, split_coord, x2, y2, split_idx, end_idx);
        }
        return true;
    };

    if (try_vertical) {
        if (!attempt_split(true)) {
            attempt_split(false);
        }
    } else {
        if (!attempt_split(false)) {
            attempt_split(true);
        }
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

    std::iota(p_indices.begin(), p_indices.end(), 0);

    for (int i = 0; i < n; ++i) {
        std::cin >> companies[i].x >> companies[i].y >> companies[i].r;
    }
    
    solve(0, 0, 10000, 10000, 0, n);

    for (int i = 0; i < n; ++i) {
        std::cout << result_rects[i].x1 << " " << result_rects[i].y1 << " "
                  << result_rects[i].x2 << " " << result_rects[i].y2 << "\n";
    }

    return 0;
}