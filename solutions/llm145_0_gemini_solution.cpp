#include <iostream>
#include <vector>
#include <string>

// Helper function to print a solution grid, padding each line with spaces to a length of 12.
void print_grid(const std::vector<std::string>& grid) {
    for (const auto& line : grid) {
        std::cout << line;
        for (size_t i = line.length(); i < 12; ++i) {
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

// Solves the Small Task (input 0) by printing a precomputed unique-solution puzzle.
// Digits are from 0 to 3.
void solve_small() {
    std::vector<std::string> solution = {
        "3   2   323",
        "13 23  1   1",
        "3 1 1  3   3",
        "1 3 3  3232",
        "3 1 1  1",
        "1   2  2",
        "",
        "3  3   32312",
        "1 2      3",
        "33   2 3 2",
        "1 1  2 1 1",
        "3  3 323 3"
    };
    print_grid(solution);
}

// Solves the Large Task (input 1) by printing a precomputed unique-solution puzzle.
// Digits are from 1 to 3.
void solve_large() {
    std::vector<std::string> solution = {
        "1   3   323",
        "32 31  2   1",
        "1 2 3  3   2",
        "3 1 1  3232",
        "2 3 3  2",
        "1   1  3",
        "",
        "1  2   22232",
        "2 2      2",
        "13   2 3 3",
        "3 1  2 1 2",
        "1  3 323 1"
    };
    print_grid(solution);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int task_type;
    std::cin >> task_type;

    if (task_type == 0) {
        solve_small();
    } else {
        solve_large();
    }

    return 0;
}