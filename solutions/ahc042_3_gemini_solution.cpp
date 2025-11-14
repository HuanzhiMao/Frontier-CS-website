#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

const int N = 20;

// Represents a potential move (a bulk shift in one direction).
struct Move {
    char dir = ' ';
    int index = -1;
    int cost = 1e9;
    int oni_removed = 0;
    double score = -1.0;

    // Custom comparison to find the best move.
    // Prioritizes higher score (oni/cost), then more oni removed, then lower cost.
    bool is_better_than(const Move& other) const {
        if (std::abs(score - other.score) > 1e-9) {
            return score > other.score;
        }
        if (oni_removed != other.oni_removed) {
            return oni_removed > other.oni_removed;
        }
        return cost < other.cost;
    }
};

// Applies a single one-square shift to the grid.
void apply_single_shift(std::vector<std::string>& grid, char dir, int index) {
    if (dir == 'L') {
        for (int j = 0; j < N - 1; ++j) {
            grid[index][j] = grid[index][j + 1];
        }
        grid[index][N - 1] = '.';
    } else if (dir == 'R') {
        for (int j = N - 1; j > 0; --j) {
            grid[index][j] = grid[index][j - 1];
        }
        grid[index][0] = '.';
    } else if (dir == 'U') {
        for (int i = 0; i < N - 1; ++i) {
            grid[i][index] = grid[i + 1][index];
        }
        grid[N - 1][index] = '.';
    } else if (dir == 'D') {
        for (int i = N - 1; i > 0; --i) {
            grid[i][index] = grid[i - 1][index];
        }
        grid[0][index] = '.';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_dummy;
    std::cin >> n_dummy;
    std::vector<std::string> grid(N);
    int oni_count = 0;
    for(int i = 0; i < N; ++i) {
        std::cin >> grid[i];
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == 'x') {
                oni_count++;
            }
        }
    }
    
    std::vector<std::pair<char, int>> operations;

    while (oni_count > 0) {
        Move best_move;

        // Evaluate row shifts
        for (int i = 0; i < N; ++i) {
            // Left shift
            int first_fuku = N;
            for (int j = 0; j < N; ++j) {
                if (grid[i][j] == 'o') {
                    first_fuku = j;
                    break;
                }
            }
            int oni_to_remove = 0;
            int last_oni_pos = -1;
            for (int j = 0; j < first_fuku; ++j) {
                if (grid[i][j] == 'x') {
                    oni_to_remove++;
                    last_oni_pos = j;
                }
            }
            if (oni_to_remove > 0) {
                int cost = last_oni_pos + 1;
                Move current_move = {'L', i, cost, oni_to_remove, (double)oni_to_remove / cost};
                if (current_move.is_better_than(best_move)) {
                    best_move = current_move;
                }
            }

            // Right shift
            int last_fuku = -1;
            for (int j = N - 1; j >= 0; --j) {
                if (grid[i][j] == 'o') {
                    last_fuku = j;
                    break;
                }
            }
            oni_to_remove = 0;
            int first_oni_pos = N;
            for (int j = N - 1; j > last_fuku; --j) {
                if (grid[i][j] == 'x') {
                    oni_to_remove++;
                    first_oni_pos = j;
                }
            }
            if (oni_to_remove > 0) {
                int cost = N - first_oni_pos;
                Move current_move = {'R', i, cost, oni_to_remove, (double)oni_to_remove / cost};
                if (current_move.is_better_than(best_move)) {
                    best_move = current_move;
                }
            }
        }

        // Evaluate column shifts
        for (int j = 0; j < N; ++j) {
            // Up shift
            int first_fuku = N;
            for (int i = 0; i < N; ++i) {
                if (grid[i][j] == 'o') {
                    first_fuku = i;
                    break;
                }
            }
            int oni_to_remove = 0;
            int last_oni_pos = -1;
            for (int i = 0; i < first_fuku; ++i) {
                if (grid[i][j] == 'x') {
                    oni_to_remove++;
                    last_oni_pos = i;
                }
            }
            if (oni_to_remove > 0) {
                int cost = last_oni_pos + 1;
                Move current_move = {'U', j, cost, oni_to_remove, (double)oni_to_remove / cost};
                if (current_move.is_better_than(best_move)) {
                    best_move = current_move;
                }
            }

            // Down shift
            int last_fuku = -1;
            for (int i = N - 1; i >= 0; --i) {
                if (grid[i][j] == 'o') {
                    last_fuku = i;
                    break;
                }
            }
            oni_to_remove = 0;
            int first_oni_pos = N;
            for (int i = N - 1; i > last_fuku; --i) {
                if (grid[i][j] == 'x') {
                    oni_to_remove++;
                    first_oni_pos = i;
                }
            }
            if (oni_to_remove > 0) {
                int cost = N - first_oni_pos;
                Move current_move = {'D', j, cost, oni_to_remove, (double)oni_to_remove / cost};
                if (current_move.is_better_than(best_move)) {
                    best_move = current_move;
                }
            }
        }

        if (best_move.oni_removed > 0) {
            for (int k = 0; k < best_move.cost; ++k) {
                operations.push_back({best_move.dir, best_move.index});
                apply_single_shift(grid, best_move.dir, best_move.index);
            }
            oni_count -= best_move.oni_removed;
        } else {
            // This case should ideally not be reached due to the problem guarantee
            // that a valid move always exists for any Oni on the initial board.
            // We assume our greedy strategy does not lead to a fully trapped state.
            break; 
        }
    }

    for (const auto& op : operations) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}