#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> mistakes(n, 0);

    for (int t = 1; t <= m; ++t) {
        std::string s;
        std::cin >> s;

        int min_m = mistakes[0];
        int max_m = mistakes[0];
        if (n > 1) {
            for (int i = 1; i < n; ++i) {
                if (mistakes[i] < min_m) min_m = mistakes[i];
                if (mistakes[i] > max_m) max_m = mistakes[i];
            }
        }
        int min_mistakes_val = std::min(min_m, (t - 1) - max_m);

        int votes0 = 0;
        int votes1 = 0;

        for (int i = 0; i < n; ++i) {
            // Vote from participant i if they are among the best
            if (mistakes[i] == min_mistakes_val) {
                if (s[i] == '0') votes0++;
                else votes1++;
            }
            // Vote from anti-participant i if they are among the best
            if ((t - 1) - mistakes[i] == min_mistakes_val) {
                if (s[i] == '0') votes1++;
                else votes0++;
            }
        }
        
        char my_guess;
        if (votes1 >= votes0) {
            my_guess = '1';
        } else {
            my_guess = '0';
        }

        std::cout << my_guess << std::endl;

        char outcome;
        std::cin >> outcome;

        for (int i = 0; i < n; ++i) {
            if (s[i] != outcome) {
                mistakes[i]++;
            }
        }
    }

    return 0;
}