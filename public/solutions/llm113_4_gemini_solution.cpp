#include <iostream>
#include <vector>
#include <cmath>

// A vector to store the sequence of moves.
std::vector<std::pair<int, int>> moves;

// Recursive function to solve the puzzle, modeled after the Tower of Hanoi problem.
// n: the number of balls to move.
// src: the source basket.
// aux: the auxiliary basket.
// dst: the destination basket.
void solve(int n, int src, int aux, int dst) {
    // Base case: if there are no balls to move, do nothing.
    if (n == 0) {
        return;
    }
    
    // Step 1: Move n-1 balls from the source to the auxiliary basket,
    // using the destination basket as temporary storage.
    solve(n - 1, src, dst, aux);
    
    // Step 2: Move the nth ball from the source to the destination basket.
    moves.push_back({src, dst});
    
    // Step 3: Move the n-1 balls from the auxiliary basket to the destination basket,
    // using the source basket as temporary storage.
    solve(n - 1, aux, src, dst);
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n;
    std::cin >> n;
    
    // The number of moves for N balls in the Tower of Hanoi puzzle is 2^N - 1.
    // Using (1LL << n) - 1 to calculate this to avoid overflow for large n.
    long long num_moves = (1LL << n) - 1;
    std::cout << num_moves << "\n";
    
    // Generate the moves for N balls from basket 1 to basket 3, using basket 2 as auxiliary.
    solve(n, 1, 2, 3);
    
    // Print all the generated moves.
    for (const auto& move : moves) {
        std::cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}