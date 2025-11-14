#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int q;
    if (!(cin >> q)) return 0;
    vector<unsigned long long> ks(q);
    for (int i = 0; i < q; ++i) cin >> ks[i];
    
    for (int i = 0; i < q; ++i) {
        unsigned long long k = ks[i];
        unsigned long long F = k - 1; // number of non-empty increasing subsequences
        vector<char> rev_ops;
        while (F > 1) {
            if ((F & 1ULL) == 0) {
                F -= 1;             // inverse of adding a minimal element (+1 to W)
                rev_ops.push_back('D'); // forward: add minimal
            } else {
                F = (F - 1) / 2;    // inverse of doubling (adding maximal element)
                rev_ops.push_back('U'); // forward: add maximal
            }
        }
        vector<char> ops(rev_ops.rbegin(), rev_ops.rend());
        int n = (int)ops.size() + 1;
        
        deque<int> order; // tokens in ascending order of values
        order.push_back(0);
        int id = 0;
        for (char c : ops) {
            ++id;
            if (c == 'U') order.push_back(id);
            else order.push_front(id);
        }
        
        vector<int> val(n);
        int v = 0;
        for (int t : order) val[t] = v++;
        
        cout << n << '\n';
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << val[j];
        }
        cout << '\n';
    }
    return 0;
}