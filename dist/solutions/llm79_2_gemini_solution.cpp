#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <numeric>
#include <algorithm>

// Use __int128_t for numbers that can exceed 2^64
using u128 = unsigned __int128;

// Custom I/O for u128
std::istream& operator>>(std::istream& is, u128& val) {
    std::string s;
    is >> s;
    val = 0;
    for (char c : s) {
        val = val * 10 + (c - '0');
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const u128& val) {
    if (val == 0) return os << "0";
    std::string s = "";
    u128 tmp = val;
    while (tmp > 0) {
        s += (tmp % 10) + '0';
        tmp /= 10;
    }
    std::reverse(s.begin(), s.end());
    return os << s;
}

// Computes number of bits in x without leading zeros
int bits(u128 x) {
    if (x == 0) return 0;
    unsigned long long hi = x >> 64;
    if (hi != 0) {
        return 64 + (64 - __builtin_clzll(hi));
    }
    unsigned long long lo = x;
    return 64 - __builtin_clzll(lo);
}

// Simulates the device to calculate total time for a given a and d
long long calculate_time(u128 a, u128 d, u128 n) {
    long long total_time = 0;
    u128 r = 1;
    for (int i = 0; i < 60; ++i) {
        if (((d >> i) & 1) != 0) {
            total_time += (long long)(bits(r) + 1) * (bits(a) + 1);
            r = (r * a) % n;
        }
        total_time += (long long)(bits(a) + 1) * (bits(a) + 1);
        a = (a * a) % n;
    }
    return total_time;
}

// Gets the intermediate values a_k and r_k needed for signal calculation
void get_signal_values(u128 a, u128 d, u128 n, int k, u128& ak, u128& rk) {
    u128 r = 1;
    for (int i = 0; i < k; ++i) {
        if (((d >> i) & 1) != 0) {
            r = (r * a) % n;
        }
        a = (a * a) % n;
    }
    ak = a;
    rk = r;
}

// Calculates variance of a vector of long longs using long double for precision
long double variance(const std::vector<long long>& v) {
    if (v.size() <= 1) return 0.0;
    long double sum = 0;
    long double sum_sq = 0;
    for (long long val : v) {
        sum += val;
        sum_sq += (long double)val * val;
    }
    long double mean = sum / v.size();
    return sum_sq / v.size() - mean * mean;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    u128 n;
    std::cin >> n;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    
    auto rand_u128 = [&]() {
        u128 r = rng();
        r = (r << 64) | rng();
        return r;
    };
    
    // From problem statement: d is coprime with m=(p-1)(q-1).
    // Since p,q > 2, p-1 and q-1 are even, so m is even.
    // Thus, d must be odd, so its 0-th bit is 1.
    u128 d = 1;
    
    const int SAMPLES_PER_BIT = 300;

    for (int k = 1; k < 60; ++k) {
        std::vector<long long> deltas;
        std::vector<long long> signals;

        for (int i = 0; i < SAMPLES_PER_BIT; ++i) {
            u128 a;
            do {
                a = rand_u128() % n;
            } while (a <= 1);
            
            std::cout << "? " << a << std::endl;
            long long observed_time;
            std::cin >> observed_time;

            long long pred_time_0 = calculate_time(a, d, n);
            deltas.push_back(observed_time - pred_time_0);

            u128 ak, rk;
            get_signal_values(a, d, n, k, ak, rk);
            signals.push_back((long long)(bits(ak) + 1) * (bits(rk) + 1));
        }

        std::vector<long long> deltas_minus_signals(SAMPLES_PER_BIT);
        for(int i = 0; i < SAMPLES_PER_BIT; ++i) {
            deltas_minus_signals[i] = deltas[i] - signals[i];
        }

        long double var0 = variance(deltas);
        long double var1 = variance(deltas_minus_signals);

        if (var1 < var0) {
            d |= (u128)1 << k;
        }
    }

    std::cout << "! " << d << std::endl;

    return 0;
}