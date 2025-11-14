#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <cmath>

// Function to send a query to the interactor and get the collision count.
long long query(const std::vector<long long>& elements) {
    std::cout << "0 " << elements.size();
    for (long long x : elements) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    long long collisions;
    std::cin >> collisions;
    return collisions;
}

// Function to submit the final answer.
void answer(long long n) {
    std::cout << "1 " << n << std::endl;
}

// Standard GCD function.
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Finds all divisors of n greater than a minimum value and adds them to a set.
void find_divisors(long long n, std::set<long long>& divisors, int min_val) {
    if (n == 0) return;
    for (long long i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i > min_val) divisors.insert(i);
            if ((n / i) > min_val) divisors.insert(n / i);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int K = 400;

    // Phase 1: Check for small n.
    std::vector<long long> initial_query_elements;
    for (int i = 1; i <= K; ++i) {
        initial_query_elements.push_back(i);
    }
    long long initial_collisions = query(initial_query_elements);

    if (initial_collisions > 0) {
        // n is small (<= K). Find it by checking the collision formula.
        for (int n_cand = 2; n_cand <= K; ++n_cand) {
            long long q = K / n_cand;
            long long r = K % n_cand;
            long long expected_collisions = r * (q + 1) * q / 2 + (n_cand - r) * q * (q - 1) / 2;
            if (expected_collisions == initial_collisions) {
                answer(n_cand);
                return 0;
            }
        }
    } else {
        // Phase 2: n is large (> K).
        const long long A0 = 1000000000000000000LL;
        const long long delta = 1000000007LL; // A large prime

        std::vector<long long> s0;
        for (int i = 1; i <= K; ++i) s0.push_back(A0 + i);

        auto get_R = [&](int multiple) {
            std::vector<long long> s_other;
            for (int i = 1; i <= K; ++i) s_other.push_back(A0 + (long long)multiple * delta + i);
            std::vector<long long> combined = s0;
            combined.insert(combined.end(), s_other.begin(), s_other.end());
            long long collisions = query(combined);
            return (long long)K - collisions;
        };

        long long R1 = get_R(1);
        long long R2 = get_R(2);
        long long R3 = get_R(3);

        if (R1 == 0 && R2 == 0 && R3 == 0) {
             // A special case where n likely divides delta. Since delta is prime, n=delta.
             answer(delta);
             return 0;
        }

        // Generate candidate multiples of n.
        long long G1 = std::abs(2 * R1 - R2);
        long long G2 = 2 * R1 + R2;
        long long G3 = std::abs(3 * R1 - R3);
        long long G4 = 3 * R1 + R3;
        
        // n must divide one of the gcds of pairs of candidate multiples.
        long long H1 = (G1 == 0 || G3 == 0) ? (G1 == 0 ? G3 : G1) : gcd(G1, G3);
        long long H2 = (G1 == 0 || G4 == 0) ? (G1 == 0 ? G4 : G1) : gcd(G1, G4);
        long long H3 = (G2 == 0 || G3 == 0) ? (G2 == 0 ? G3 : G2) : gcd(G2, G3);
        long long H4 = (G2 == 0 || G4 == 0) ? (G2 == 0 ? G4 : G2) : gcd(G2, G4);
        
        // Collect all potential divisors of these multiples.
        std::set<long long> candidates;
        find_divisors(H1, candidates, K);
        find_divisors(H2, candidates, K);
        find_divisors(H3, candidates, K);
        find_divisors(H4, candidates, K);

        std::vector<long long> sorted_candidates(candidates.begin(), candidates.end());
        
        // Test each candidate.
        for (long long d : sorted_candidates) {
            if (d <= K) continue;
            std::vector<long long> check_elements;
            for (int i = 1; i <= 11; ++i) {
                check_elements.push_back(d * i);
            }
            if (query(check_elements) == 55) { // 55 = 10+9+...+1
                answer(d);
                return 0;
            }
        }
    }

    return 0;
}