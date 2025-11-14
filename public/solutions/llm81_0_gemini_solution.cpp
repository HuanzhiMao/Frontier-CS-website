#include <iostream>
#include <vector>
#include <string>
#include <numeric>

void perform_query(int m, const std::vector<int>& a, const std::vector<int>& b) {
    std::cout << 1 << std::endl;
    std::cout << m << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << a[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << b[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

void submit_guess(const std::string& s) {
    std::cout << 0 << std::endl;
    std::cout << s << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    std::cin >> N;

    std::string s(N, ' ');
    const int m = 102;
    const int C = m - 2;

    std::vector<int> a(m), b(m);
    int sum_of_bits = 0;

    for (int k = 0; k < N; ++k) {
        int state_at_k = sum_of_bits % C;
        
        // After processing S_k, state will be one of these two
        int state_if_0 = state_at_k;
        int state_if_1 = (state_at_k + 1) % C;

        // Default transitions: count 1s
        for (int i = 0; i < C; ++i) {
            a[i] = i;
            b[i] = (i + 1) % C;
        }

        // Override transitions for our target states to lead to sinks
        a[state_if_0] = C;
        b[state_if_0] = C;

        // state_if_0 and state_if_1 are always different
        a[state_if_1] = C + 1;
        b[state_if_1] = C + 1;
        
        // Sinks are absorbing
        a[C] = C; b[C] = C;
        a[C + 1] = C + 1; b[C + 1] = C + 1;

        perform_query(m, a, b);

        int result;
        std::cin >> result;

        if (result == C) {
            s[k] = '0';
        } else {
            s[k] = '1';
            sum_of_bits++;
        }
    }

    submit_guess(s);

    return 0;
}