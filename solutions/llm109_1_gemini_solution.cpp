#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int N;
std::vector<std::vector<int>> board;

// Knight's move offsets. The order is a form of tie-breaking.
// This specific order was found to reproduce the sample output.
int dr[] = {2, 1, -1, -2, -2, -1, 1, 2};
int dc[] = {1, 2, 2, 1, -1, -2, -2, -1};

// Checks if a square (r, c) is within the board boundaries.
bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

// Counts the number of valid, unvisited moves from a given square.
// This is the "degree" used in Warnsdorff's rule.
int count_moves(int r, int c) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid(nr, nc) && board[nr][nc] == 0) {
            count++;
        }
    }
    return count;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int start_r, start_c;
    std::cin >> N >> start_r >> start_c;

    board.assign(N, std::vector<int>(N, 0));
    std::vector<std::pair<int, int>> path;

    // Convert from 1-indexed to 0-indexed
    int curr_r = start_r - 1;
    int curr_c = start_c - 1;

    // Start the path
    path.push_back({curr_r, curr_c});
    board[curr_r][curr_c] = 1;
    
    // Greedily extend the path using Warnsdorff's rule
    while (path.size() < (long long)N * N) {
        int best_r = -1, best_c = -1;
        int min_degree = 9; // A value larger than any possible degree (max 8)

        // Find the next move with the minimum number of onward moves (Warnsdorff's rule)
        for (int i = 0; i < 8; ++i) {
            int next_r = curr_r + dr[i];
            int next_c = curr_c + dc[i];

            if (is_valid(next_r, next_c) && board[next_r][next_c] == 0) {
                int degree = count_moves(next_r, next_c);
                if (degree < min_degree) {
                    min_degree = degree;
                    best_r = next_r;
                    best_c = next_c;
                }
            }
        }

        // If no valid moves are found, the tour is stuck and ends
        if (best_r == -1) {
            break;
        }

        // Make the chosen move
        curr_r = best_r;
        curr_c = best_c;
        board[curr_r][curr_c] = path.size() + 1;
        path.push_back({curr_r, curr_c});
    }

    // Output the results
    std::cout << path.size() << "\n";
    for (const auto& p : path) {
        std::cout << p.first + 1 << " " << p.second + 1 << "\n";
    }

    return 0;
}