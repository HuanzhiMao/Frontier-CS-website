#include <iostream>
#include <vector>
#include <utility>

// This vector will store the sequence of moves.
std::vector<std::pair<int, int>> moves;

/**
 * @brief Generates moves for the Tower of Hanoi puzzle.
 *
 * This function recursively generates the sequence of moves to transfer n disks
 * from a source peg to a destination peg using an auxiliary peg. The problem,
 * despite its unique rules about moving the "center ball", follows the same
 * move sequence pattern as the classic Tower of Hanoi problem.
 *
 * @param n The number of balls to move.
 * @param from_peg The source basket (1, 2, or 3).
 * @param to_peg The destination basket (1, 2, or 3).
 * @param aux_peg The auxiliary basket (1, 2, or 3).
 */
void solve(int n, int from_peg, int to_peg, int aux_peg) {
    if (n == 0) {
        return;
    }
    // Move n-1 balls from source to auxiliary, using destination as temporary.
    solve(n - 1, from_peg, aux_peg, to_peg);

    // Move the nth ball from source to destination.
    moves.push_back({from_peg, to_peg});

    // Move the n-1 balls from auxiliary to destination, using source as temporary.
    solve(n - 1, aux_peg, to_peg, from_peg);
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // The initial call to move N balls from basket 1 to 3, using 2 as auxiliary.
    solve(n, 1, 3, 2);

    // Output the total number of moves.
    std::cout << moves.size() << "\n";

    // Output each move.
    for (const auto& move : moves) {
        std::cout << move.first << " " << move.second << "\n";
    }

    return 0;
}