#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>

// Use long double for precision in distance square intermediate calculations,
// as coordinate differences can be large.
using LD = long double;

struct Point {
    long long x, y;
};

// Global variables to hold city data for easy access in helper functions.
int N;
std::vector<Point> cities;
std::vector<bool> is_prime;

// Precompute prime numbers up to N using a standard Sieve of Eratosthenes.
void sieve(int n) {
    if (n <= 1) {
        is_prime.assign(n, false);
        return;
    }
    is_prime.assign(n, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p < n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i < n; i += p)
                is_prime[i] = false;
        }
    }
}

// Calculate Euclidean distance between two cities.
inline double dist(int c1_id, int c2_id) {
    LD dx = cities[c1_id].x - cities[c2_id].x;
    LD dy = cities[c1_id].y - cities[c2_id].y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    // Fast I/O for performance.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N;
    cities.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> cities[i].x >> cities[i].y;
    }

    sieve(N);

    std::vector<int> tour;
    tour.reserve(N + 1);

    // --- Initial Tour Construction: Snaking Heuristic ---
    // The input cities are sorted by x-coordinate. We partition them into vertical strips
    // and sort cities within each strip by y-coordinate, alternating direction (up/down)
    // to create a "snaking" path. This is a strong heuristic for geometric TSP.
    std::vector<int> path_cities;
    path_cities.reserve(N - 1);
    for (int i = 1; i < N; ++i) path_cities.push_back(i);

    int W = 450; // Strip width parameter, tuned for N up to 200,000.
    if (N < 5000) W = static_cast<int>(sqrt(N));
    if (W == 0) W = 1;

    bool forward_sort = true;
    for (int i = 0; i < N - 1; i += W) {
        int start_idx = i;
        int end_idx = std::min(i + W, N - 1);

        std::sort(path_cities.begin() + start_idx,
                  path_cities.begin() + end_idx,
                  [&](int a, int b) {
            if (forward_sort) return cities[a].y < cities[b].y;
            else return cities[a].y > cities[b].y;
        });
        forward_sort = !forward_sort;
    }

    tour.push_back(0);
    for (int id : path_cities) tour.push_back(id);
    tour.push_back(0);


    // --- Tour Improvement: Randomized 2-opt ---
    // We use a time-limited randomized 2-opt local search to improve the initial tour.
    // 2-opt involves reversing a subsegment of the tour to uncross paths.
    // We select random subsegments within a limited window to make cost calculations fast.
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    auto start_time = std::chrono::high_resolution_clock::now();
    const double time_limit_seconds = 1.95; // Leave a small margin from the 2s limit.

    if (N > 3) {
        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - start_time;
            if (elapsed.count() > time_limit_seconds) break;

            int window_size = 250;
            if (N < 1000) window_size = N / 4;
            if (window_size < 2) window_size = 2;

            std::uniform_int_distribution<int> i_dist(1, N - 2);
            int i = i_dist(rng);

            int max_j = std::min(N - 1, i + window_size);
            if (i + 1 > max_j) continue;
            std::uniform_int_distribution<int> j_dist(i + 1, max_j);
            int j = j_dist(rng);

            // Calculate cost change for a 2-opt move (reversing tour[i...j])
            // This is the most performance-critical part.
            double old_cost = 0;
            int u = tour[i - 1], v = tour[i];
            old_cost += dist(u, v) * (i % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);
            for (int k = i + 1; k <= j; ++k) {
                u = tour[k - 1]; v = tour[k];
                old_cost += dist(u, v) * (k % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);
            }
            u = tour[j]; v = tour[j + 1];
            old_cost += dist(u, v) * ((j + 1) % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);

            double new_cost = 0;
            u = tour[i - 1]; v = tour[j];
            new_cost += dist(u, v) * (i % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);
            for (int k = i + 1; k <= j; ++k) {
                u = tour[i + j - k + 1]; v = tour[i + j - k];
                new_cost += dist(u, v) * (k % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);
            }
            u = tour[i]; v = tour[j + 1];
            new_cost += dist(u, v) * ((j + 1) % 10 == 0 && !is_prime[u] ? 1.1 : 1.0);

            if (new_cost < old_cost) {
                std::reverse(tour.begin() + i, tour.begin() + j + 1);
            }
        }
    }

    // Output the final tour
    std::cout << N + 1 << "\n";
    for (int city_id : tour) {
        std::cout << city_id << "\n";
    }

    return 0;
}