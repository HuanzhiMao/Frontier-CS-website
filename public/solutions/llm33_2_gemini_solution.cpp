#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <deque>

void solve() {
    long long k;
    std::cin >> k;

    std::vector<char> ops;
    long long temp_k = k;
    while (temp_k > 1) {
        if (temp_k % 2 == 0) {
            ops.push_back('D');
            temp_k /= 2;
        } else {
            ops.push_back('I');
            temp_k -= 1;
        }
    }
    std::reverse(ops.begin(), ops.end());

    std::deque<int> p;
    int current_val = 0;
    for (char op : ops) {
        if (op == 'D') {
            p.push_back(current_val);
        } else { // 'I'
            p.push_front(current_val);
        }
        current_val++;
    }

    std::cout << p.size() << "\n";
    for (int i = 0; i < p.size(); ++i) {
        std::cout << p[i] << (i == p.size() - 1 ? "" : " ");
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int q;
    std::cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}