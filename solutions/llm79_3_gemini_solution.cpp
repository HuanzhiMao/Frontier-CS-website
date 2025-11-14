#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>
#include <algorithm>

using namespace std;

typedef unsigned long long ull;
typedef __int128_t int128;

ull n;

// The bits(x) function calculates ceil(log2(x+1)).
// For a 64-bit integer x > 0, this is equivalent to floor(log2(x)) + 1,
// which can be computed as 64 - __builtin_clzll(x).
int bits(ull x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

// Overload for __int128_t for intermediate products
int bits(int128 x) {
    if (x == 0) return 0;
    ull high = x >> 64;
    if (high > 0) {
        return 64 + (64 - __builtin_clzll(high));
    }
    ull low = x;
    return 64 - __builtin_clzll(low);
}

// Issues a query to the system
ull query(ull a) {
    cout << "? " << a << endl;
    ull time;
    cin >> time;
    return time;
}

// Simulates the device's computation to calculate the theoretical time for a given a and d
ull calculate_time(ull a_in, ull d) {
    int128 r = 1;
    int128 a = a_in;
    ull total_time = 0;

    for (int i = 0; i < 60; ++i) {
        if ((d >> i) & 1) {
            total_time += (ull)(bits(r) + 1) * (bits(a) + 1);
            r = r * a % n;
        }
        total_time += (ull)(bits(a) + 1) * (bits(a) + 1);
        a = a * a % n;
    }
    return total_time;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    // Seed a random number generator
    ull seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937_64 rng(seed);

    // d is coprime to m=(p-1)(q-1), which is even. So d must be odd.
    // This means the least significant bit is 1.
    ull d = 1;

    // Determine bits d_1 to d_59
    for (int k = 1; k < 60; ++k) {
        long double score0 = 0;
        long double score1 = 0;
        
        // Number of random queries to determine one bit.
        int queries_per_bit = 50;

        for (int j = 0; j < queries_per_bit; ++j) {
            uniform_int_distribution<ull> distrib(2, n - 1);
            ull a_val = distrib(rng);

            ull T_obs = query(a_val);
            
            // Hypothesis 1: d_k = 0. Candidate d has determined bits up to k-1.
            ull d0_candidate = d;
            // Hypothesis 2: d_k = 1. Candidate d has determined bits up to k-1, plus d_k=1.
            ull d1_candidate = d | (1ULL << k);
            
            // Predicted time for each hypothesis, assuming higher bits are 0.
            ull T_pred0 = calculate_time(a_val, d0_candidate);
            ull T_pred1 = calculate_time(a_val, d1_candidate);
            
            // The score is the sum of absolute differences between observed and predicted times.
            score0 += fabsl((long double)T_obs - T_pred0);
            score1 += fabsl((long double)T_obs - T_pred1);
        }

        // The hypothesis with the lower score is more likely correct.
        if (score1 < score0) {
            d |= (1ULL << k);
        }
    }

    cout << "! " << d << endl;

    return 0;
}