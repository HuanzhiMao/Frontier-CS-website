#include <iostream>
#include <vector>
#include <string>

int main() {
    // We need to print a map that makes it difficult for all kangaroos to gather in a single cell.
    // The map must be a tree of empty cells ('1') on a grid of at most 20x20.
    //
    // The chosen strategy is to create a map with a '+' shape on a 19x19 grid.
    // This map consists of four long arms meeting at a central point.
    // This structure has several properties that make gathering difficult:
    // 1. It is a valid tree structure.
    // 2. Kangaroos start distributed across four arms.
    // 3. Movement along the North/South arms is primarily affected by U/D commands,
    //    while movement along the West/East arms is primarily affected by L/R commands.
    // 4. A random sequence of moves can be thought of as two independent 1D random walks
    //    (one vertical, one horizontal).
    // 5. A statistical bias in the vertical commands (e.g., more 'U' than 'D' moves)
    //    will cause kangaroos on BOTH the North and South arms to drift upwards.
    //    This pushes kangaroos on the North arm towards their tip, but kangaroos on the
    //    South arm towards the center. It does not help them gather; in fact, it separates them.
    // 6. Similarly, a horizontal bias pushes kangaroos on the West and East arms in the
    //    same direction (e.g., left), causing one group to move towards their tip and the
    //    other towards the center.
    // 7. For kangaroos from all arms to meet, they must first reach the central cell.
    //    However, any random drift in the control sequence that brings one arm's population
    //    towards the center will push the opposite arm's population away from the center.
    //
    // This fundamental conflict makes it extremely improbable for a random sequence of
    // 50,000 moves to gather all 37 kangaroos into a single cell. The most likely outcome
    // is the formation of multiple, separate clusters of kangaroos at the tips of the arms.
    // This should result in a high "failure rate" for the judge's test cases, well above
    // the required 125 out of 500.

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 19;
    int m = 19;

    std::cout << n << " " << m << std::endl;

    std::vector<std::string> grid(n, std::string(m, '0'));

    int center_row = n / 2;
    int center_col = m / 2;

    // Create the vertical arm of the '+'
    for (int i = 0; i < n; ++i) {
        grid[i][center_col] = '1';
    }

    // Create the horizontal arm of the '+'
    for (int j = 0; j < m; ++j) {
        grid[center_row][j] = '1';
    }

    // Print the resulting grid
    for (int i = 0; i < n; ++i) {
        std::cout << grid[i] << std::endl;
    }

    return 0;
}