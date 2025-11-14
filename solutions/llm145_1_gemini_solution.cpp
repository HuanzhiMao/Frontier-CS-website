#include <iostream>
#include <vector>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int task_type;
    std::cin >> task_type;

    if (task_type == 0) {
        std::cout << "1   2   101 \n";
    } else {
        std::cout << "1   2   111 \n";
    }

    std::cout << "32 23  1   1\n";
    std::cout << "3 2 1  1   1\n";
    std::cout << "1 1 1  1222 \n";
    std::cout << "1 1 1  2    \n";
    std::cout << "1   2  2    \n";
    std::cout << "            \n";
    std::cout << "1  2   2 211\n";
    std::cout << "1 2   1 3 1 \n";
    std::cout << "21   232 2 1\n";
    std::cout << "1 2  2 1 2 2\n";
    std::cout << "1  2 1 2 211\n";

    return 0;
}