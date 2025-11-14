#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long x;
    cin >> x;

    // Use a sufficiently large grid size. M is the dimension of the backbone.
    // n is the total grid size.
    int M = 63;
    int n = M + 2 * 61 + 2;
    vector<vector<int>> grid(n + 1, vector<int>(n + 1, 0));

    // Part 1: Backbone to generate powers of 2.
    // The number of paths to (i,i) will be 2^(i-1).
    // We need powers up to 2^60, so backbone up to i=61 is enough.
    for (int i = 1; i <= 62; ++i) {
        grid[i][i] = 1;
    }
    for (int i = 1; i <= 61; ++i) {
        grid[i + 1][i] = 1;
        grid[i][i + 1] = 1;
    }

    // Part 2: Collector path, runs along row M.
    // It will sum the contributions from the taps.
    int last_tap_col = M + 2 * 60;
    for (int j = 1; j <= last_tap_col + 1; ++j) {
        grid[M][j] = 1;
    }

    // Part 3: Taps for each set bit of x.
    // For each k where the k-th bit is 1, tap 2^k from (k+1, k+1).
    for (int k = 0; k < 61; ++k) {
        if ((x >> k) & 1) {
            // 2^k paths are available at cell (k+1, k+1).
            // We route these paths to the collector row at a designated column.
            // Using staggered columns (M + 2*k) prevents interference between taps.
            int tap_col = M + 2 * k;
            
            // Horizontal path segment from backbone to the tap column.
            for (int j = k + 1; j <= tap_col; ++j) {
                grid[k + 1][j] = 1;
            }
            // Vertical path segment up to the row just before the collector.
            for (int i = k + 1; i < M; ++i) {
                grid[i][tap_col] = 1;
            }
        }
    }
    
    // Part 4: Final sink path.
    // The total path count x is accumulated on the collector path.
    // The final sum is at cell (M, last_tap_col + 1).
    // We route this to the bottom-right corner (n, n).
    int final_sum_col = last_tap_col + 1;
    for (int i = M; i <= n; ++i) {
        grid[i][final_sum_col] = 1;
    }
    for (int j = final_sum_col; j <= n; ++j) {
        grid[n][j] = 1;
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