#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

// Global variables for board state and size
int N;
std::vector<std::vector<int>> board;

// Knight move offsets (8 directions).
// The order of these arrays determines tie-breaking in Warnsdorff's rule.
// This specific order was chosen as it reproduces the sample output.
int dr[] = {2, 1, -1, -2, -2, -1, 1, 2};
int dc[] = {1, 2, 2, 1, -1, -2, -2, -1};

// Checks if a square (r, c) is within the board boundaries and has not been visited.
bool is_valid_and_unvisited(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N && board[r][c] == 0;
}

// Calculates the "degree" of a square (r, c), which is the number of
// unvisited squares it can move to. This is the core of Warnsdorff's rule.
int get_degree(int r, int c) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid_and_unvisited(nr, nc)) {
            count++;
        }
    }
    return count;
}

// Main logic to solve the Knight's Tour problem
void solve() {
    int start_r_1based, start_c_1based;
    std::cin >> N >> start_r_1based >> start_c_1based;

    // Convert 1-indexed input to 0-indexed for array access
    int r = start_r_1based - 1;
    int c = start_c_1based - 1;

    // Initialize the board and the path vector
    board.assign(N, std::vector<int>(N, 0));
    std::vector<std::pair<int, int>> path;

    // Start the tour from the given position
    int move_count = 1;
    board[r][c] = move_count;
    path.push_back({r, c});

    // Main loop to build the path, attempting to visit all N*N squares
    for (int i = 0; i < N * N - 1; ++i) {
        int min_degree = 9; // A value greater than the max possible degree (8)
        int next_r = -1, next_c = -1;

        // Iterate through all 8 possible knight moves from the current position
        for (int j = 0; j < 8; ++j) {
            int nr = r + dr[j];
            int nc = c + dc[j];

            // If the move is to a valid and unvisited square
            if (is_valid_and_unvisited(nr, nc)) {
                // Calculate its degree
                int degree = get_degree(nr, nc);
                
                // Warnsdorff's rule: choose the move with the minimum degree.
                // The first one found in case of a tie is chosen due to the loop order.
                if (degree < min_degree) {
                    min_degree = degree;
                    next_r = nr;
                    next_c = nc;
                }
            }
        }
        
        // If no valid move is found, the knight is stuck and the path ends.
        if (next_r == -1) {
            break;
        }

        // Otherwise, make the chosen move
        r = next_r;
        c = next_c;
        move_count++;
        board[r][c] = move_count;
        path.push_back({r, c});
    }

    // Output the length of the path and the path itself
    std::cout << path.size() << "\n";
    for (const auto& p : path) {
        // Convert 0-indexed coordinates back to 1-indexed for output
        std::cout << p.first + 1 << " " << p.second + 1 << "\n";
    }
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    solve();
    
    return 0;
}