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
        vector<int> segs;

        auto flog2 = [](unsigned long long x) -> int {
            return 63 - __builtin_clzll(x);
        };

        while (true) {
            int S = flog2(k);
            unsigned long long pow2S = 1ULL << S;
            if (pow2S == k) {
                segs.push_back(S);
                break;
            } else {
                segs.push_back(S);
                k = k - pow2S + 1;
            }
        }

        int m = (int)segs.size();
        vector<int> starts(m);
        int offset = 0;
        for (int i = m - 1; i >= 0; --i) {
            starts[i] = offset;
            offset += segs[i];
        }
        int n = offset;

        cout << n << "\n";
        bool first = true;
        for (int i = 0; i < m; ++i) {
            int s = starts[i];
            int len = segs[i];
            for (int j = 0; j < len; ++j) {
                if (!first) cout << ' ';
                cout << (s + j);
                first = false;
            }
        }
        if (n == 0) cout << "\n"; else cout << "\n";
    }
    return 0;
}