#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

// Global constants from the problem statement
const int N_CONST = 100;
const int L_CONST = 500000;

// Global variables to hold problem input
int N, L;
std::vector<int> T;

// Random number generator
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// A temporary global vector to store counts from simulation,
// avoiding repeated memory allocation.
std::vector<long long> t_counts_temp;

// Simulates the cleaning process and returns the total error.
// Also updates t_counts_temp with the resulting counts.
long long calculate_error(const std::vector<int>& a, const std::vector<int>& b) {
    t_counts_temp.assign(N, 0);
    int current_cleaner = 0;
    
    for (int week = 0; week < L; ++week) {
        t_counts_temp[current_cleaner]++;
        if (week == L - 1) break;

        if (t_counts_temp[current_cleaner] % 2 != 0) { // Odd count
            current_cleaner = a[current_cleaner];
        } else { // Even count
            current_cleaner = b[current_cleaner];
        }
    }

    long long error = 0;
    for (int i = 0; i < N; ++i) {
        error += std::abs(t_counts_temp[i] - T[i]);
    }
    return error;
}

// Generates an initial solution using a greedy heuristic.
// The heuristic tries to match "supplies" of transitions from employees
// to the "demands" of other employees.
void initial_greedy_solution(std::vector<int>& a, std::vector<int>& b) {
    a.assign(N, 0);
    b.assign(N, 0);

    struct Supply {
        int val;
        int owner;
        bool is_odd;
    };

    std::vector<Supply> supplies;
    for (int i = 0; i < N; ++i) {
        supplies.push_back({(T[i] + 1) / 2, i, true}); // Transitions on odd counts
        supplies.push_back({T[i] / 2, i, false});    // Transitions on even counts
    }
    // Process larger supplies first
    std::sort(supplies.begin(), supplies.end(), [](const Supply& s1, const Supply& s2) {
        return s1.val > s2.val;
    });

    // Demands are the target counts, adjusted for the starting cleaner
    std::vector<long long> demands(N);
    for (int i = 0; i < N; ++i) {
        demands[i] = T[i];
    }
    if (demands[0] > 0) demands[0]--;

    for (const auto& supply : supplies) {
        if (supply.val == 0) continue;
        
        // Find employee with the highest remaining demand
        int best_j = 0;
        for (int j = 1; j < N; ++j) {
            if (demands[j] > demands[best_j]) {
                best_j = j;
            }
        }
        
        if (supply.is_odd) {
            a[supply.owner] = best_j;
        } else {
            b[supply.owner] = best_j;
        }
        demands[best_j] -= supply.val;
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> L;
    T.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> T[i];
    }

    std::vector<int> a, b;
    initial_greedy_solution(a, b);

    long long current_error = calculate_error(a, b);
    std::vector<long long> current_t = t_counts_temp;

    std::vector<int> best_a = a;
    std::vector<int> best_b = b;
    long long best_error = current_error;

    // Parameters for simulated annealing
    double T_start = 5000;
    double T_end = 1;
    double time_limit_sec = 1.95;

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed_sec = std::chrono::duration<double>(now - start_time).count();
        if (elapsed_sec > time_limit_sec) {
            break;
        }
        
        double progress = elapsed_sec / time_limit_sec;
        double temp = T_start * std::pow(T_end / T_start, progress);

        std::vector<int> next_a = a;
        std::vector<int> next_b = b;

        int move_type = std::uniform_int_distribution<int>(0, 9)(rng);

        if (move_type < 5) { // 50% chance: Random change
            int change_idx = std::uniform_int_distribution<int>(0, N - 1)(rng);
            int new_val = std::uniform_int_distribution<int>(0, N - 1)(rng);
            if (std::uniform_int_distribution<int>(0, 1)(rng) == 0) {
                if (new_val == next_a[change_idx]) new_val = (new_val + 1) % N;
                next_a[change_idx] = new_val;
            } else {
                if (new_val == next_b[change_idx]) new_val = (new_val + 1) % N;
                next_b[change_idx] = new_val;
            }
        } else if (move_type < 8) { // 30% chance: Swap
            int idx1 = std::uniform_int_distribution<int>(0, N - 1)(rng);
            int idx2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
            bool type1_is_a = std::uniform_int_distribution<int>(0, 1)(rng) == 0;
            bool type2_is_a = std::uniform_int_distribution<int>(0, 1)(rng) == 0;
            
            if (type1_is_a && type2_is_a) { std::swap(next_a[idx1], next_a[idx2]); }
            else if (type1_is_a && !type2_is_a) { std::swap(next_a[idx1], next_b[idx2]); }
            else if (!type1_is_a && type2_is_a) { std::swap(next_b[idx1], next_a[idx2]); }
            else { std::swap(next_b[idx1], next_b[idx2]); }
        } else { // 20% chance: Guided change
            long long max_diff = -2e18; int i_over = -1;
            long long min_diff = 2e18; int i_under = -1;
            
            for (int i = 0; i < N; ++i) {
                long long diff = current_t[i] - T[i];
                if (diff > max_diff) { max_diff = diff; i_over = i; }
                if (diff < min_diff) { min_diff = diff; i_under = i; }
            }
            
            if (i_over != -1 && i_under != -1 && i_over != i_under) {
                std::vector<std::pair<int, bool>> sources;
                for (int i = 0; i < N; ++i) {
                    if (a[i] == i_over) sources.push_back({i, true});
                    if (b[i] == i_over) sources.push_back({i, false});
                }
                if (!sources.empty()) {
                    auto [src_idx, is_a] = sources[std::uniform_int_distribution<int>(0, sources.size() - 1)(rng)];
                    if (is_a) next_a[src_idx] = i_under;
                    else next_b[src_idx] = i_under;
                }
            }
        }
        
        long long next_error = calculate_error(next_a, next_b);
        double delta = next_error - current_error;

        if (delta < 0 || std::uniform_real_distribution<double>(0.0, 1.0)(rng) < std::exp(-delta / temp)) {
            a = next_a;
            b = next_b;
            current_error = next_error;
            current_t = t_counts_temp;

            if (current_error < best_error) {
                best_error = current_error;
                best_a = a;
                best_b = b;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        std::cout << best_a[i] << " " << best_b[i] << "\n";
    }

    return 0;
}