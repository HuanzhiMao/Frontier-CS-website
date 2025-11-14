#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Performs a query for the box at index i.
 * @param i The 0-based index of the box to query.
 * @return A pair of integers {a0, a1}, where a0 is the count of more
 *         expensive prizes to the left of i, and a1 is the count to the right.
 */
std::pair<int, int> ask(int i) {
    std::cout << "? " << i << std::endl;
    int a0, a1;
    std::cin >> a0 >> a1;
    return {a0, a1};
}

/**
 * @brief Submits the final answer.
 * @param i The 0-based index of the box containing the diamond.
 */
void answer(int i) {
    std::cout << "! " << i << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    int l = 0, r = n - 1;

    while (l < r) {
        int m = l + (r - l) / 2;
        std::pair<int, int> res = ask(m);
        
        // If a0+a1 is 0, we've found the diamond.
        if (res.first + res.second == 0) {
            answer(m);
            return 0;
        }

        // Heuristic: If there are fewer expensive items on the left than on the right,
        // the unique, most expensive item (the diamond) is likely on the right.
        // We move our search space to the right half.
        if (res.first < res.second) {
            l = m + 1;
        } else {
            // Otherwise, the diamond is likely on the left half (or at m).
            // We move our search space to the left half, including m.
            r = m;
        }
    }

    // When l == r, we have narrowed down the search to a single box.
    answer(l);

    return 0;
}