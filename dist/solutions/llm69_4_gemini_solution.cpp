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

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            std::cout << 'X';
        }
        std::cout << 'O' << '\n';
    }
    std::cout.flush();

    int q;
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
        long long p;
        std::cin >> p;

        for (int u = 1; u <= n; ++u) {
            bool found = false;

            // Case 1: v = u
            // Power = u^2 + 4u + 2
            if (p == (long long)u * u + 4LL * u + 2) {
                std::cout << u << " " << u << '\n';
                std::cout.flush();
                found = true;
            }
            if (found) break;

            // Case 2: v < u
            // Power = u*v + v + 3u + 2  => v(u+1) = p - 3u - 2
            if (u > 1) {
                long long num_v_lt = p - 3LL * u - 2;
                long long den_v_lt = u + 1;
                if (num_v_lt > 0 && num_v_lt % den_v_lt == 0) {
                    long long v = num_v_lt / den_v_lt;
                    if (v >= 1 && v < u) {
                        std::cout << u << " " << v << '\n';
                        std::cout.flush();
                        found = true;
                    }
                }
            }
            if (found) break;
            
            // Case 3: v > u
            // Power = u*v + 2v + 2u + 2 => v(u+2) = p - 2u - 2
            if (u < n) {
                long long num_v_gt = p - 2LL * u - 2;
                long long den_v_gt = u + 2;
                if (num_v_gt > 0 && num_v_gt % den_v_gt == 0) {
                    long long v = num_v_gt / den_v_gt;
                    if (v > u && v <= n) {
                        std::cout << u << " " << v << '\n';
                        std::cout.flush();
                        found = true;
                    }
                }
            }
            if (found) break;
        }
    }

    return 0;
}