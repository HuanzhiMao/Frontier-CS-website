#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> tokens;
    int x;
    while (cin >> x) tokens.push_back(x);
    if (tokens.empty()) return 0;
    
    int n = tokens[0];
    if ((int)tokens.size() < n + 1) return 0; // insufficient data for offline version
    
    vector<int> pos(n + 1, -1);
    for (int i = 1; i <= n; ++i) {
        int val = tokens[i];
        if (val >= 1 && val <= n) pos[val] = i;
    }
    
    int i1 = pos[n / 2];
    int i2 = pos[n / 2 + 1];
    if (i1 == -1 || i2 == -1) return 0;
    
    cout << i1 << " " << i2 << "\n";
    return 0;
}