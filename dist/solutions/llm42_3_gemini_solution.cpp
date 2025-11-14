#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <numeric>
#include <algorithm>

const double PI = acos(-1.0);

struct Square {
    double x, y, angle;
};

struct Result {
    double L;
    std::vector<Square> squares;
};

std::map<int, Result> memo;

Result solve(int n);

Result baseline_packing(int n) {
    if (n == 0) return {0.0, {}};
    int k = static_cast<int>(ceil(sqrt(n)));
    double L = k;
    std::vector<Square> squares;
    squares.reserve(n);
    int count = 0;
    for (int i = 0; i < k && count < n; ++i) {
        for (int j = 0; j < k && count < n; ++j) {
            squares.push_back({0.5 + j, 0.5 + i, 0.0});
            count++;
        }
    }
    return {L, squares};
}

Result special_packing_5() {
    double L = 2.0 + 1.0 / sqrt(2.0);
    std::vector<Square> squares;
    squares.push_back({L / 2.0, L / 2.0, 45.0});
    squares.push_back({0.5, 0.5, 0.0});
    squares.push_back({L - 0.5, 0.5, 0.0});
    squares.push_back({0.5, L - 0.5, 0.0});
    squares.push_back({L - 0.5, L - 0.5, 0.0});
    return {L, squares};
}


Result solve(int n) {
    if (memo.count(n)) {
        return memo[n];
    }

    if (n <= 4) {
        if (n == 0) return memo[n] = {0.0, {}};
        if (n == 1) return memo[n] = {1.0, {{0.5, 0.5, 0.0}}};
        return memo[n] = baseline_packing(n);
    }
    if (n == 5) {
        return memo[n] = special_packing_5();
    }
    if (n <= 9) {
        return memo[n] = baseline_packing(n);
    }

    Result res_baseline = baseline_packing(n);

    int k = n / 4;
    int r = n % 4;
    
    Result res_k = solve(k);
    Result res_k1 = (r > 0) ? solve(k + 1) : res_k;

    std::vector<Result*> sub_results;
    for(int i = 0; i < r; ++i) sub_results.push_back(&res_k1);
    for(int i = 0; i < 4 - r; ++i) sub_results.push_back(&res_k);
    
    Result best_res_rec;
    best_res_rec.L = 1e18;

    int p[4] = {0, 1, 2, 3};
    int configs[3][4] = {{0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 2, 1}};

    for (int i = 0; i < 3; ++i) {
        Result* r1 = sub_results[configs[i][0]];
        Result* r2 = sub_results[configs[i][1]];
        Result* r3 = sub_results[configs[i][2]];
        Result* r4 = sub_results[configs[i][3]];

        double W = std::max(r1->L, r3->L) + std::max(r2->L, r4->L);
        double H = std::max(r1->L, r2->L) + std::max(r3->L, r4->L);
        double current_L = std::max(W, H);

        if (current_L < best_res_rec.L) {
            best_res_rec.L = current_L;
            best_res_rec.squares.clear();
            
            double w1_offset = std::max(r1->L, r3->L);
            double h1_offset = std::max(r1->L, r2->L);

            for(const auto& sq : r1->squares) best_res_rec.squares.push_back({sq.x, sq.y, sq.angle});
            for(const auto& sq : r2->squares) best_res_rec.squares.push_back({sq.x + w1_offset, sq.y, sq.angle});
            for(const auto& sq : r3->squares) best_res_rec.squares.push_back({sq.x, sq.y + h1_offset, sq.angle});
            for(const auto& sq : r4->squares) best_res_rec.squares.push_back({sq.x + w1_offset, sq.y + h1_offset, sq.angle});
        }
    }


    if (best_res_rec.L < res_baseline.L) {
        return memo[n] = best_res_rec;
    } else {
        return memo[n] = res_baseline;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    Result result = solve(n);

    std::cout << std::fixed << std::setprecision(10) << result.L << "\n";
    for (const auto& s : result.squares) {
        std::cout << std::fixed << std::setprecision(10) << s.x << " " << s.y << " " << s.angle << "\n";
    }

    return 0;
}