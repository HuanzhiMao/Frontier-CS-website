#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>

void do_query(int m, const std::vector<int>& a, const std::vector<int>& b) {
    std::cout << "1 " << m << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << a[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << b[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

int read_response() {
    int x;
    std::cin >> x;
    return x;
}

void do_guess(const std::string& s) {
    std::cout << "0 " << s << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    const int k = 5;
    const int num_blocks = n / k;
    const int m = (1 << (k + 1)) - 1;

    std::vector<int> a_std(m), b_std(m);
    for (int j = 0; j < k; ++j) {
        int level_start_state = (1 << j) - 1;
        int next_level_start_state = (1 << (j + 1)) - 1;
        int states_at_level = 1 << j;
        for (int i = 0; i < states_at_level; ++i) {
            int s = level_start_state + i;
            int val = i;
            a_std[s] = next_level_start_state + 2 * val;
            b_std[s] = next_level_start_state + 2 * val + 1;
        }
    }
    for (int s = (1 << k) - 1; s < m; ++s) {
        a_std[s] = s;
        b_std[s] = s;
    }

    std::string s_ans(n, '0');

    for (int j = 0; j < num_blocks; ++j) {
        std::vector<int> a = a_std;
        std::vector<int> b = b_std;

        for (int l = 0; l < j; ++l) {
            int val4 = 0;
            for (int bit_idx = 0; bit_idx < k - 1; ++bit_idx) {
                val4 = val4 * 2 + (s_ans[l * k + bit_idx] - '0');
            }
            int s4 = (1 << (k - 1)) - 1 + val4;
            if (s_ans[l * k + k - 1] == '0') {
                a[s4] = 0;
            } else {
                b[s4] = 0;
            }
        }
        
        do_query(m, a, b);
        int final_state = read_response();
        
        int val5 = final_state - ((1 << k) - 1);
        for (int bit_idx = k - 1; bit_idx >= 0; --bit_idx) {
            s_ans[j * k + bit_idx] = ((val5 % 2) == 1 ? '1' : '0');
            val5 /= 2;
        }
    }

    do_guess(s_ans);

    return 0;
}