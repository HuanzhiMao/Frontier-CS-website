#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int n;
vector<bool> in_g1;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (static_cast<__int128>(res) * base) % mod;
        base = (static_cast<__int128>(base) * base) % mod;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n, long long mod) {
    return power(n, mod - 2, mod);
}

int ask(const vector<int>& indices) {
    cout << "? " << indices.size();
    for (int index : indices) {
        cout << " " << index;
    }
    cout << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const vector<int>& p) {
    cout << "!";
    for (int val : p) {
        cout << " " << val;
    }
    cout << endl;
}

long long extendedGcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long long x1, y1;
    long long gcd = extendedGcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

long long modInverseCRT(long long a, long long m) {
    long long x, y;
    long long g = extendedGcd(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

long long get_q_sum_mod_p(const vector<int>& indices) {
    int res = ask(indices);
    if (res == 0) return -1LL;
    
    long long k = indices.size();
    long long k1 = 0;
    for (int idx : indices) {
        if (in_g1[idx]) {
            k1++;
        }
    }
    long long inv2 = (k + 1) / 2; // k is odd prime
    return (k1 * inv2) % k;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    in_g1.resize(n + 1, false);
    vector<int> g1_indices, g2_indices;
    g1_indices.push_back(1);
    in_g1[1] = true;

    for (int i = 2; i <= n; ++i) {
        if (ask({1, i}) == 1) {
            g1_indices.push_back(i);
            in_g1[i] = true;
        } else {
            g2_indices.push_back(i);
        }
    }

    vector<int> primes;
    if (n > 2) primes.push_back(3);
    if (n > 6) primes.push_back(5);
    if (n > 30) primes.push_back(7);
    if (n > 210) primes.push_back(11);
    
    vector<long long> D(n + 1, 0);
    long long M = 1;
    
    for (int p : primes) {
        vector<long long> q_mod_p(n + 1, 0);
        
        auto find_sum = [&](const vector<int>& query_indices) {
            vector<int> current_query = query_indices;
            vector<bool> used(n + 1, false);
            for(int idx : query_indices) used[idx] = true;

            long long res = -1;
            int retries = 0;
            while(res == -1 && retries < 20) {
                res = get_q_sum_mod_p(current_query);
                if (res == -1) {
                     int basis_idx_to_change = uniform_int_distribution<int>(0, p - 3)(rng);
                     int old_val = current_query[basis_idx_to_change];
                     used[old_val] = false;
                     int new_val;
                     do {
                        new_val = uniform_int_distribution<int>(1, n)(rng);
                     } while(used[new_val]);
                     current_query[basis_idx_to_change] = new_val;
                     used[new_val] = true;
                }
                retries++;
            }
            return res;
        };

        vector<int> basis;
        vector<bool> basis_used(n + 1, false);
        basis_used[1] = basis_used[2] = true;
        for (int i = 3; i <= n && basis.size() < p - 2; ++i) {
            basis.push_back(i);
            basis_used[i] = true;
        }

        vector<int> base_query = basis; base_query.push_back(1); base_query.push_back(2);
        long long v12 = find_sum(base_query);

        for (int i = 3; i <= n; ++i) {
            vector<int> current_query = basis; current_query.push_back(1); current_query.push_back(i);
            long long v1i = find_sum(current_query);
            q_mod_p[i] = (v1i - v12 + p) % p;
        }
        
        vector<int> q23_query = basis; q23_query.push_back(2); q23_query.push_back(3);
        long long v23 = find_sum(q23_query);
        
        vector<int> q13_query = basis; q13_query.push_back(1); q13_query.push_back(3);
        long long v13 = find_sum(q13_query);
        
        q_mod_p[1] = (v13 - v23 + p) % p;

        long long q2_minus_q1 = (p - q_mod_p[1]) % p;
        for (int i = 1; i <= n; ++i) {
            long long current_rem = (q_mod_p[i] + q2_minus_q1) % p;
            
            long long old_D = D[i];
            long long old_M = M;
            long long inv = modInverseCRT(old_M, p);
            long long term = (((current_rem - old_D % p + p) % p) * inv) % p;
            D[i] = old_D + term * old_M;
        }
        M *= p;
    }

    int q1_val = -1;
    for (int q1_cand = 0; q1_cand < n / 2; ++q1_cand) {
        vector<int> q_vals_g1, q_vals_g2;
        bool ok = true;

        for (int idx : g1_indices) {
            long long val = (q1_cand + D[idx]) % M;
            if (val >= n / 2) { ok = false; break; }
            q_vals_g1.push_back(val);
        }
        if (!ok) continue;

        for (int idx : g2_indices) {
            long long val = (q1_cand + D[idx]) % M;
            if (val >= n / 2) { ok = false; break; }
            q_vals_g2.push_back(val);
        }
        if (!ok) continue;

        sort(q_vals_g1.begin(), q_vals_g1.end());
        sort(q_vals_g2.begin(), q_vals_g2.end());

        bool match = true;
        for (int i = 0; i < n / 2; ++i) {
            if (q_vals_g1[i] != i || q_vals_g2[i] != i) {
                match = false;
                break;
            }
        }
        
        if (match) {
            q1_val = q1_cand;
            break;
        }
    }

    vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i) {
        long long q_val = (q1_val + D[i]) % M;
        if (in_g1[i]) {
            p[i] = 2 * q_val + 1;
        } else {
            p[i] = 2 * q_val + 2;
        }
    }

    if (p[1] > n / 2) {
        for (int i = 1; i <= n; ++i) {
            p[i] = n + 1 - p[i];
        }
    }

    vector<int> final_p(p.begin() + 1, p.end());
    answer(final_p);

    return 0;
}