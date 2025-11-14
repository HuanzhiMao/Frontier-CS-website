#include <bits/stdc++.h>
using namespace std;

int main() {
    const int n = 20, m = 20;
    vector<string> g(n, string(m, '0'));
    // Top row as spine
    for (int c = 0; c < m; ++c) g[0][c] = '1';
    // Vertical branches on even columns
    for (int c = 0; c < m; c += 2) {
        for (int r = 1; r < n; ++r) g[r][c] = '1';
    }
    cout << n << " " << m << "\n";
    for (int r = 0; r < n; ++r) {
        cout << g[r] << "\n";
    }
    return 0;
}