#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long x;
    cin >> x;

    int n = 64;
    vector<vector<int>> grid(n + 1, vector<int>(n + 1, 0));

    // Part 1: Power-of-2 Generator
    // This structure ensures that the number of paths from (1,1) to (i,i) is 2^(i-1).
    // We generate powers up to 2^61, which requires cells up to (62, 62).
    grid[1][1] = 1;
    for (int i = 2; i <= 62; ++i) {
        grid[i][i] = 1;
        grid[i - 1][i] = 1;
        grid[i][i - 1] = 1;
    }

    // Part 2: Collector and Router
    // We check the binary representation of x. If the (i-1)-th bit is 1,
    // we need to add 2^(i-1) to the total count. The cell (i, i) has 2^(i-1) paths.
    // We create a path from (i, i) to the sink (n, n) for each required power of two.
    for (int i = 1; i <= 62; ++i) {
        if ((x >> (i - 1)) & 1) {
            // Route paths from (i, i) to (n, n).
            // The path goes right along row i, then down along column n.
            // Horizontal segment on row i:
            for (int c = i + 1; c <= n; ++c) {
                grid[i][c] = 1;
            }
            // Vertical segment on column n:
            for (int r = i + 1; r <= n; ++r) {
                grid[r][n] = 1;
            }
        }
    }
    
    // The sink cell itself must be walkable.
    grid[n][n] = 1;

    cout << n << endl;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << grid[i][j] << (j == n ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}