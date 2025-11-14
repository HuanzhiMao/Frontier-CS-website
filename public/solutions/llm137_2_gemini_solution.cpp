#include <iostream>
#include <vector>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 20;
    int m = 20;

    std::cout << n << " " << m << std::endl;

    std::vector<std::string> grid(n, std::string(m, '0'));

    // The map is designed as a large comb-like structure.
    // It is a tree, which satisfies the problem constraints.
    // The main idea is to create a structure where different groups of kangaroos
    // are primarily affected by different sets of moves.
    // Kangaroos in vertical "teeth" of the comb are only moved by U/D commands.
    // Kangaroos on the horizontal "spine" are mostly moved by L/R commands.
    // This separation of controls makes it very difficult for a random sequence
    // of moves to coordinate the gathering of all kangaroos, especially given
    // the large number of kangaroos and the size of the map.
    // Moving kangaroos from one tooth to another requires a trip along the spine,
    // and actions that bring some kangaroos closer might move others farther apart.

    // A horizontal spine is placed in the middle of the grid.
    int spine_row = n / 2 - 1;
    for (int j = 0; j < m; ++j) {
        grid[spine_row][j] = '1';
    }
    
    // Vertical teeth are placed on every other column, covering the full height.
    for (int j = 0; j < m; j += 2) {
        for (int i = 0; i < n; ++i) {
            grid[i][j] = '1';
        }
    }
    
    for (int i = 0; i < n; ++i) {
        std::cout << grid[i] << std::endl;
    }
    
    return 0;
}