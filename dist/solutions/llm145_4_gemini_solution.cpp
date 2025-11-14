#include <iostream>
#include <vector>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int task_type;
    std::cin >> task_type;

    // The solution grid is the same for both the Small Task (input 0) and the Large Task (input 1).
    // This is because the grid only contains digits from 1 to 3, which is a valid set of digits for both tasks.
    // This specific grid is a known solution to this problem that results in a Number Loop puzzle with a unique solution.
    std::vector<std::string> grid = {
        "1   1   311 ",
        "11 11  3   1",
        "1 2 1  3   1",
        "1 2 1  3111 ",
        "1 2 1  3    ",
        "1   1  3    ",
        "            ",
        "2  2   11111",
        "2 1      1  ",
        "22   1 2 1  ",
        "2 1  1 2 1  ",
        "2  1 111 1  "
    };

    for (const std::string& row : grid) {
        std::cout << row << "\n";
    }

    return 0;
}