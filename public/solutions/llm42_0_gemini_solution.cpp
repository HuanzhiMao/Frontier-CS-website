#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <algorithm>
#include <tuple>

// Use long double for higher precision in geometric calculations
using LD = long double;

// Structure to hold information about a single square
struct Square {
    LD x, y, a; // center (x, y), angle a in degrees
};

// Structure to hold a complete solution for a given n
struct Solution {
    LD L; // side length of the container
    std::vector<Square> squares;
};

// Memoization table to store solutions for subproblems
std::map<int, Solution> memo;

// The main recursive function to solve the packing problem for n squares
Solution solve(int n) {
    // If the solution is already computed, return it
    if (memo.count(n)) {
        return memo[n];
    }

    // Base case: n=0, no squares, container size 0
    if (n == 0) {
        return {0.0, {}};
    }
    // Base case: n=1, a single square in a 1x1 container
    if (n == 1) {
        return memo[n] = {1.0, {{0.5, 0.5, 0.0}}};
    }
    
    // If n is a perfect square, the optimal solution is a simple grid packing
    long long k_perf = round(sqrt(n));
    if (k_perf * k_perf == n) {
        Solution sol;
        sol.L = k_perf;
        for (int i = 0; i < n; ++i) {
            sol.squares.push_back({(LD)(i % k_perf) + 0.5L, (LD)(i / k_perf) + 0.5L, 0.0L});
        }
        return memo[n] = sol;
    }

    // For n=3, use a known, good configuration which is better than the baseline.
    // This serves as a "seed" for the recursion to generate better solutions for larger n.
    // This specific configuration places three squares in a 1.866... x 1.866... box.
    if (n == 3) {
        Solution sol;
        sol.L = 1.0L + sqrtl(3.0L) / 2.0L;
        sol.squares.push_back({sol.L / 2.0L, 0.5, 90.0});
        sol.squares.push_back({0.5, sol.L - 0.5L / sqrtl(3.0L), 30.0});
        sol.squares.push_back({sol.L - 0.5, sol.L - 0.5L / sqrtl(3.0L), 150.0});
        return memo[n] = sol;
    }

    // --- Strategy: Compare a simple baseline with a recursive approach ---

    // 1. Baseline solution: pack squares in a grid.
    LD L_base = ceill(sqrtl(n));
    
    // 2. Recursive solution:
    // Divide the problem for n squares into four subproblems for ceil(n/4) squares.
    // Solve for ceil(n/4), get a container of side L_sub.
    // Combine four such solutions in a 2x2 arrangement to get a solution for n.
    // The new container side length will be 2 * L_sub.
    int n_sub = (n + 3) / 4;
    Solution sub_sol = solve(n_sub);
    LD L_rec = 2.0L * sub_sol.L;
    
    // Choose the better of the two strategies
    if (L_base <= L_rec) {
        // If baseline is better, construct the baseline packing.
        Solution sol;
        sol.L = L_base;
        long long k_base = L_base;
        for (int i = 0; i < n; ++i) {
            sol.squares.push_back({(LD)(i % k_base) + 0.5L, (LD)(i / k_base) + 0.5L, 0.0L});
        }
        return memo[n] = sol;
    } else {
        // If recursion is better, construct the recursive packing.
        Solution sol;
        sol.L = L_rec;
        int placed_count = 0;
        
        // Place squares from the subproblem solution into the four quadrants,
        // scaling their positions appropriately.
        
        // Quadrant 1 (bottom-left)
        int count1 = std::min(n_sub, n - placed_count);
        for (int i = 0; i < count1; ++i) {
            Square s = sub_sol.squares[i];
            sol.squares.push_back({s.x, s.y, s.a});
        }
        placed_count += count1;
        
        // Quadrant 2 (bottom-right)
        if (placed_count < n) {
            int count2 = std::min(n_sub, n - placed_count);
            for (int i = 0; i < count2; ++i) {
                Square s = sub_sol.squares[i];
                sol.squares.push_back({s.x + sub_sol.L, s.y, s.a});
            }
            placed_count += count2;
        }

        // Quadrant 3 (top-left)
        if (placed_count < n) {
            int count3 = std::min(n_sub, n - placed_count);
            for (int i = 0; i < count3; ++i) {
                Square s = sub_sol.squares[i];
                sol.squares.push_back({s.x, s.y + sub_sol.L, s.a});
            }
            placed_count += count3;
        }

        // Quadrant 4 (top-right)
        if (placed_count < n) {
            int count4 = std::min(n_sub, n - placed_count);
            for (int i = 0; i < count4; ++i) {
                Square s = sub_sol.squares[i];
                sol.squares.push_back({s.x + sub_sol.L, s.y + sub_sol.L, s.a});
            }
        }
        return memo[n] = sol;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    Solution result = solve(n);
    
    // Output the results with high precision
    std::cout << std::fixed << std::setprecision(10) << result.L << "\n";
    for (const auto& s : result.squares) {
        std::cout << std::fixed << std::setprecision(10) << s.x << " " << s.y << " " << s.a << "\n";
    }

    return 0;
}