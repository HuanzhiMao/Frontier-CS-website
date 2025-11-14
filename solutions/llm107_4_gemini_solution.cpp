#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

void solve() {
    long long d_known = 1;
    
    std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313};

    for (int p : primes) {
        long long q = p;
        // Find the smallest power of p that is greater than 10^9
        // This is done by first finding the largest power <= 10^9 and then multiplying by p once more.
        // The check q <= 1000000000LL / p prevents overflow.
        while (q <= 1000000000LL / p) {
            q *= p;
        }
        q *= p;
        
        std::cout << "0 " << q << std::endl;
        long long response;
        std::cin >> response;
        
        int count = 0;
        if (response > 0) {
            while (response % p == 0) {
                response /= p;
                count++;
            }
        }
        d_known *= (count + 1);
    }
    
    std::cout << "1 " << 2 * d_known << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}