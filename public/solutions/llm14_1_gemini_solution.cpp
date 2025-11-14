#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <numeric>
#include <algorithm>

// Function to issue a "walk" command to the judge.
// Input: a non-negative integer x (0 <= x <= 10^9).
// Effect: the token moves x steps forward along the cycle.
// Output: the label of the vertex reached after the move.
int walk(long long x) {
    std::cout << "walk " << x << std::endl;
    int v;
    std::cin >> v;
    return v;
}

// Function to issue a "guess" command to the judge.
// Input: an integer g (1 <= g <= 10^9).
// Effect: ends the interaction.
void guess(long long g) {
    std::cout << "guess " << g << std::endl;
}

// Function to find the prime factorization of a number n.
// Returns a map where keys are prime factors and values are their exponents.
std::map<long long, int> prime_factorize(long long n) {
    std::map<long long, int> factors;
    for (long long i = 2; i * i <= n; ++i) {
        while (n % i == 0) {
            factors[i]++;
            n /= i;
        }
    }
    if (n > 1) {
        factors[n]++;
    }
    return factors;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Baby-step giant-step parameter. B should be around sqrt(N_max).
    // N_max = 10^9, sqrt(N_max) is approx 31622. A slightly larger round number is fine.
    const int B = 32000;
    
    // Map to store visited vertices and the distance to reach them during baby steps.
    std::map<int, int> visited;
    
    // M will store a multiple of the cycle length n.
    long long M = 0;

    // --- Phase 1: Find a multiple M of n using Baby-Step Giant-Step ---

    // Phase 1a: Baby steps. Walk 1 step at a time for B times.
    bool multiple_found = false;
    for (int i = 1; i <= B; ++i) {
        int v = walk(1);
        if (visited.count(v)) {
            // Collision found during baby steps.
            // This happens if n is small (n <= B).
            M = i - visited[v];
            multiple_found = true;
            break;
        }
        visited[v] = i;
    }

    // Phase 1b: Giant steps. If no collision yet, walk B steps at a time.
    if (!multiple_found) {
        for (int j = 1; j <= B; ++j) {
            int v = walk(B);
            if (visited.count(v)) {
                // Collision between a giant step and a baby step.
                long long current_dist = (long long)B + (long long)j * B;
                long long prev_dist = visited[v];
                M = current_dist - prev_dist;
                break;
            }
        }
    }

    // --- Phase 2: Find n from its multiple M ---

    // Now, M is a multiple of n. We find n by "dividing out" prime factors of M.
    long long n_candidate = M;
    auto prime_factors_of_M = prime_factorize(M);

    for (auto const& [p, a] : prime_factors_of_M) {
        // For each prime factor p of M, try to remove it from our candidate for n.
        for (int i = 0; i < a; ++i) {
            long long test_d = n_candidate / p;
            
            // Check if test_d is a multiple of n.
            // A check involves moving the token, so we must reset its position afterwards
            // to ensure subsequent checks are independent.
            // We can use M, a known multiple of n, to reset the position.
            
            int v_curr = walk(0);
            int v_new = walk(test_d);
            
            // Reset position: walking M-test_d steps after walking test_d steps
            // results in a total walk of M steps, returning to the start of the check.
            walk(M - test_d);

            if (v_curr == v_new) {
                // test_d is a multiple of n. We can update our candidate.
                n_candidate /= p;
            } else {
                // test_d is not a multiple of n. No smaller multiple of p will work either.
                break;
            }
        }
    }
    
    // The final candidate is our answer for n.
    guess(n_candidate);

    return 0;
}