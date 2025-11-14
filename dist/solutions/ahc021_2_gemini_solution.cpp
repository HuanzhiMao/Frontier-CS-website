#include <iostream>
#include <vector>
#include <algorithm>

const int N = 30;
std::vector<std::vector<int>> b;

struct Move {
    int r1, c1, r2, c2;
};
std::vector<Move> history;

// Tries to swap parent with one of its children to satisfy heap property.
// Returns true if a swap was made.
bool fix_up(int r, int c) {
    if (history.size() >= 10000) {
        return false;
    }

    int child1_r = r + 1, child1_c = c;
    int child2_r = r + 1, child2_c = c + 1;

    if (b[child1_r][child1_c] < b[child2_r][child2_c]) {
        if (b[r][c] > b[child1_r][child1_c]) {
            history.push_back({r, c, child1_r, child1_c});
            std::swap(b[r][c], b[child1_r][child1_c]);
            return true;
        }
    } else {
        if (b[r][c] > b[child2_r][child2_c]) {
            history.push_back({r, c, child2_r, child2_c});
            std::swap(b[r][c], b[child2_r][child2_c]);
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    b.resize(N);
    for (int i = 0; i < N; ++i) {
        b[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) {
            std::cin >> b[i][j];
        }
    }

    int iterations = 16;
    
    for (int i = 0; i < iterations; ++i) {
        if (history.size() >= 10000) {
            break;
        }
        
        bool changed = false;

        // Downwards pass: Large values sink
        for (int r = 0; r < N - 1; ++r) {
            if (r % 2 == 0) { // Serpentine traversal
                for (int c = 0; c <= r; ++c) {
                    if (fix_up(r, c)) changed = true;
                }
            } else {
                for (int c = r; c >= 0; --c) {
                    if (fix_up(r, c)) changed = true;
                }
            }
        }
        
        // Upwards pass: Small values float
        for (int r = N - 2; r >= 0; --r) {
            if (r % 2 == 0) { // Serpentine traversal
                 for (int c = 0; c <= r; ++c) {
                    if (fix_up(r, c)) changed = true;
                }
            } else {
                for (int c = r; c >= 0; --c) {
                    if (fix_up(r, c)) changed = true;
                }
            }
        }

        if (!changed) {
            break;
        }
    }
    
    std::cout << history.size() << std::endl;
    for (const auto& move : history) {
        std::cout << move.r1 << " " << move.c1 << " " << move.r2 << " " << move.c2 << std::endl;
    }

    return 0;
}