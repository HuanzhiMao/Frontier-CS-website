#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

// Function to perform a walk operation
long long walk(long long x) {
    std::cout << "walk " << x << std::endl;
    long long result;
    std::cin >> result;
    if (result == -1) exit(0); // Judge may signal termination
    return result;
}

// Function to make a guess
void guess(long long g) {
    std::cout << "guess " << g << std::endl;
    exit(0);
}

// GCD for long long
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

// Function to get all divisors
std::vector<long long> get_divisors(long long k) {
    std::vector<long long> divs;
    if (k <= 0) return divs;
    for (long long i = 1; i * i <= k; ++i) {
        if (k % i == 0) {
            divs.push_back(i);
            if (i * i != k) {
                divs.push_back(k / i);
            }
        }
    }
    return divs;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int B = 32000;
    long long K = 0;
    
    std::map<long long, long long> visited;
    long long current_v;
    long long distance = 0;
    
    int collisions_found = 0;
    const int max_collisions = 20;

    // Baby steps
    for (int i = 1; i <= B; ++i) {
        current_v = walk(1);
        distance++;
        if (visited.count(current_v)) {
            long long multiple = distance - visited[current_v];
            if (K == 0) {
                K = multiple;
            } else {
                K = gcd(K, multiple);
            }
            collisions_found++;
        }
        visited[current_v] = distance;
        if (collisions_found >= max_collisions) {
            break;
        }
    }

    // Giant steps, if needed
    if (collisions_found < max_collisions) {
        // With B=32000, max n=10^9, n/B is approx 31250.
        // giant_steps_limit should be slightly larger than this to be safe.
        int giant_steps_limit = B + 50; 
        for (int i = 0; i < giant_steps_limit; ++i) {
            current_v = walk(B);
            distance += B;
            if (visited.count(current_v)) {
                long long multiple = distance - visited[current_v];
                if (K == 0) {
                    K = multiple;
                } else {
                    K = gcd(K, multiple);
                }
                collisions_found++;
                if (collisions_found >= max_collisions) {
                    break;
                }
            }
        }
    }
    
    // Divisor check
    std::vector<long long> divs = get_divisors(K);
    std::sort(divs.begin(), divs.end());

    long long v_check_start = current_v;
    long long dist_walked_in_check = 0;

    for (long long d : divs) {
        if (d == 0) continue;
        long long step = d - dist_walked_in_check;
        current_v = walk(step);
        dist_walked_in_check = d;
        if (current_v == v_check_start) {
            guess(d);
        }
    }
    
    // Fallback: If no divisor worked, it's possible n=K. This is unlikely
    // to be reached if logic is correct, but serves as a safeguard.
    if (K > 0) {
        guess(K);
    }

    return 0;
}