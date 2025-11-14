#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

void solve() {
    int n;
    int m;
    std::cin >> n >> m;

    std::vector<int> mistakes(n, 0);
    std::vector<int> anti_mistakes(n, 0);

    for (int k = 0; k < m; ++k) {
        std::string s;
        std::cin >> s;

        int min_err = m + 1;

        for (int i = 0; i < n; ++i) {
            if (mistakes[i] < min_err) {
                min_err = mistakes[i];
            }
        }
        for (int i = 0; i < n; ++i) {
            if (anti_mistakes[i] < min_err) {
                min_err = anti_mistakes[i];
            }
        }
        
        int votes_for_0 = 0;
        int votes_for_1 = 0;

        for (int i = 0; i < n; ++i) {
            if (mistakes[i] == min_err) {
                if (s[i] == '0') {
                    votes_for_0++;
                } else {
                    votes_for_1++;
                }
            }
            if (anti_mistakes[i] == min_err) {
                if (s[i] == '0') {
                    votes_for_1++;
                } else {
                    votes_for_0++;
                }
            }
        }

        int my_prediction;
        if (votes_for_0 >= votes_for_1) {
            my_prediction = 0;
        } else {
            my_prediction = 1;
        }

        std::cout << my_prediction << std::endl;

        int outcome;
        std::cin >> outcome;

        for (int i = 0; i < n; ++i) {
            int p_i = s[i] - '0';
            if (p_i != outcome) {
                mistakes[i]++;
            } else {
                anti_mistakes[i]++;
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