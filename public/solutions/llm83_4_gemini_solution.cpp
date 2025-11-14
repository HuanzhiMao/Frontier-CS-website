#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int n = 1000000;
    
    std::vector<int> spf(n + 1);
    std::iota(spf.begin(), spf.end(), 0);

    for (long long i = 2; i * i <= n; ++i) {
        if (spf[i] == i) { // i is a prime number
            for (long long j = i * i; j <= n; j += i) {
                if (spf[j] == j) { // Check if spf[j] is not set yet
                    spf[j] = i;
                }
            }
        }
    }
    
    std::vector<int> f(n + 1);
    f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        f[i] = -f[i / spf[i]];
    }
    
    for (int i = 1; i <= n; ++i) {
        std::cout << f[i] << (i == n ? "" : " ");
    }
    std::cout << "\n";
    
    return 0;
}