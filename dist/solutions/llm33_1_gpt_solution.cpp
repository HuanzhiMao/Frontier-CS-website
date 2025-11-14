#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int q;
    if(!(cin >> q)) return 0;
    for (int _ = 0; _ < q; ++_) {
        unsigned long long k;
        cin >> k;
        
        vector<char> ops_rev;
        unsigned long long cur = k;
        while (cur > 2) {
            if (cur & 1ULL) {
                ops_rev.push_back('L'); // +1 -> append new minimum
                --cur;
            } else {
                ops_rev.push_back('H'); // *2 -> append new maximum
                cur >>= 1;
            }
        }
        vector<char> ops(ops_rev.rbegin(), ops_rev.rend());
        
        vector<long long> a;
        long long mn = 0, mx = 0;
        a.push_back(0);
        for (char c : ops) {
            if (c == 'H') {
                ++mx;
                a.push_back(mx);
            } else {
                --mn;
                a.push_back(mn);
            }
        }
        long long shift = -mn;
        for (auto &x : a) x += shift;
        
        cout << (int)a.size() << "\n";
        for (int i = 0; i < (int)a.size(); ++i) {
            if (i) cout << ' ';
            cout << a[i];
        }
        cout << "\n";
    }
    return 0;
}