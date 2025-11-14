#include <iostream>
#include <vector>
#include <string>

int main() {
    // Set the grid dimensions to the maximum allowed size to maximize complexity.
    int n = 20;
    int m = 20;

    std::cout << n << " " << m << std::endl;

    std::vector<std::string> grid(n, std::string(m, '0'));

    // The chosen map structure is a "comb" or "centipede".
    // It consists of a long horizontal "backbone" and many vertical "teeth".
    // This structure is a tree, satisfying the problem requirements (connected, acyclic).
    // The key idea is to partition the kangaroos into many groups, one for each tooth.
    // Kangaroos start in a tooth and are very likely to remain there, as moving out
    // requires a specific sequence of moves to reach the backbone.
    // For all kangaroos to gather, they must all navigate out of their starting teeth,
    // meet on the backbone, and then move to a common cell.
    // The long paths and the need for coordinated movement across different parts of the
    // map make it extremely unlikely for this to happen with a random sequence of moves.

    // Place the backbone in the middle row.
    int backbone_row = 9;
    for (int j = 0; j < m; ++j) {
        grid[backbone_row][j] = '1';
    }

    // Add teeth alternating in direction (up/down) for each column.
    for (int j = 0; j < m; ++j) {
        if (j % 2 == 0) {
            // Teeth pointing up from the backbone for even columns.
            for (int i = 0; i < backbone_row; ++i) {
                grid[i][j] = '1';
            }
        } else {
            // Teeth pointing down from the backbone for odd columns.
            for (int i = backbone_row + 1; i < n; ++i) {
                grid[i][j] = '1';
            }
        }
    }

    // Print the generated grid.
    for (int i = 0; i < n; ++i) {
        std::cout << grid[i] << std::endl;
    }

    return 0;
}