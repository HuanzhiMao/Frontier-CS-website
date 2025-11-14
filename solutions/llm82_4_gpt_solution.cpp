#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    auto solve = [&](int n) {
        int k = 0;
        while ((1 << k) < n) ++k;
        int mask = (1 << k) - 1;
        vector<int> p(n, mask);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int v; 
                if (!(cin >> v)) return vector<int>();
                p[i] &= v;
                p[j] &= v;
            }
        }
        return p;
    };

    int first;
    if (!(cin >> first)) return 0;

    vector<vector<int>> results;
    if (first >= 3) {
        int n = first;
        auto res = solve(n);
        if (res.empty()) return 0;
        results.push_back(move(res));
        int n2;
        while (cin >> n2) {
            auto res2 = solve(n2);
            if (res2.empty()) break;
            results.push_back(move(res2));
        }
    } else {
        int T = first;
        for (int t = 0; t < T; ++t) {
            int n; 
            if (!(cin >> n)) return 0;
            auto res = solve(n);
            if (res.empty()) return 0;
            results.push_back(move(res));
        }
    }

    for (size_t idx = 0; idx < results.size(); ++idx) {
        const auto &p = results[idx];
        for (size_t i = 0; i < p.size(); ++i) {
            if (i) cout << ' ';
            cout << p[i];
        }
        if (idx + 1 < results.size()) cout << '\n';
    }
    return 0;
}