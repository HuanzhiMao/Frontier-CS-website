#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <numeric>

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> mistakes(n, 0);
    std::mt19937 rng(42); // Use a fixed seed for deterministic random-like behavior

    for (int k = 0; k < m; ++k) {
        std::string s;
        std::cin >> s;

        int my_pred;

        if (k == 0) {
            int ones = 0;
            for (char c : s) {
                if (c == '1') {
                    ones++;
                }
            }
            if (ones * 2 > n) {
                my_pred = 1;
            } else if (ones * 2 < n) {
                my_pred = 0;
            } else {
                std::uniform_int_distribution<int> dist(0, 1);
                my_pred = dist(rng);
            }
        } else {
            int rounds_played = k;
            
            int min_mistakes = k + 1;
            for (int i = 0; i < n; ++i) {
                min_mistakes = std::min(min_mistakes, mistakes[i]);
                min_mistakes = std::min(min_mistakes, rounds_played - mistakes[i]);
            }

            int vote0 = 0;
            int vote1 = 0;
            for (int i = 0; i < n; ++i) {
                if (mistakes[i] == min_mistakes) {
                    if (s[i] == '0') vote0++;
                    else vote1++;
                }
                if (rounds_played - mistakes[i] == min_mistakes) {
                    if (s[i] == '0') vote1++;
                    else vote0++;
                }
            }

            if (vote0 > vote1) {
                my_pred = 0;
            } else if (vote1 > vote0) {
                my_pred = 1;
            } else {
                std::uniform_int_distribution<int> dist(0, 1);
                my_pred = dist(rng);
            }
        }

        std::cout << my_pred << std::endl;

        int outcome;
        std::cin >> outcome;
        for (int i = 0; i < n; ++i) {
            if ((s[i] - '0') != outcome) {
                mistakes[i]++;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}