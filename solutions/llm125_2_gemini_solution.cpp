#include <iostream>
#include <vector>
#include <numeric>

void solve() {
    int n;
    std::cin >> n;

    std::vector<int> s;
    int current_kinds = 0;

    for (int i = 1; i <= 2 * n; ++i) {
        std::cout << "? " << i << std::endl;
        int new_kinds;
        std::cin >> new_kinds;

        if (new_kinds > current_kinds) {
            s.push_back(i);
        } else {
            int partner = s.back();
            s.pop_back();
            std::cout << "! " << partner << " " << i << std::endl;
        }
        current_kinds = new_kinds;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}