#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <map>
#include <chrono>
#include <random>

// Use __int128_t for cross product calculations to avoid overflow
using int128 = __int128_t;

struct Point {
    long long x, y;
};

// For random number generation
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// Timer
auto start_time = std::chrono::steady_clock::now();

// Problem parameters
int N, K;
std::vector<int> a;
std::vector<Point> strawberries;
int total_a;

// Solution representation
using Line = std::pair<Point, Point>;

long long evaluate(const std::vector<Line>& cuts) {
    if (cuts.empty()) {
        std::vector<int> b(11, 0);
        if (N > 0 && N <= 10) {
            b[N] = 1;
        }
        long long score = 0;
        for (int d = 1; d <= 10; ++d) {
            score += std::min(a[d - 1], b[d]);
        }
        return score;
    }

    std::map<std::vector<int>, int> regions;
    for (int i = 0; i < N; ++i) {
        std::vector<int> sign_vec;
        bool on_line = false;
        for (const auto& line : cuts) {
            int128 p1x = line.first.x, p1y = line.first.y;
            int128 p2x = line.second.x, p2y = line.second.y;
            int128 sx = strawberries[i].x, sy = strawberries[i].y;
            
            int128 val = (p2y - p1y) * (sx - p1x) - (p2x - p1x) * (sy - p1y);
            
            if (val == 0) {
                on_line = true;
                break;
            }
            sign_vec.push_back(val > 0 ? 1 : -1);
        }
        if (!on_line) {
            regions[sign_vec]++;
        }
    }

    std::vector<int> b(11, 0);
    for (auto const& [key, val] : regions) {
        if (val >= 1 && val <= 10) {
            b[val]++;
        }
    }

    long long score = 0;
    for (int d = 1; d <= 10; ++d) {
        score += std::min(a[d - 1], b[d]);
    }
    return score;
}

Line get_perp_bisector_approx(Point s1, Point s2) {
    long long A_i = 2LL * (s2.x - s1.x);
    long long B_i = 2LL * (s2.y - s1.y);
    long long C_i = - (1LL*s2.x * s2.x - 1LL*s1.x * s1.x) - (1LL*s2.y * s2.y - 1LL*s1.y * s1.y);

    if (A_i == 0 && B_i == 0) {
        return {{0,0}, {1,0}};
    }

    long double A = A_i, B = B_i, C = C_i;

    const long long L = 30000;
    if (std::abs(A) > std::abs(B)) { // more horizontal
        long long y1 = L;
        long long y2 = -L;
        long long x1 = roundl(-(B * y1 + C) / A);
        long long x2 = roundl(-(B * y2 + C) / A);
        return {{x1, y1}, {x2, y2}};
    } else { // more vertical
        long long x1 = L;
        long long x2 = -L;
        long long y1 = roundl(-(A * x1 + C) / B);
        long long y2 = roundl(-(A * x2 + C) / B);
        return {{x1, y1}, {x2, y2}};
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> K;
    a.resize(10);
    for (int i = 0; i < 10; ++i) {
        std::cin >> a[i];
    }
    strawberries.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> strawberries[i].x >> strawberries[i].y;
    }

    std::vector<Line> current_cuts;
    
    long long current_score = evaluate(current_cuts);
    std::vector<Line> best_cuts = current_cuts;
    long long best_score = current_score;

    double T_start = 5.0, T_end = 0.01;
    double time_limit = 2.9; // seconds

    std::uniform_real_distribution<double> p_dist(0.0, 1.0);
    std::uniform_int_distribution<int> strawberry_dist(0, N - 1);
    std::uniform_int_distribution<int> move_type_dist(0, 2);
    
    int iter = 0;
    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time).count();
        if (elapsed_time > time_limit) {
            break;
        }
        iter++;

        double T = T_start + (T_end - T_start) * elapsed_time / time_limit;
        
        std::vector<Line> next_cuts = current_cuts;
        
        int move_type;
        if (current_cuts.size() == 0) {
            move_type = 0;
        } else if (current_cuts.size() >= K) {
            move_type = std::uniform_int_distribution<int>(1, 2)(rng);
        } else {
            move_type = move_type_dist(rng);
        }

        if (move_type == 0 && N > 1) { // Add
            int s1_idx = strawberry_dist(rng);
            int s2_idx = strawberry_dist(rng);
            while (s1_idx == s2_idx) {
                s2_idx = strawberry_dist(rng);
            }
            next_cuts.push_back(get_perp_bisector_approx(strawberries[s1_idx], strawberries[s2_idx]));
        } else if (move_type == 1) { // Remove
            std::uniform_int_distribution<int> cut_idx_dist(0, next_cuts.size() - 1);
            int cut_to_remove = cut_idx_dist(rng);
            next_cuts.erase(next_cuts.begin() + cut_to_remove);
        } else { // Modify
            std::uniform_int_distribution<int> cut_idx_dist(0, next_cuts.size() - 1);
            int modified_idx = cut_idx_dist(rng);
            Line& cut = next_cuts[modified_idx];
            
            int step = std::max(1, (int)(20000.0 * T / T_start));
            std::uniform_int_distribution<int> d_dist(-step, step);

            int p_idx = std::uniform_int_distribution<int>(0, 1)(rng);
            int c_idx = std::uniform_int_distribution<int>(0, 1)(rng);

            Point new_p = (p_idx == 0) ? cut.first : cut.second;

            if (c_idx == 0) new_p.x += d_dist(rng);
            else new_p.y += d_dist(rng);
            
            new_p.x = std::clamp(new_p.x, -30000LL, 30000LL);
            new_p.y = std::clamp(new_p.y, -30000LL, 30000LL);
            
            if (p_idx == 0) cut.first = new_p;
            else cut.second = new_p;
            
            if (cut.first.x == cut.second.x && cut.first.y == cut.second.y) {
                continue;
            }
        }
        
        long long next_score = evaluate(next_cuts);
        
        double delta = next_score - current_score;

        if (delta >= 0 || p_dist(rng) < std::exp(delta / T)) {
            current_cuts = next_cuts;
            current_score = next_score;
            if (current_score > best_score) {
                best_score = current_score;
                best_cuts = current_cuts;
            }
        }
    }

    std::cout << best_cuts.size() << std::endl;
    long long coord_limit = 1000000000;
    for (const auto& line : best_cuts) {
        long long px1 = std::clamp(line.first.x, -coord_limit, coord_limit);
        long long py1 = std::clamp(line.first.y, -coord_limit, coord_limit);
        long long px2 = std::clamp(line.second.x, -coord_limit, coord_limit);
        long long py2 = std::clamp(line.second.y, -coord_limit, coord_limit);
        if (px1 == px2 && py1 == py2) {
             px2++;
        }
        std::cout << px1 << " " << py1 << " " << px2 << " " << py2 << std::endl;
    }

    return 0;
}