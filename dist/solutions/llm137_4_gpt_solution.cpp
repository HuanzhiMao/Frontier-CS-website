#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 20, m = 20;
    vector<string> grid(n, string(m, '0'));

    for (int r = 0; r < n; ++r) {
        if (r % 2 == 0) {
            // Even rows: full row of '1's
            for (int c = 0; c < m; ++c) grid[r][c] = '1';
        } else {
            // Odd rows: single connector cell to keep the structure as a tree
            if (r % 4 == 1) grid[r][m - 1] = '1';
            else grid[r][0] = '1';
        }
    }

    cout << n << " " << m << "\n";
    for (int i = 0; i < n; ++i) {
        cout << grid[i] << "\n";
    }
    return 0;
}