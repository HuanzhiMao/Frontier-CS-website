#include <iostream>
#include <vector>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int n = 20;
    const int m = 20;

    std::vector<std::string> grid(n, std::string(m, '0'));

    // Create the central vertical spine
    const int spine_col = 9;
    for (int i = 0; i < n; ++i) {
        grid[i][spine_col] = '1';
    }

    // Create the main alternating horizontal teeth on even rows
    for (int i = 0; i < n; i += 2) {
        if ((i / 2) % 2 == 0) { // Right-pointing teeth for rows 0, 4, 8, 12, 16
            for (int j = spine_col + 1; j < m; ++j) {
                grid[i][j] = '1';
            }
        } else { // Left-pointing teeth for rows 2, 6, 10, 14, 18
            for (int j = 0; j < spine_col; ++j) {
                grid[i][j] = '1';
            }
        }
    }

    // Create secondary vertical branches on odd rows, attached to the main teeth
    for (int i = 0; i < n; i += 2) { // Iterate over main teeth locations
        if ((i / 2) % 2 == 0) { // From a right-pointing tooth at row i
            // Add stubs on rows i-1 and i+1 if they are within bounds
            if (i > 0) {
                grid[i - 1][13] = '1';
                grid[i - 1][17] = '1';
            }
            if (i < n - 1) {
                grid[i + 1][13] = '1';
                grid[i + 1][17] = '1';
            }
        } else { // From a left-pointing tooth at row i
            // Add stubs on rows i-1 and i+1 if they are within bounds
            if (i > 0) {
                grid[i - 1][2] = '1';
                grid[i - 1][6] = '1';
            }
            if (i < n - 1) {
                grid[i + 1][2] = '1';
                grid[i + 1][6] = '1';
            }
        }
    }

    // Print the dimensions and the grid
    std::cout << n << " " << m << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << grid[i] << std::endl;
    }

    return 0;
}