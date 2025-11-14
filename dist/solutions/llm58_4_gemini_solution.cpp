#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int MAX_N = 130;
bool grid[MAX_N][MAX_N];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long x;
    cin >> x;

    int k = 0;
    if (x > 0) {
        k = 63 - __builtin_clzll(x);
    }

    int n = k * 2 + 4;
    int r = 0, c = 0;

    // Start with a single path
    grid[r][c] = true;
    grid[r + 1][c] = true;
    r++;

    // This represents the initial '1' for the MSB of x
    grid[r][c + 1] = true;
    c++;

    // Process bits from k-1 down to 0
    for (int i = k - 1; i >= 0; --i) {
        // Doubler gadget: from (r, c) to (r+1, c+1)
        grid[r - 1][c + 1] = true;
        grid[r][c + 1] = true;
        r++;
        grid[r][c] = true;
        c++;
        grid[r][c] = true;

        if ((x >> i) & 1) {
            // Add-one gadget: merge a path from start
            // This path travels along row 0 and then comes down
            // to merge with the upper branch of the next doubler.
            for (int j = 0; j < r; ++j) {
                grid[j][c + 1] = true;
            }
        }
    }

    // Connect the final point to the sink (n-1, n-1)
    grid[r][c + 1] = true;
    c++;
    while (c < n - 1) {
        grid[r][c + 1] = true;
        c++;
    }
    while (r < n - 1) {
        grid[r + 1][c] = true;
        r++;
    }
    
    cout << n << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << grid[i][j] << (j == n - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}