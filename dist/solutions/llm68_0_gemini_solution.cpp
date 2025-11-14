#include <iostream>
#include <vector>
#include <numeric>
#include <list>

void solve() {
    int n;
    std::cin >> n;

    std::list<int> candidates;
    for (int i = 0; i < n; ++i) {
        candidates.push_back(i);
    }

    while (candidates.size() > 2) {
        int p1 = candidates.front();
        candidates.pop_front();
        int p2 = candidates.front();
        candidates.pop_front();

        std::cout << "0 " << p1 << std::endl;
        int res1;
        std::cin >> res1;

        std::cout << "0 " << p2 << std::endl;
        int res2;
        std::cin >> res2;

        if (res1 == 1) {
            candidates.push_back(p1);
        }
        if (res2 == 1) {
            candidates.push_back(p2);
        }
    }

    int final1 = candidates.front();
    candidates.pop_front();
    int final2 = candidates.front();
    candidates.pop_front();

    std::cout << "1 " << final1 << " " << final2 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}