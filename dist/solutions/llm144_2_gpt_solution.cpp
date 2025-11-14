#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<int> p;
    p.reserve(n);
    for (int i = 0, x; i < n && (cin >> x); ++i) p.push_back(x);
    if ((int)p.size() < n) return 0;

    int v1 = n / 2, v2 = n / 2 + 1;
    int i1 = -1, i2 = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] == v1) i1 = i + 1;
        if (p[i] == v2) i2 = i + 1;
    }
    if (i1 == -1 || i2 == -1) return 0;

    cout << i1 << " " << i2 << "\n";
    return 0;
}