#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

struct Point {
    long long x, y;
};

int N;
std::vector<Point> cities;
std::vector<bool> is_prime;
std::vector<bool> visited;

void sieve(int n) {
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

long long distSq(int c1_idx, int c2_idx) {
    long long dx = cities[c1_idx].x - cities[c2_idx].x;
    long long dy = cities[c1_idx].y - cities[c2_idx].y;
    return dx * dx + dy * dy;
}

double dist(int c1_idx, int c2_idx) {
    return std::sqrt(static_cast<long double>(distSq(c1_idx, c2_idx)));
}

std::pair<int, long long> find_nearest_unvisited(int current_city, const std::function<bool(int)>& predicate) {
    int best_city = -1;
    long long min_dist_sq = -1;

    for (int W = 32; ; W *= 2) {
        if (W > 2 * N) W = 2 * N;
        int L = std::max(1, current_city - W);
        int R = std::min(N - 1, current_city + W);

        for (int i = L; i <= R; ++i) {
            if (!visited[i] && predicate(i)) {
                long long d_sq = distSq(current_city, i);
                if (best_city == -1 || d_sq < min_dist_sq) {
                    min_dist_sq = d_sq;
                    best_city = i;
                }
            }
        }

        if (best_city != -1) break;
        if (W >= 2 * N) break;
    }
    
    if (best_city == -1) { // Fallback for sparse predicates
        for (int i = 1; i < N; ++i) {
            if (!visited[i] && predicate(i)) {
                long long d_sq = distSq(current_city, i);
                if (best_city == -1 || d_sq < min_dist_sq) {
                    min_dist_sq = d_sq;
                    best_city = i;
                }
            }
        }
    }

    return {best_city, min_dist_sq};
}

int main() {
    fast_io();

    std::cin >> N;
    cities.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> cities[i].x >> cities[i].y;
    }

    sieve(N);

    double avg_dist = 0;
    if (N > 1) {
        for (int i = 0; i < N - 1; ++i) {
            avg_dist += dist(i, i + 1);
        }
        avg_dist /= (N - 1);
    }
    double penalty_benefit = 0.1 * avg_dist;

    std::vector<int> path;
    path.reserve(N + 1);
    path.push_back(0);
    visited.assign(N, false);
    visited[0] = true;
    int current_city = 0;
    
    int primes_left = 0;
    for (int i = 2; i < N; ++i) {
        if (is_prime[i]) {
            primes_left++;
        }
    }
    
    auto any_pred = [](int) { return true; };
    auto is_prime_pred = [](int i){ return is_prime[i]; };

    for (int i = 1; i < N; ++i) {
        int next_city = -1;
        bool is_critical_choice = ((i + 1) % 10 == 0);

        if (is_critical_choice && primes_left > 0) {
            auto [p_cand, p_dist_sq] = find_nearest_unvisited(current_city, is_prime_pred);
            
            if (p_cand != -1) {
                auto [any_cand, any_dist_sq] = find_nearest_unvisited(current_city, any_pred);
                
                if (any_cand == p_cand) {
                    next_city = p_cand;
                } else {
                    double p_dist = std::sqrt(static_cast<long double>(p_dist_sq));
                    double any_dist = std::sqrt(static_cast<long double>(any_dist_sq));
                    if (p_dist < any_dist + penalty_benefit) {
                        next_city = p_cand;
                    } else {
                        next_city = any_cand;
                    }
                }
            } else {
                auto [any_cand, any_dist_sq] = find_nearest_unvisited(current_city, any_pred);
                next_city = any_cand;
            }
        } else {
            auto [any_cand, any_dist_sq] = find_nearest_unvisited(current_city, any_pred);
            next_city = any_cand;
        }

        path.push_back(next_city);
        visited[next_city] = true;
        if (is_prime[next_city]) {
            primes_left--;
        }
        current_city = next_city;
    }

    path.push_back(0);

    std::cout << N + 1 << "\n";
    for (int city_idx : path) {
        std::cout << city_idx << "\n";
    }

    return 0;
}