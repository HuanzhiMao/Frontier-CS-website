#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int q;
    if(!(cin >> q)) return 0;
    vector<unsigned long long> ks(q);
    for (int i = 0; i < q; ++i) cin >> ks[i];

    for (int i = 0; i < q; ++i) {
        unsigned long long k = ks[i];
        unsigned long long K = k - 1; // we will represent K as sum of (2^b - 1)
        vector<int> blocks;

        while (K > 0) {
            unsigned long long x = K + 1;
            int b = 63 - __builtin_clzll(x); // floor(log2(K+1))
            unsigned long long val = (1ULL << b) - 1;
            blocks.push_back(b);
            K -= val;
        }

        int n = 0;
        for (int b : blocks) n += b;

        cout << n << "\n";
        vector<int> perm;
        perm.reserve(n);
        int cur = n - 1;
        for (int b : blocks) {
            for (int v = cur - b + 1; v <= cur; ++v) perm.push_back(v);
            cur -= b;
        }
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << perm[j];
        }
        cout << "\n";
    }
    return 0;
}