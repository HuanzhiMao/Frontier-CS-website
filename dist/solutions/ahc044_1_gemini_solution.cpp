#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

int N;
int L;
std::vector<int> T;

struct Solution {
    std::vector<int> a, b;
};

std::mt19937 rng;
std::chrono::steady_clock::time_point start_time;

long long evaluate(const Solution& sol) {
    std::vector<int> t_counts(N, 0);
    int current_cleaner = 0;
    t_counts[0]++;
    for (int week = 1; week < L; ++week) {
        int last_cleaner = current_cleaner;
        int last_cleaner_count = t_counts[last_cleaner];
        if (last_cleaner_count % 2 != 0) {
            current_cleaner = sol.a[last_cleaner];
        } else {
            current_cleaner = sol.b[last_cleaner];
        }
        t_counts[current_cleaner]++;
    }

    long long error = 0;
    for (int i = 0; i < N; ++i) {
        error += std::abs(t_counts[i] - T[i]);
    }
    return error;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    start_time = std::chrono::steady_clock::now();
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    std::cin >> N >> L;
    T.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> T[i];
    }

    Solution best_sol;
    best_sol.a.resize(N);
    best_sol.b.resize(N);

    struct Source {
        int owner;
        int val;
        bool is_a;
    };
    std::vector<Source> sources;
    for (int i = 0; i < N; ++i) {
        sources.push_back({i, (T[i] + 1) / 2, true});
        sources.push_back({i, T[i] / 2, false});
    }
    std::sort(sources.begin(), sources.end(), [](const Source& x, const Source& y) {
        return x.val > y.val;
    });

    std::vector<double> needed(N);
    for (int i = 0; i < N; ++i) {
        needed[i] = T[i];
    }

    for (const auto& s : sources) {
        int max_idx = -1, second_max_idx = -1;
        double max_val = -1e18, second_max_val = -1e18;
        for (int i = 0; i < N; ++i) {
            if (needed[i] > max_val) {
                second_max_val = max_val;
                second_max_idx = max_idx;
                max_val = needed[i];
                max_idx = i;
            } else if (needed[i] > second_max_val) {
                second_max_val = needed[i];
                second_max_idx = i;
            }
        }
        
        int target_node = max_idx;
        if (target_node == s.owner && N > 1) {
            target_node = second_max_idx;
        }
        
        if (s.is_a) {
            best_sol.a[s.owner] = target_node;
        } else {
            best_sol.b[s.owner] = target_node;
        }
        needed[target_node] -= s.val;
    }

    Solution current_sol = best_sol;
    long long best_error = evaluate(best_sol);
    long long current_error = best_error;

    double time_limit = 1.95;
    
    double T_start = 1000;
    double T_end = 0.1;

    int iter_count = 0;
    while (true) {
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time).count();
        if (elapsed > time_limit) break;
        
        iter_count++;

        Solution new_sol = current_sol;
        int i = std::uniform_int_distribution<int>(0, N - 1)(rng);
        int type = std::uniform_int_distribution<int>(0, 1)(rng);
        int new_j = std::uniform_int_distribution<int>(0, N - 1)(rng);
        
        if (type == 0) {
            if (new_sol.a[i] == new_j) continue;
            new_sol.a[i] = new_j;
        } else {
            if (new_sol.b[i] == new_j) continue;
            new_sol.b[i] = new_j;
        }

        long long new_error = evaluate(new_sol);
        
        double temp = T_start * std::pow(T_end / T_start, elapsed / time_limit);
        double acceptance_prob = std::exp((double)(current_error - new_error) / temp);

        if (acceptance_prob > std::uniform_real_distribution<double>(0.0, 1.0)(rng)) {
            current_sol = new_sol;
            current_error = new_error;
        }

        if (current_error < best_error) {
            best_error = current_error;
            best_sol = current_sol;
        }
    }
    
    for (int i = 0; i < N; ++i) {
        std::cout << best_sol.a[i] << " " << best_sol.b[i] << "\n";
    }

    return 0;
}