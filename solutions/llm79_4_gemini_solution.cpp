#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>

// Use __int128 for modular multiplication to prevent overflow
unsigned long long n_ull;

unsigned long long mul(unsigned long long a, unsigned long long b) {
    return ((unsigned __int128)a * b) % n_ull;
}

// bits(x) = ceil(log2(x+1))
int bits(unsigned long long x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

long long query(unsigned long long a) {
    std::cout << "? " << a << std::endl;
    long long time;
    std::cin >> time;
    return time;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n_ull;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long long> distrib(2, n_ull - 1);

    unsigned long long d = 0;

    int M = 400;

    std::vector<unsigned long long> a_samples(M);
    std::vector<long long> T_responses(M);

    for (int k = 0; k < 60; ++k) {
        for (int j = 0; j < M; ++j) {
            a_samples[j] = distrib(rng);
        }

        for (int j = 0; j < M; ++j) {
            T_responses[j] = query(a_samples[j]);
        }
        
        std::vector<__int128> R(M);
        std::vector<__int128> P(M);

        for (int j = 0; j < M; ++j) {
            std::vector<unsigned long long> a_at_iter(60);
            
            a_at_iter[0] = a_samples[j];
            for (int i = 1; i < 60; ++i) {
                a_at_iter[i] = mul(a_at_iter[i - 1], a_at_iter[i - 1]);
            }

            __int128 sum_sq_cost = 0;
            for(int i = 0; i < 60; ++i) {
                sum_sq_cost += (__int128)(bits(a_at_iter[i]) + 1) * (bits(a_at_iter[i]) + 1);
            }
            
            __int128 sum_known_d_cost = 0;
            unsigned long long r_k = 1;
            for (int i = 0; i < k; ++i) {
                if ((d >> i) & 1) {
                    sum_known_d_cost += (__int128)(bits(r_k) + 1) * (bits(a_at_iter[i]) + 1);
                    r_k = mul(r_k, a_at_iter[i]);
                }
            }

            R[j] = T_responses[j] - sum_sq_cost - sum_known_d_cost;
            P[j] = (__int128)(bits(r_k) + 1) * (bits(a_at_iter[k]) + 1);
        }

        __int128 sum_P_R = 0;
        __int128 sum_P2 = 0;
        
        for (int j = 0; j < M; ++j) {
            sum_P_R += P[j] * R[j];
            sum_P2 += P[j] * P[j];
        }
        
        if (sum_P2 > 0 && sum_P_R * 2 > sum_P2) {
             d |= (unsigned long long)1 << k;
        }
    }

    std::cout << "! " << d << std::endl;

    return 0;
}