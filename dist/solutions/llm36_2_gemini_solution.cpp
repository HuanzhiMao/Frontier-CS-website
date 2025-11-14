#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <cmath>
#include <algorithm>

long long combinations(long long n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }
    if (k > n / 2) {
        k = n - k;
    }
    if (k == 2) {
        return n * (n - 1) / 2;
    }
    return 0; // Not needed for k > 2
}

long long calculate_collisions(int d, int k) {
    if (d == 0) return -1; // Should not happen
    if (d > k) return 0;
    long long q = k / d;
    long long rem = k % d;
    long long collisions = rem * combinations(q + 1, 2) + (d - rem) * combinations(q, 2);
    return collisions;
}

int vp(long long n, int p) {
    if (n == 0) return 1e9; // Or some large value
    int count = 0;
    while (n > 0 && n % p == 0) {
        count++;
        n /= p;
    }
    return count;
}

long long power(long long base, int exp) {
    unsigned __int128 res = 1;
    unsigned __int128 b = base;
    while (exp > 0) {
        if (exp % 2 == 1) res *= b;
        b *= b;
        exp /= 2;
    }
    return (long long)res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int K = 400;
    std::map<long long, std::vector<int>> coll_to_d;
    for (int d = 1; d <= K; ++d) {
        coll_to_d[calculate_collisions(d, K)].push_back(d);
    }
    
    std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    std::vector<int> powers = {30, 19, 13, 11, 9, 8, 7, 7, 6, 6, 6};
    
    std::vector<long long> M;
    for (size_t i = 0; i < primes.size(); ++i) {
        M.push_back(power(primes[i], powers[i]));
    }

    std::vector<std::vector<int>> D(primes.size());
    bool all_zero_collisions = true;
    for (size_t i = 0; i < primes.size(); ++i) {
        std::cout << "0 " << K;
        for (int j = 1; j <= K; ++j) {
            std::cout << " " << (unsigned __int128)j * M[i];
        }
        std::cout << std::endl;
        long long collisions;
        std::cin >> collisions;
        if (collisions != 0) {
            all_zero_collisions = false;
        }
        if (coll_to_d.count(collisions)) {
            D[i] = coll_to_d[collisions];
        }
    }

    if (all_zero_collisions) {
        // This case suggests n has no prime factors from our list and n > K
        // Test for larger prime factors.
        std::vector<int> large_primes;
        for (int p = 37; p <= K; ++p) {
             bool is_prime = true;
             for (int j = 2; j * j <= p; ++j) {
                 if (p % j == 0) { is_prime = false; break; }
             }
             if (is_prime) large_primes.push_back(p);
        }

        for (int p : large_primes) {
            std::cout << "0 " << K;
            for (int j = 1; j <= K; ++j) {
                std::cout << " " << (long long)j * p;
            }
            std::cout << std::endl;
            long long collisions;
            std::cin >> collisions;
            if (collisions > 0) {
                // p divides n, and n/p <= K
                std::vector<int> d_cands = coll_to_d[collisions];
                for (int d : d_cands) {
                    std::cout << "1 " << (long long)p * d << std::endl;
                    return 0;
                }
            }
        }
        // Fallback for n being a large prime, or product of large primes.
        std::cout << "1 " << 401 << std::endl;
        return 0;
    }


    std::vector<long long> candidates;
    for (int d0 : D[0]) {
        if (vp(d0, primes[0]) != 0) continue;
        for (int d1 : D[1]) {
            if (vp(d1, primes[1]) != 0) continue;

            bool consistent = true;
            for (size_t j = 2; j < primes.size(); ++j) {
                if (vp(d0, primes[j]) != vp(d1, primes[j])) {
                    consistent = false;
                    break;
                }
            }
            if (!consistent) continue;

            std::vector<int> e(primes.size());
            e[0] = vp(d1, primes[0]);
            e[1] = vp(d0, primes[1]);
            for (size_t j = 2; j < primes.size(); ++j) {
                e[j] = vp(d0, primes[j]);
            }

            unsigned __int128 n_known_part = 1;
            for (size_t j = 0; j < primes.size(); ++j) {
                for (int p = 0; p < e[j]; ++p) n_known_part *= primes[j];
            }

            unsigned __int128 n_without_p0 = 1;
            if (e[0] > 0) n_without_p0 = n_known_part / power(primes[0], e[0]);
            else n_without_p0 = n_known_part;

            if (d0 % (long long)n_without_p0 != 0) continue;
            long long m = d0 / (long long)n_without_p0;
            
            unsigned __int128 n_without_p1 = 1;
            if (e[1] > 0) n_without_p1 = n_known_part / power(primes[1], e[1]);
            else n_without_p1 = n_known_part;

            if (d1 % (long long)n_without_p1 != 0) continue;
            if (m != d1 / (long long)n_without_p1) continue;
            
            bool m_coprime = true;
            for(int p : primes) {
                if (m > 1 && m % p == 0) {
                    m_coprime = false;
                    break;
                }
            }
            if (!m_coprime) continue;
            
            unsigned __int128 n_cand_val = n_known_part * m;
            if (n_cand_val < 2 || n_cand_val > 1000000000) continue;
            long long n_cand = (long long) n_cand_val;

            bool fully_consistent = true;
            for (size_t i = 2; i < primes.size(); ++i) {
                unsigned __int128 p_i_pow_e_i = power(primes[i], e[i]);
                long long d_expected = n_cand / (long long)p_i_pow_e_i;

                bool found = false;
                for (int d_i : D[i]) {
                    if (d_i == d_expected) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    fully_consistent = false;
                    break;
                }
            }

            if (fully_consistent) {
                candidates.push_back(n_cand);
            }
        }
    }

    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
    
    if (candidates.empty()) {
        // Fallback for small n, not captured by the prime power method's assumptions
        std::cout << "0 " << K + 1;
        for (int i = 1; i <= K + 1; ++i) std::cout << " " << i;
        std::cout << std::endl;
        long long C;
        std::cin >> C;
        for (long long n_cand = 2; n_cand <= K; ++n_cand) {
            long long rem = (K + 1) % n_cand;
            long long q = (K + 1) / n_cand;
            if (rem == 0 && K+1 > 0) { // All numbers from 1 to n_cand have distinct remainders
                rem = n_cand;
                q--;
            }
            long long expected_C = rem * combinations(q + 1, 2) + (n_cand - rem) * combinations(q, 2);
            if (expected_C == C) {
                 std::cout << "1 " << n_cand << std::endl;
                 return 0;
            }
        }
    } else {
      std::cout << "1 " << candidates[0] << std::endl;
    }

    return 0;
}