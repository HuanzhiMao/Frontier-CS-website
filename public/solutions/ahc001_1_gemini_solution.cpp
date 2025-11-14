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

std::vector<Rect> ans;

// Calculates the term (1 - min/max)^2 from the problem's satisfaction formula.
// We want to minimize the sum of these terms, weighted by desired area r.
double calculate_satisfaction_term(long long s, long long r) {
    if (s <= 0 || r <= 0) return 1.0; // Max penalty for invalid area
    double ratio = std::min((double)s, (double)r) / std::max((double)s, (double)r);
    double term = 1.0 - ratio;
    return term * term;
}

void solve(std::vector<Company> companies, Rect R) {
    int k = companies.size();
    if (k == 0) return;
    if (k == 1) {
        ans[companies[0].id] = R;
        return;
    }

    long long total_r = 0;
    for (const auto& c : companies) total_r += c.r;

    // --- Try vertical split ---
    std::sort(companies.begin(), companies.end(), [](const Company& a, const Company& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    int best_j_v = -1;
    int best_x_split_v = -1;
    double min_cost_v = 1e18;

    long long r_prefix_sum = 0;
    for (int j = 1; j < k; ++j) {
        r_prefix_sum += companies[j - 1].r;
        // A split is only possible between points with different x-coordinates
        if (companies[j - 1].x < companies[j].x) {
            long long r1 = r_prefix_sum;
            long long r2 = total_r - r1;

            // Ideal split position based on area proportions
            double ideal_x_double = R.x1 + (double)(R.x2 - R.x1) * r1 / total_r;
            int ideal_x = static_cast<int>(floor(ideal_x_double + 0.5));

            // Valid range for the split line to ensure point containment and positive area
            int min_split = std::max(R.x1 + 1, companies[j - 1].x + 1);
            int max_split = std::min(R.x2 - 1, companies[j].x);
            
            if (min_split > max_split) continue;

            // Clamp ideal split to the valid range
            int x_split = std::max(min_split, std::min(max_split, ideal_x));
            
            long long s1 = (long long)(x_split - R.x1) * (R.y2 - R.y1);
            long long s2 = (long long)(R.x2 - x_split) * (R.y2 - R.y1);
            
            // Cost is the sum of weighted dissatisfaction terms
            double cost = r1 * calculate_satisfaction_term(s1, r1) + r2 * calculate_satisfaction_term(s2, r2);
            if (cost < min_cost_v) {
                min_cost_v = cost;
                best_j_v = j;
                best_x_split_v = x_split;
            }
        }
    }
    
    // --- Try horizontal split ---
    std::sort(companies.begin(), companies.end(), [](const Company& a, const Company& b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    });

    int best_j_h = -1;
    int best_y_split_h = -1;
    double min_cost_h = 1e18;
    
    r_prefix_sum = 0;
    for (int j = 1; j < k; ++j) {
        r_prefix_sum += companies[j-1].r;
        if (companies[j-1].y < companies[j].y) {
            long long r1 = r_prefix_sum;
            long long r2 = total_r - r1;

            double ideal_y_double = R.y1 + (double)(R.y2 - R.y1) * r1 / total_r;
            int ideal_y = static_cast<int>(floor(ideal_y_double + 0.5));

            int min_split = std::max(R.y1 + 1, companies[j-1].y + 1);
            int max_split = std::min(R.y2 - 1, companies[j].y);

            if (min_split > max_split) continue;
            
            int y_split = std::max(min_split, std::min(max_split, ideal_y));

            long long s1 = (long long)(R.x2 - R.x1) * (y_split - R.y1);
            long long s2 = (long long)(R.x2 - R.x1) * (R.y2 - y_split);
            
            double cost = r1 * calculate_satisfaction_term(s1, r1) + r2 * calculate_satisfaction_term(s2, r2);

            if (cost < min_cost_h) {
                min_cost_h = cost;
                best_j_h = j;
                best_y_split_h = y_split;
            }
        }
    }

    // --- Decide and execute the best split ---
    bool can_split_v = (best_j_v != -1);
    bool can_split_h = (best_j_h != -1);
    
    // Since all points are distinct, at least one split direction must be possible for k > 1.
    bool use_vertical;
    if (can_split_v && can_split_h) {
        use_vertical = min_cost_v < min_cost_h;
    } else {
        use_vertical = can_split_v;
    }

    if (use_vertical) {
        std::sort(companies.begin(), companies.end(), [](const Company& a, const Company& b) {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });
        std::vector<Company> group1(companies.begin(), companies.begin() + best_j_v);
        std::vector<Company> group2(companies.begin() + best_j_v, companies.end());
        solve(group1, {R.x1, R.y1, best_x_split_v, R.y2});
        solve(group2, {best_x_split_v, R.y1, R.x2, R.y2});
    } else {
        std::sort(companies.begin(), companies.end(), [](const Company& a, const Company& b) {
            if (a.y != b.y) return a.y < b.y;
            return a.x < b.x;
        });
        std::vector<Company> group1(companies.begin(), companies.begin() + best_j_h);
        std::vector<Company> group2(companies.begin() + best_j_h, companies.end());
        solve(group1, {R.x1, R.y1, R.x2, best_y_split_h});
        solve(group2, {R.x1, best_y_split_h, R.x2, R.y2});
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<Company> companies(n);
    for (int i = 0; i < n; ++i) {
        companies[i].id = i;
        std::cin >> companies[i].x >> companies[i].y >> companies[i].r;
    }
    ans.resize(n);
    solve(companies, {0, 0, 10000, 10000});
    for (int i = 0; i < n; ++i) {
        std::cout << ans[i].x1 << " " << ans[i].y1 << " " << ans[i].x2 << " " << ans[i].y2 << "\n";
    }

    return 0;
}