#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <chrono>
#include <cstdlib>

using u64 = unsigned long long;
using i64 = long long;
using u128 = __int128;

// Function to read u64
u64 read_u64() {
    u64 val;
    std::cin >> val;
    return val;
}

// Function to print query
void print_query(u64 a) {
    std::cout << "? " << a << std::endl;
}

// Function to print answer
void print_answer(u64 d) {
    std::cout << "! " << d << std::endl;
}

// bits(x) = ceil(log2(x+1))
// for x=0, bits(0)=0
// for x > 0, bits(x) = floor(log2(x)) + 1 = 64 - clz(x) for 64-bit integers
int bits(u64 x) {
    if (x == 0) {
        return 0;
    }
    return 64 - __builtin_clzll(x);
}

i64 calculate_cost(u64 a_orig, u64 d, u64 n) {
    i64 total_cost = 0;
    u64 r = 1;
    u64 a = a_orig;
    for (int i = 0; i < 60; ++i) {
        if ((d >> i) & 1) {
            total_cost += (i64)(bits(r) + 1) * (bits(a) + 1);
            r = ((u128)r * a) % n;
        }
        total_cost += (i64)(bits(a) + 1) * (bits(a) + 1);
        a = ((u128)a * a) % n;
    }
    return total_cost;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    u64 n = read_u64();

    // d is coprime to m=(p-1)(q-1). Since p,q are odd primes, m is a multiple of 4.
    // Thus, d must be odd, so its least significant bit (d_0) is 1.
    u64 d = 1;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<u64> a_dist(2, n - 1);
    
    // We have 59 bits to find, and a generous query limit.
    // 30000 / 59 is about 508. We use 450 to be safe.
    int queries_per_bit = 450;

    for (int i = 1; i < 60; ++i) {
        int votes0 = 0;
        int votes1 = 0;
        
        for (int k = 0; k < queries_per_bit; ++k) {
            u64 a = a_dist(rng);
            print_query(a);
            i64 T_obs;
            std::cin >> T_obs;

            // Generate a random pattern for the unknown higher bits of d.
            u64 d_higher_bits_shifted = (rng() >> (i + 1)) << (i + 1);

            // Construct two hypotheses for d based on d_i = 0 or d_i = 1.
            // Both use the same known prefix (stored in d) and the same random higher bits.
            u64 d_hyp0 = d | d_higher_bits_shifted;
            u64 d_hyp1 = d | (1ULL << i) | d_higher_bits_shifted;

            i64 T0 = calculate_cost(a, d_hyp0, n);
            i64 T1 = calculate_cost(a, d_hyp1, n);

            if (std::abs(T_obs - T0) < std::abs(T_obs - T1)) {
                votes0++;
            } else {
                votes1++;
            }
        }

        if (votes1 > votes0) {
            d |= (1ULL << i);
        }
    }

    print_answer(d);

    return 0;
}