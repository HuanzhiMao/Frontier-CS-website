#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    for (int i = 0; i < n; ++i) {
        int x, y;
        long long r;
        cin >> x >> y >> r;
        int a = x, b = y, c = min(10000, x + 1), d = min(10000, y + 1);
        if (c <= a) c = a + 1;
        if (d <= b) d = b + 1;
        cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    }
    return 0;
}