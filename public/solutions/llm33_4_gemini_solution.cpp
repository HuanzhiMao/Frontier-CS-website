#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

void solve();
void process_single_case();
void build_sieve();
std::vector<int> get_binary_perm(long long k);

const int SIEVE_LIMIT = 1000001;
std::vector<int> primes;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}

void solve() {
    build_sieve();
    int q;
    std::cin >> q;
    while (q--) {
        process_single_case();
    }
}

void build_sieve() {
    std::vector<bool> is_prime_sieve(SIEVE_LIMIT, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (int p = 2; p * p < SIEVE_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            for (int i = p * p; i < SIEVE_LIMIT; i += p)
                is_prime_sieve[i] = false;
        }
    }
    for (int p = 2; p < SIEVE_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            primes.push_back(p);
        }
    }
}

std::vector<int> get_binary_perm(long long k) {
    if (k <= 1) {
        return {};
    }

    std::vector<char> ops;
    long long temp_k = k;
    while (temp_k > 1) {
        if (temp_k % 2 == 0) {
            ops.push_back('D'); // Represents *2 operation
            temp_k /= 2;
        } else {
            ops.push_back('I'); // Represents +1 operation
            temp_k -= 1;
        }
    }
    std::reverse(ops.begin(), ops.end());

    std::vector<int> p;
    int val = 0;
    for (char op : ops) {
        if (op == 'D') {
            p.push_back(val);
        } else { // 'I'
            p.insert(p.begin(), val);
        }
        val++;
    }
    return p;
}

void process_single_case() {
    long long k;
    std::cin >> k;

    std::vector<long long> factors;
    long long temp_k = k;

    for (int p : primes) {
        if ((long long)p * p > temp_k) {
            break;
        }
        while (temp_k % p == 0) {
            factors.push_back(p);
            temp_k /= p;
        }
    }
    if (temp_k > 1) {
        factors.push_back(temp_k);
    }

    std::vector<int> final_p;
    int current_offset = 0;

    for (long long f : factors) {
        std::vector<int> p_f = get_binary_perm(f);
        for (int &val : p_f) {
            val += current_offset;
        }
        final_p.insert(final_p.end(), p_f.begin(), p_f.end());
        current_offset += p_f.size();
    }

    std::cout << final_p.size() << "\n";
    for (int i = 0; i < final_p.size(); ++i) {
        std::cout << final_p[i] << (i == final_p.size() - 1 ? "" : " ");
    }
    std::cout << "\n";
}