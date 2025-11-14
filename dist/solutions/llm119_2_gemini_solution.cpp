#include <iostream>
#include <vector>
#include <string>
#include <numeric>

long long power(long long base, long long exp) {
    long long res = 1;
    long long M = 1000000007;
    base %= M;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % M;
        base = (base * base) % M;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, 1000000007 - 2);
}

long long query(int n, const std::vector<long long>& a) {
    std::cout << "?";
    for (int i = 0; i <= n; ++i) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;
    long long result;
    std::cin >> result;
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    if (n == 0) {
        std::cout << "!" << std::endl;
        return 0;
    }

    std::vector<long long> a_base(n + 1, 2);
    long long r_base = query(n, a_base);

    std::vector<long long> diffs(n + 1);
    for (int k = 1; k <= n; ++k) {
        std::vector<long long> a_k = a_base;
        a_k[k] = 3;
        diffs[k] = (query(n, a_k) - r_base + 1000000007) % 1000000007;
    }
    
    std::vector<int> b_final(n);
    b_final[0] = 0; // Assume op_1 is + (b_1 = 0)
    
    std::vector<long long> C(n + 1);
    C[1] = 2;

    for (int k = 1; k < n; ++k) {
        long long ratio = (diffs[k] * modInverse(diffs[k+1])) % 1000000007;
        
        long long Ck = C[k];
        
        long long ratio_if_plus_star = (2 * modInverse((Ck + 2) % 1000000007)) % 1000000007;
        long long ratio_if_star_plus = Ck;
        
        if (ratio == 1) { // op_k and op_{k+1} are the same
            b_final[k] = b_final[k-1];
        } else { // op_k and op_{k+1} are different
            b_final[k] = 1 - b_final[k-1];
        }

        // Update C for next iteration
        if (b_final[k-1] == 0) { // op_k is +
            C[k+1] = (C[k] + 2) % 1000000007;
        } else { // op_k is *
            C[k+1] = (C[k] * 2) % 1000000007;
        }
    }
    
    // Now we have a candidate op sequence (assuming op_1 = +). Let's check it with D_n.
    long long Cn;
    if (n > 0) {
        Cn = C[n];
    } else { // n=0 case already handled, but for safety
        Cn = 2;
    }

    long long Dn_val_if_assumption_correct;
    if (b_final[n-1] == 0) { // op_n is +
        Dn_val_if_assumption_correct = 1;
    } else { // op_n is *
        Dn_val_if_assumption_correct = Cn;
    }
    
    if (Dn_val_if_assumption_correct != diffs[n]) {
        // Our assumption was wrong. op_1 is *. Flip all bits.
        for (int i=0; i<n; ++i) {
            b_final[i] = 1 - b_final[i];
        }
    }

    std::cout << "!";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << b_final[i];
    }
    std::cout << std::endl;

    return 0;
}