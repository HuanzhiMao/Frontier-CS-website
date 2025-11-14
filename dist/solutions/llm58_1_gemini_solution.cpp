#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#if defined(__GNUC__) || defined(__clang__)
#define __builtin_clzll __builtin_clzll
#else
#include <intrin.h>
#pragma intrinsic(_BitScanReverse64)
unsigned long __builtin_clzll(unsigned long long x) {
    if (x == 0) return 64;
    unsigned long index;
    _BitScanReverse64(&index, x);
    return 63 - index;
}
#endif

using namespace std;

const int MAXN_GRID = 300;
int grid[MAXN_GRID + 1][MAXN_GRID + 1];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long x;
    cin >> x;

    if (x == 0) {
        cout << 1 << endl;
        cout << 0 << endl;
        return 0;
    }

    int k = 0;
    if (x > 0) {
        k = 63 - __builtin_clzll(x);
    }

    int n = 2 * k + 3;
    grid[1][1] = 1;

    int r = 1, c = 1;
    for (int i = k - 1; i >= 0; --i) {
        // Current value S is at (r, c). We assume dp[r,c] = S.
        // We will construct a subgrid that produces 2S or 2S+1 at (r+2, c+2).
        
        // Doubler block: Starts from (r,c), produces 2S at (r+1,c+1).
        // It requires paths from (r,c) to both (r,c+1) and (r+1,c).
        grid[r][c + 1] = 1;
        grid[r + 1][c] = 1;
        // Merge point for the doubler.
        grid[r + 1][c + 1] = 1;

        if ((x >> i) & 1) {
            // Add 1. We want to compute 2S+1.
            // We route the 2S path from (r+1,c+1) to (r+2,c+1).
            grid[r + 2][c + 1] = 1;
            
            // We create a separate path with value 1, routed to (r+1,c+2).
            // Start a new path from an unreachable cell (1, c+2).
            grid[1][c + 2] = 1;
            // Extend this path downwards. This path is clean because all cells
            // grid[i][c+1] for i < r are 0.
            for (int row = 2; row <= r + 1; ++row) {
                grid[row][c + 2] = 1;
            }
            
            // Merge the 2S path and the 1 path at (r+2, c+2).
            grid[r + 2][c + 2] = 1;
        } else {
            // No add 1. Just pass the 2S value.
            // Route from (r+1,c+1) to (r+2,c+2) to align with the other case.
            grid[r + 1][c + 2] = 1;
            grid[r + 2][c + 2] = 1;
        }
        r += 2;
        c += 2;
    }

    // After the loop, the value for bits k-1..0 is at (r,c).
    // The MSB (bit k) is implicitly 1, which is our starting value at (1,1).
    // So the final value x is at (r,c).
    // Now connect (r,c) to the destination (n,n).
    for (int j = c + 1; j <= n; ++j) {
        grid[r][j] = 1;
    }
    for (int i = r + 1; i <= n; ++i) {
        grid[i][n] = 1;
    }
    
    cout << n << endl;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << grid[i][j] << (j == n ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}