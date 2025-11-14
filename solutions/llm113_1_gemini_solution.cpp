#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// This function recursively generates the moves for the Tower of Hanoi puzzle.
// n: number of disks to move
// from_peg: the starting peg
// to_peg: the destination peg
// aux_peg: the auxiliary peg
void solve(int n, int from_peg, int to_peg, int aux_peg) {
    if (n == 0) {
        return;
    }
    // Move n-1 disks from source to auxiliary peg, using destination as auxiliary.
    solve(n - 1, from_peg, aux_peg, to_peg);
    // Move the nth disk from source to destination peg.
    std::cout << from_peg << " " << to_peg << "\n";
    // Move the n-1 disks from auxiliary peg to destination peg, using source as auxiliary.
    solve(n - 1, aux_peg, to_peg, from_peg);
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // The number of moves for the Tower of Hanoi puzzle with n disks is 2^n - 1.
    // For N up to 30, this fits within an unsigned long long.
    // (1ULL << n) calculates 2^n using a bit shift on an unsigned long long.
    unsigned long long num_moves = (1ULL << n) - 1;
    std::cout << num_moves << "\n";

    // Although the rules seem complex, the underlying pattern of moves
    // matches the classic Tower of Hanoi problem. For large N (e.g., N > 20),
    // printing all 2^N - 1 moves might be too slow for typical time limits,
    // but we must follow the problem's output requirement.
    if (n > 0) {
        solve(n, 1, 3, 2);
    }

    return 0;
}