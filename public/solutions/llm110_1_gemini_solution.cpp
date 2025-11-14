#include <iostream>
#include <vector>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int R = 8;
    const int C = 14;
    std::vector<std::vector<int>> grid(R, std::vector<int>(C));

    const int C_split = C / 2;

    // The grid is constructed using several patterns to ensure that for any
    // pair of digits (d1, d2), there is an instance of d1 adjacent to an
    // instance of d2. This is a very strong property that allows forming
    // a vast number of integers.

    // Upper part (rows 0 to R-3) uses two different periodic formulas.
    // This creates a variety of adjacencies within each part and at their boundary.
    for (int r = 0; r < R - 2; ++r) {
        // Left half of the upper part: grid[r][c] = (3*r + c) % 10
        // This pattern makes each digit adjacent to digits d +/- {1, 2, 3, 4}.
        for (int c = 0; c < C_split; ++c) {
            grid[r][c] = (3 * r + c) % 10;
        }
        // Right half of the upper part: grid[r][c] = (5*r + c) % 10
        // This pattern makes each digit adjacent to digits d +/- {1, 4, 5, 6}.
        for (int c = C_split; c < C; ++c) {
            grid[r][c] = (5 * r + c) % 10;
        }
    }

    // The boundary between row R-3 and R-2, and the boundary between the
    // left and right halves provide additional adjacencies, covering most
    // digit transitions.

    // Bottom two rows (R-2 and R-1) are made identical.
    // The formula grid[r][c] = c % 10 ensures all digits 0-9 appear.
    // By duplicating the row, we guarantee that for every digit d, an instance
    // of d is adjacent to another instance of d. This covers all d->d transitions.
    for (int c = 0; c < C; ++c) {
        grid[R - 2][c] = c % 10;
        grid[R - 1][c] = c % 10;
    }
    
    // The adjacency between the upper part (row R-3) and the bottom part (row R-2)
    // covers the remaining required transitions, such as d -> d+5.

    // Print the generated grid
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            std::cout << grid[r][c];
        }
        std::cout << '\n';
    }

    return 0;
}