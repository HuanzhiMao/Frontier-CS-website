#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    unsigned long long x;
    if (!(cin >> x)) return 0;
    
    const int L = 60;
    const int r0 = 2;
    const int c0 = 2;
    const int N = max(r0 + L, c0 + L + 1); // 63
    
    vector<vector<int>> g(N + 1, vector<int>(N + 1, 0));
    
    // Row 1 walkway
    for (int j = 1; j <= c0 + L; ++j) g[1][j] = 1;
    
    // Build the chain of L doubling gadgets (2x2 squares)
    for (int k = 0; k < L; ++k) {
        int r = r0 + k, c = c0 + k;
        g[r][c] = 1;
        g[r][c + 1] = 1;
        g[r + 1][c] = 1;
        g[r + 1][c + 1] = 1;
    }
    
    // Aggregator path: from a_L to (N,N), unique forced path
    int r_end = r0 + L;
    int c_end = c0 + L;
    for (int j = c_end + 1; j <= N; ++j) g[r_end][j] = 1;     // right to column N
    for (int i = r_end + 1; i <= N; ++i) g[i][N] = 1;         // down to row N
    
    // Vertical entrances according to bits of x
    for (int i = 0; i < L; ++i) {
        if ((x >> i) & 1ULL) {
            int k = L - i;               // entrance at a_k
            int col = c0 + k;
            int until = r0 + k;
            for (int row = 1; row <= until; ++row) g[row][col] = 1;
        }
    }
    
    cout << N << '\n';
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (j > 1) cout << ' ';
            cout << g[i][j];
        }
        cout << '\n';
    }
    return 0;
}