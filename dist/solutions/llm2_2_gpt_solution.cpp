#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> tokens;
    long long val;
    while (cin >> val) tokens.push_back(val);
    if (tokens.empty()) return 0;

    int n = (int)tokens[0];
    vector<long long> perm;
    if ((int)tokens.size() >= 1 + n) {
        perm.assign(tokens.begin() + 1, tokens.begin() + 1 + n);
    } else {
        perm.resize(n);
        for (int i = 0; i < n; ++i) perm[i] = i + 1;
    }

    cout << 1;
    for (int i = 0; i < n; ++i) cout << ' ' << perm[i];
    cout << '\n';
    return 0;
}