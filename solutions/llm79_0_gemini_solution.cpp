#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

using int128 = __int128_t;

long long n;

inline int bits(long long x) {
    if (x == 0) {
        return 0;
    }
    return 64 - __builtin_clzll(x);
}

long long calculate_time(long long a_in, long long d) {
    long long r = 1;
    long long a = a_in;
    long long total_time = 0;

    for (int i = 0; i < 60; ++i) {
        if ((d >> i) & 1) {
            total_time += (long long)(bits(r) + 1) * (bits(a) + 1);
            r = (long long)((int128)r * a % n);
        }
        total_time += (long long)(bits(a) + 1) * (bits(a) + 1);
        a = (long long)((int128)a * a % n);
    }
    return total_time;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<long long> distrib(2, n - 1);

    long long d_known = 1;

    for (int k = 1; k < 60; ++k) {
        long long d_guess_0 = d_known;
        long long d_guess_1 = d_known | (1LL << k);

        long double total_error_0 = 0;
        long double total_error_1 = 0;

        int num_samples = 450;
        
        for (int i = 0; i < num_samples; ++i) {
            long long a = distrib(rng);
            
            std::cout << "? " << a << std::endl;
            long long observed_time;
            std::cin >> observed_time;

            long long time_0 = calculate_time(a, d_guess_0);
            long long time_1 = calculate_time(a, d_guess_1);
            
            long double error_0 = (long double)observed_time - time_0;
            long double error_1 = (long double)observed_time - time_1;
            
            total_error_0 += error_0 * error_0;
            total_error_1 += error_1 * error_1;
        }

        if (total_error_0 < total_error_1) {
            // d_k is 0, d_known is correct
        } else {
            // d_k is 1
            d_known = d_guess_1;
        }
    }

    std::cout << "! " << d_known << std::endl;

    return 0;
}