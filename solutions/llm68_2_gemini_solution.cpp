#include <iostream>
#include <vector>
#include <numeric>
#include <list>

void solve() {
    int n;
    std::cin >> n;

    std::list<int> cands;
    for (int i = 0; i < n; ++i) {
        cands.push_back(i);
    }

    while (cands.size() > 2) {
        int p = cands.front();
        cands.pop_front();

        std::cout << "0 " << p << std::endl;
        int res;
        std::cin >> res;
        
        if (res == 1) {
            cands.push_back(p);
        }
    }
    
    int p1 = cands.front();
    cands.pop_front();
    int p2 = cands.front();
    cands.pop_front();

    std::cout << "1 " << p1 << " " << p2 << std::endl;
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