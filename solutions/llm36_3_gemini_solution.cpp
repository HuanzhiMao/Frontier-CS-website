#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

long long query(const std::vector<long long>& v) {
    if (v.empty()) {
        return 0;
    }
    std::cout << 0 << " " << v.size();
    for (long long x : v) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    long long collisions;
    std::cin >> collisions;
    return collisions;
}

long long query_range(long long k) {
    if (k <= 0) return 0;
    std::vector<long long> v(k);
    std::iota(v.begin(), v.end(), 1);
    return query(v);
}

void guess(long long n) {
    std::cout << 1 << " " << n << std::endl;
}

long long get_collisions(long long k, long long n) {
    if (n <= 1) return -1;
    if (k < n) {
        return 0;
    }
    long long q = k / n;
    long long r = k % n;
    long long c_q_plus_1 = q * (q + 1) / 2;
    long long c_q = q * (q - 1) / 2;
    return r * c_q_plus_1 + (n - r) * c_q;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long K = 250000;
    long long total_cost = 0;
    long long rem_budget = 1000000;

    auto do_query_range = [&](long long k) {
        total_cost += k;
        rem_budget -= k;
        return query_range(k);
    };

    long long cK = do_query_range(K);
    long long cK1 = do_query_range(K + 1);

    long long q = cK1 - cK;

    if (q > 0) {
        long long n_min = K / (q + 1) + 1;
        long long n_max = K / q;
        for (long long n = n_min; n <= n_max; ++n) {
            if (n <= 1) continue;
            if (get_collisions(K, n) == cK) {
                if (get_collisions(K + 1, n) == cK1) {
                    guess(n);
                    return 0;
                }
            }
        }
    } else {
        long long low = K + 2, high = 1000000000, first_k_coll = -1;

        while(low <= high) {
            long long mid = low + (high-low)/2;
            if (mid <= 0) { low = 1; continue; }
            if (mid > rem_budget) {
                high = mid-1;
                continue;
            }
            if(do_query_range(mid) > 0) {
                first_k_coll = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        if (first_k_coll != -1) {
            guess(first_k_coll-1);
        } else {
            guess(low-1);
        }
    }

    return 0;
}