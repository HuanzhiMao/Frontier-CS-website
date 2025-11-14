#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <cmath>

// Function to make a walk query
int walk(long long x) {
    std::cout << "walk " << x << std::endl;
    int result;
    std::cin >> result;
    if (result == 0) {
        exit(0);
    }
    return result;
}

// Function to make a guess
void guess(long long g) {
    std::cout << "guess " << g << std::endl;
}

std::map<long long, int> prime_factorize(long long n) {
    std::map<long long, int> factors;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                n /= i;
                count++;
            }
            factors[i] = count;
        }
    }
    if (n > 1) {
        factors[n] = 1;
    }
    return factors;
}

void generate_divs_recursive(
    const std::vector<std::pair<long long, int>>& factors,
    int k,
    long long current_divisor,
    std::vector<long long>& result) {
    
    if (k == (int)factors.size()) {
        result.push_back(current_divisor);
        return;
    }
    
    long long p = factors[k].first;
    int a = factors[k].second;
    long long term = 1;
    for (int i = 0; i <= a; ++i) {
        generate_divs_recursive(factors, k + 1, current_divisor * term, result);
        if (i < a) {
            term *= p;
        }
    }
}

std::vector<long long> get_divisors(long long n) {
    if (n <= 0) return {};
    auto prime_factors_map = prime_factorize(n);
    std::vector<std::pair<long long, int>> factors(prime_factors_map.begin(), prime_factors_map.end());
    std::vector<long long> result;
    generate_divs_recursive(factors, 0, 1, result);
    return result;
}

void solve() {
    int s0 = walk(0);

    std::map<int, long long> visited;
    visited[s0] = 0;
    long long total_dist = 0;
    
    long long C = -1;
    int v_collision = -1;

    // Phase 1: Small n search (linear walk)
    const int K_small = 2000;
    for (int i = 0; i < K_small; ++i) {
        total_dist++;
        int v = walk(1);
        if (visited.count(v)) {
            guess(total_dist - visited[v]);
            return;
        }
        visited[v] = total_dist;
    }

    // Phase 2: Random walk for larger n
    const int K_rand = 55000;
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<long long> distrib(1, 1000000000);
    
    for (int i = 0; i < K_rand; ++i) {
        long long x = distrib(rng);
        int v = walk(x);
        total_dist += x;
        if (visited.count(v)) {
            C = total_dist - visited[v];
            v_collision = v;
            break;
        }
        visited[v] = total_dist;
    }

    if (C == -1) {
        while (C == -1) {
            long long x = distrib(rng);
            int v = walk(x);
            total_dist += x;
            if (visited.count(v)) {
                C = total_dist - visited[v];
                v_collision = v;
                break;
            }
            visited[v] = total_dist;
        }
    }

    // Phase 3: Verification
    std::vector<long long> divisors = get_divisors(C);
    std::sort(divisors.begin(), divisors.end());

    long long dist_walked_in_verification = 0;
    for (long long d : divisors) {
        if (d == 0) continue;
        long long walk_amount = d - dist_walked_in_verification;
        int v_new = walk(walk_amount);
        dist_walked_in_verification = d;
        if (v_new == v_collision) {
            guess(d);
            return;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}