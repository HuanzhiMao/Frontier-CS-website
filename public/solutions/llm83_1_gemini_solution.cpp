#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> spf(n + 1);
    std::iota(spf.begin(), spf.end(), 0);

    for (int i = 2; i * i <= n; ++i) {
        if (spf[i] == i) { // i is a prime
            for (int j = i * i; j <= n; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    std::vector<int> f(n + 1);
    f[1] = 1;
    long long current_sum = 1;

    for (int i = 2; i <= n; ++i) {
        if (spf[i] == i) { // i is a prime
            if (current_sum >= 0) {
                f[i] = -1;
            } else {
                f[i] = 1;
            }
        } else { // i is composite
            f[i] = f[spf[i]] * f[i / spf[i]];
        }
        current_sum += f[i];
    }

    for (int i = 1; i <= n; ++i) {
        std::cout << f[i] << (i == n ? "" : " ");
    }
    std::cout << '\n';

    return 0;
}