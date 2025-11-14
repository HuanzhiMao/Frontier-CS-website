#include <iostream>
#include <vector>
#include <string>

void solve() {
    int task_type;
    std::cin >> task_type;

    if (task_type == 0) {
        std::vector<std::string> solution = {
            "1   3   121 ",
            "21 21  3   3",
            "1 3 1  3   2",
            "1 2 2  2132 ",
            "2 1 2  1    ",
            "1   2  2    ",
            "            ",
            "2  1   32213",
            "1 1      2  ",
            "21   1 3 1  ",
            "1 1  1 2 2  ",
            "1  3 222 2  "
        };
        for (const auto& line : solution) {
            std::cout << line << "\n";
        }
    } else {
        std::vector<std::string> solution = {
            "1   2   132 ",
            "32 32  2   1",
            "1 2 2  3   2",
            "2 2 2  3232 ",
            "1 1 1  2    ",
            "1   1  2    ",
            "            ",
            "2  1   32313",
            "2 2      1  ",
            "32   2 3 2  ",
            "1 2  1 1 2  ",
            "1  3 323 1  "
        };
        for (const auto& line : solution) {
            std::cout << line << "\n";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}