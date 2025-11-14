#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

// A vector to store the sequence of moves.
std::vector<std::pair<int, int>> moves;

/**
 * @brief Recursively solves the puzzle, moving n balls from one basket to another.
 *
 * This function implements the Tower of Hanoi algorithm. It moves n balls
 * from a source basket to a destination basket using an auxiliary basket.
 *
 * @param n The number of balls to move.
 * @param from_peg The source basket number (1, 2, or 3).
 * @param to_peg The destination basket number (1, 2, or 3).
 * @param aux_peg The auxiliary basket number (1, 2, or 3).
 */
void solve(int n, int from_peg, int to_peg, int aux_peg) {
    if (n == 0) {
        return;
    }
    // Step 1: Move n-1 balls from source to auxiliary, so they are out of the way.
    solve(n - 1, from_peg, aux_peg, to_peg);
    
    // Step 2: Move the nth ball from source to destination.
    moves.push_back({from_peg, to_peg});
    
    // Step 3: Move the n-1 balls from auxiliary to destination.
    solve(n - 1, aux_peg, to_peg, from_peg);
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // Initial call to move N balls from basket 1 to 3, using 2 as auxiliary.
    solve(n, 1, 3, 2);

    // Output the total number of moves.
    std::cout << moves.size() << "\n";

    // Output each move.
    for (const auto& move : moves) {
        std::cout << move.first << " " << move.second << "\n";
    }

    return 0;
}