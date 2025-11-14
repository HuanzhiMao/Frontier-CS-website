#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int q;
    if (!(cin >> q)) return 0;
    vector<unsigned long long> ks(q);
    for (int i = 0; i < q; ++i) cin >> ks[i];

    for (int qi = 0; qi < q; ++qi) {
        unsigned long long k = ks[qi];
        int m = 63 - __builtin_clzll(k); // floor(log2(k))
        unsigned long long base = 1ULL << m; // 2^m
        vector<int> order; // indices in increasing value order
        int id = 0;
        // base increasing block
        for (int i = 0; i < m; ++i) order.push_back(id++);

        unsigned long long R = k - base;
        // insert remainder bits from high to low
        for (int r = m - 1; r >= 0; --r) {
            if ((R >> r) & 1ULL) {
                order.insert(order.begin() + r, id++);
            }
        }

        int n = id;
        vector<int> val(n);
        for (int i = 0; i < n; ++i) val[order[i]] = i;

        cout << n << "\n";
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << val[i];
        }
        cout << "\n";
    }
    return 0;
}