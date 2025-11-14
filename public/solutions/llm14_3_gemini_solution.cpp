#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <map>

// Interaction functions
long long walk(long long x) {
    std::cout << "walk " << x << std::endl;
    long long v;
    std::cin >> v;
    if (v == -1) exit(0);
    return v;
}

void guess(long long g) {
    std::cout << "guess " << g << std::endl;
    exit(0);
}

// Helper for prime factorization
std::vector<long long> get_distinct_prime_factors(long long n) {
    std::vector<long long> factors;
    if (n <= 1) return factors;
    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0) n /= 2;
    }
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 2) {
        factors.push_back(n);
    }
    return factors;
}

// After finding a multiple C of n, this function finds n
void solve_for_n(long long C, long long p_ref) {
    long long n_cand = C;
    std::vector<long long> p_factors = get_distinct_prime_factors(C);
    
    for (long long p : p_factors) {
        while (n_cand > 1 && n_cand % p == 0) {
            long long d = n_cand / p;
            long long test_pos = walk(d);
            if (test_pos == p_ref) {
                n_cand = d;
            } else {
                // Return to p_ref.
                // A walk of C steps is a full cycle (or multiple).
                // So walking d steps and then C-d steps is equivalent to walking C steps,
                // which brings us back to where we started the C-step walk.
                // We started at p_ref and walked d, so we walk C-d to return.
                walk(C - d); 
                break;
            }
        }
    }
    guess(n_cand);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long m = 32000;
    std::unordered_map<long long, long long> visited;

    long long current_pos;
    // Baby steps
    for (long long i = 1; i <= m; ++i) {
        current_pos = walk(1);
        if (visited.count(current_pos)) {
            long long C = i - visited[current_pos];
            solve_for_n(C, current_pos);
        }
        visited[current_pos] = i;
    }

    // Giant steps
    // After baby steps, total distance is m, current position is current_pos.
    for (long long k = 1; ; ++k) {
        current_pos = walk(m);
        if (visited.count(current_pos)) {
            long long coll_dist = visited[current_pos];
            long long total_dist = m + k * m;
            long long C = total_dist - coll_dist;
            solve_for_n(C, current_pos);
        }
    }

    return 0;
}