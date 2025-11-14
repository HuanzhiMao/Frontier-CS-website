#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    long long L = n + 1;
    for (int i = 1; i <= n; ++i) {
        std::cout << std::string(L, 'X') + std::string(i, 'O') << "\n";
    }
    std::cout << std::flush;

    int q;
    std::cin >> q;

    for (int j = 0; j < q; ++j) {
        long long p;
        std::cin >> p;

        long long M = p % L;
        long long Q = p / L;

        // We know M = max(u, v). There are two symmetric cases: u=M or v=M.
        // We test both and output the one that gives a valid integer solution
        // for the other variable in the range [1, n].
        // It can be shown that only one case (or both, if u=v) will yield a valid solution.

        // Case 1: v = M. This implies u <= M.
        // The formula for the quotient Q is: Q = 1 + max(u,v) + u + u*v
        // Substituting v=M and max(u,v)=M: Q = 1 + M + u + u*M = 1 + M + u(1+M)
        if ((Q - 1 - M) >= 0 && (M + 1) != 0 && (Q - 1 - M) % (M + 1) == 0) {
            long long u = (Q - 1 - M) / (M + 1);
            if (u >= 1 && u <= n && u <= M) {
                std::cout << u << " " << M << std::endl;
                continue;
            }
        }
        
        // Case 2: u = M. This implies v <= M.
        // Substituting u=M and max(u,v)=M: Q = 1 + M + M + M*v = 1 + 2*M + M*v
        if (M > 0 && (Q - 1 - 2 * M) >= 0 && (Q - 1 - 2 * M) % M == 0) {
            long long v = (Q - 1 - 2 * M) / M;
            if (v >= 1 && v <= n && v <= M) {
                std::cout << M << " " << v << std::endl;
                continue;
            }
        }
    }

    return 0;
}