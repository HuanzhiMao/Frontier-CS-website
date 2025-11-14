#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

// Define a structure for 2D points (cities)
struct Point {
    long long x, y;
};

// Global vector to store city coordinates
std::vector<Point> cities;
// Global vector to store primality information
std::vector<bool> is_prime;

// Function to calculate Euclidean distance between two cities by their IDs
double get_dist(int c1, int c2) {
    long long dx = cities[c1].x - cities[c2].x;
    long long dy = cities[c1].y - cities[c2].y;
    return std::sqrt(static_cast<double>(dx * dx + dy * dy));
}

// Sieve of Eratosthenes to find all primes up to n-1
void generate_primes(int n) {
    is_prime.assign(n, true);
    if (n > 0) is_prime[0] = false;
    if (n > 1) is_prime[1] = false;
    for (int p = 2; p * p < n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i < n; i += p)
                is_prime[i] = false;
        }
    }
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    cities.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> cities[i].x >> cities[i].y;
    }

    generate_primes(n);

    // p[i] stores the city ID at tour position i (for i=1..N-1)
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);

    // Heuristic improvement only for N large enough to have penalties
    if (n > 9) {
        // Identify tour positions that cause penalties in the baseline tour
        // and store them with their penalty value (negated for max-heap like sorting)
        std::vector<std::pair<double, int>> problem_indices;
        for (int i = 9; i < n; i += 10) {
            if (!is_prime[p[i]]) {
                double penalty_dist;
                if (i == n - 1) {
                    penalty_dist = get_dist(p[i], 0);
                } else {
                    penalty_dist = get_dist(p[i], p[i + 1]);
                }
                problem_indices.push_back({-0.1 * penalty_dist, i});
            }
        }
        // Process problems with the largest penalties first
        std::sort(problem_indices.begin(), problem_indices.end());

        // Identify tour positions with prime cities that can be used to fix problems
        std::vector<int> fix_indices;
        for (int j = 1; j < n; ++j) {
            if (is_prime[p[j]] && (j % 10 != 9)) {
                fix_indices.push_back(j);
            }
        }
        
        std::vector<bool> fix_pos_used(n, false);

        // Greedily fix problems
        for (const auto& pair : problem_indices) {
            int i = pair.second;
            
            int best_j = -1;
            double min_delta = 1e18; // Using a large value for infinity

            // Search for the best swap partner in a local window for efficiency
            auto it = std::lower_bound(fix_indices.begin(), fix_indices.end(), i);
            
            int search_window = 250;
            int start_idx = std::max(0, (int)(it - fix_indices.begin()) - search_window);
            int end_idx = std::min((int)fix_indices.size(), (int)(it - fix_indices.begin()) + search_window);

            for (int k = start_idx; k < end_idx; ++k) {
                int j = fix_indices[k];
                if (fix_pos_used[j]) continue;

                int city_i = p[i];
                int city_j = p[j];

                double current_dist, new_dist;
                
                // Get neighbors for city at position i
                int prev_i_city = (i > 1) ? p[i - 1] : 0;
                int next_i_city = (i < n - 1) ? p[i + 1] : 0;
                
                // Get neighbors for city at position j
                int prev_j_city = (j > 1) ? p[j - 1] : 0;
                int next_j_city = (j < n - 1) ? p[j + 1] : 0;
                
                // Calculate distance change for swapping p[i] and p[j]
                if (j == i + 1) { // adjacent case
                    current_dist = get_dist(prev_i_city, city_i) + get_dist(city_i, city_j) + get_dist(city_j, next_j_city);
                    new_dist = get_dist(prev_i_city, city_j) + get_dist(city_j, city_i) + get_dist(city_i, next_j_city);
                } else if (j == i - 1) { // adjacent case (swapped order)
                     current_dist = get_dist(prev_j_city, city_j) + get_dist(city_j, city_i) + get_dist(city_i, next_i_city);
                     new_dist = get_dist(prev_j_city, city_i) + get_dist(city_i, city_j) + get_dist(city_j, next_i_city);
                } else { // non-adjacent case
                    current_dist = get_dist(prev_i_city, city_i) + get_dist(city_i, next_i_city) +
                                   get_dist(prev_j_city, city_j) + get_dist(city_j, next_j_city);
                    new_dist = get_dist(prev_i_city, city_j) + get_dist(city_j, next_i_city) +
                               get_dist(prev_j_city, city_i) + get_dist(city_i, next_j_city);
                }
                
                double delta = new_dist - current_dist;

                if (delta < min_delta) {
                    min_delta = delta;
                    best_j = j;
                }
            }
            
            if (best_j != -1) {
                std::swap(p[i], p[best_j]);
                fix_pos_used[best_j] = true;
            }
        }
    }

    // Output the resulting tour
    std::cout << n + 1 << "\n";
    std::cout << 0 << "\n";
    for (int i = 1; i < n; ++i) {
        std::cout << p[i] << "\n";
    }
    std::cout << 0 << "\n";

    return 0;
}