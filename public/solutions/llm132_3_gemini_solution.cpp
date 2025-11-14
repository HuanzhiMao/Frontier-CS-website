#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int R_in, H_in;
    std::cin >> R_in >> H_in;

    int num_robots = 26;
    int weight = 13;

    std::vector<int> codes;
    codes.reserve(1000);
    int comb = (1 << weight) - 1;
    for (int i = 0; i < 1000; ++i) {
        codes.push_back(comb);
        int u = comb & -comb;
        int v = comb + u;
        if (v == 0) break; // Avoid overflow issues if n=32
        comb = v + (((comb ^ v) / u) >> 2);
    }
    
    std::vector<std::vector<int>> queries(num_robots);
    for (int p = 1; p <= 1000; ++p) {
        for (int i = 0; i < num_robots; ++i) {
            if ((codes[p - 1] >> i) & 1) {
                queries[i].push_back(p);
            }
        }
    }

    for (int i = 0; i < num_robots; ++i) {
        std::cout << "? " << queries[i].size();
        for (int pos : queries[i]) {
            std::cout << " " << pos;
        }
        std::cout << std::endl;
    }

    std::cout << "@" << std::endl;

    int L;
    std::cin >> L;
    int result_mask = 0;
    for (int i = 0; i < L; ++i) {
        int bit;
        std::cin >> bit;
        if (bit == 1) {
            result_mask |= (1 << i);
        }
    }

    int c1 = -1, c2 = -1;
    
    int result_weight = 0;
#if defined(__GNUC__) || defined(__clang__)
    result_weight = __builtin_popcount(result_mask);
#else
    for(int i = 0; i < num_robots; ++i) if((result_mask >> i) & 1) result_weight++;
#endif

    if (result_weight == weight) {
      for (int p = 1; p <= 1000; ++p) {
          if (codes[p-1] == result_mask) {
              c1 = p;
              c2 = p;
              break;
          }
      }
    }

    if (c1 == -1) {
        bool found = false;
        for (int p1 = 1; p1 <= 1000; ++p1) {
            if ((codes[p1-1] & result_mask) != codes[p1-1]) continue;
            for (int p2 = p1 + 1; p2 <= 1000; ++p2) {
                if ((codes[p1 - 1] | codes[p2 - 1]) == result_mask) {
                    c1 = p1;
                    c2 = p2;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    std::cout << "! " << c1 << " " << c2 << std::endl;

    return 0;
}