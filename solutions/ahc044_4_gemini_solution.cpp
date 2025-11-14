#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>

// Constants
constexpr int N = 100;
constexpr int L = 500000;
constexpr double TIME_LIMIT = 2.8; // seconds, with a bit of safety margin

// Random number generator
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

struct State {
    std::vector<int> a, b;
    double score;
};

// Function to estimate counts for a given cleaning plan (a, b)
std::vector<double> estimate_counts(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& T) {
    std::vector<double> t(N);
    for (int i = 0; i < N; ++i) {
        t[i] = static_cast<double>(T[i]);
    }

    std::vector<double> next_t(N);
    // Iteratively refine the estimated counts
    for (int iter = 0; iter < 30; ++iter) {
        std::fill(next_t.begin(), next_t.end(), 0.0);
        next_t[0] += 1.0;

        for (int i = 0; i < N; ++i) {
            if (t[i] > 0.5) { // Only consider employees who clean at least once
                long long current_t_i = round(t[i]);
                long long odd_transitions = (current_t_i + 1) / 2;
                long long even_transitions = current_t_i / 2;
                
                next_t[a[i]] += odd_transitions;
                next_t[b[i]] += even_transitions;
            }
        }
        
        double current_sum = 0;
        for(double val : next_t) current_sum += val;

        if (current_sum > 1e-9) {
            for (int i = 0; i < N; ++i) {
                t[i] = next_t[i] * L / current_sum;
            }
        } else {
            // This case should be rare
            std::fill(t.begin(), t.end(), 0.0);
            if (N > 0) t[0] = L;
        }
    }
    return t;
}

double calculate_score(const std::vector<double>& t, const std::vector<int>& T) {
    double error = 0;
    for (int i = 0; i < N; ++i) {
        error += std::abs(t[i] - T[i]);
    }
    return error;
}

struct Supply {
    int size;
    int from;
    bool is_a;

    bool operator<(const Supply& other) const {
        return size > other.size; // Sort in descending order
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_dummy, l_dummy;
    std::cin >> n_dummy >> l_dummy;

    std::vector<int> T(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> T[i];
    }

    // --- Initial solution: Greedy ---
    std::vector<Supply> supplies;
    for (int i = 0; i < N; ++i) {
        supplies.push_back({(T[i] + 1) / 2, i, true});
        supplies.push_back({T[i] / 2, i, false});
    }
    std::sort(supplies.begin(), supplies.end());

    std::vector<double> rem_demand(N);
    for(int i=0; i<N; ++i) rem_demand[i] = T[i];

    State current_state;
    current_state.a.resize(N);
    current_state.b.resize(N);

    for (const auto& s : supplies) {
        auto best_j_it = std::max_element(rem_demand.begin(), rem_demand.end());
        int j = std::distance(rem_demand.begin(), best_j_it);
        
        if (s.is_a) {
            current_state.a[s.from] = j;
        } else {
            current_state.b[s.from] = j;
        }
        rem_demand[j] -= s.size;
    }
    
    auto t_est = estimate_counts(current_state.a, current_state.b, T);
    current_state.score = calculate_score(t_est, T);

    State best_state = current_state;

    // --- Simulated Annealing ---
    auto start_time = std::chrono::high_resolution_clock::now();
    double T_start = 1000, T_end = 0.1;
    std::uniform_int_distribution<> dist_n(0, N - 1);
    std::uniform_real_distribution<> dist_real(0.0, 1.0);

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start_time).count();
        if (elapsed > TIME_LIMIT) {
            break;
        }

        int i = dist_n(rng);
        int j = dist_n(rng);
        bool is_a = dist_real(rng) < 0.5;

        State next_state = current_state;
        int original_dest;
        if (is_a) {
            original_dest = next_state.a[i];
            if (original_dest == j) continue;
            next_state.a[i] = j;
        } else {
            original_dest = next_state.b[i];
            if (original_dest == j) continue;
            next_state.b[i] = j;
        }

        auto next_t_est = estimate_counts(next_state.a, next_state.b, T);
        next_state.score = calculate_score(next_t_est, T);
        
        double temp = T_start * std::pow(T_end / T_start, elapsed / TIME_LIMIT);

        double prob = std::exp((current_state.score - next_state.score) / temp);

        if (prob > dist_real(rng)) {
            current_state = next_state;
            if (current_state.score < best_state.score) {
                best_state = current_state;
            }
        }
    }
    
    // --- Output ---
    for (int i = 0; i < N; ++i) {
        std::cout << best_state.a[i] << " " << best_state.b[i] << "\n";
    }

    return 0;
}