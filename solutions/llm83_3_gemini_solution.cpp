#include <iostream>
#include <vector>
#include <numeric>

const int N = 1000000;

int spf[N + 1];
int f[N + 1];

void sieve() {
    std::iota(spf, spf + N + 1, 0);
    for (int i = 2; i * i <= N; ++i) {
        if (spf[i] == i) { // i is a prime
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();

    f[1] = 1;
    long long current_sum = 1;

    for (int i = 2; i <= N; ++i) {
        int p = spf[i];
        if (p == i) { // i is a prime
            if (current_sum >= 0) {
                f[i] = -1;
            } else {
                f[i] = 1;
            }
        } else { // i is composite
            f[i] = f[p] * f[i / p];
        }
        current_sum += f[i];
    }

    for (int i = 1; i <= N; ++i) {
        std::cout << f[i] << (i == N ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}