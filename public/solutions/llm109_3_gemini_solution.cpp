#include <iostream>
#include <vector>
#include <utility>

// Using global variables for board size and visited status for simplicity in competitive programming.
int N;
std::vector<std::vector<int>> visited;

// Knight move offsets: 8 possible moves
const int dr[] = {-2, -2, -1, -1,  1,  1,  2,  2};
const int dc[] = {-1,  1, -2,  2, -2,  2, -1,  1};

// Checks if a given cell (r, c) is on the board and has not been visited yet.
inline bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N && visited[r][c] == 0;
}

// Calculates the "degree" of a cell (r, c), which is the number of
// valid unvisited moves from that cell. This is a key part of Warnsdorff's rule.
int get_degree(int r, int c) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        if (is_valid(r + dr[i], c + dc[i])) {
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

    // Initialize the visited board (all 0s)
    visited.assign(N, std::vector<int>(N, 0));
    
    // Vector to store the sequence of moves in the path
    std::vector<std::pair<int, int>> path;

    // Convert 1-indexed input to 0-indexed for array access
    int curr_r = start_r - 1;
    int curr_c = start_c - 1;
    
    // Main loop to build the knight's tour
    while (true) {
        // Mark current square as visited and add it to the path
        visited[curr_r][curr_c] = 1; // Any non-zero value works
        path.push_back({curr_r + 1, curr_c + 1}); // Store 1-indexed for final output

        // Variables to determine the best next move
        int next_r = -1, next_c = -1;
        int min_degree = 9; // Max possible degree is 8, so 9 is a safe initial value

        // Apply Warnsdorff's rule: iterate through all 8 potential moves
        for (int i = 0; i < 8; ++i) {
            int pot_r = curr_r + dr[i];
            int pot_c = curr_c + dc[i];

            // Check if the potential move is valid
            if (is_valid(pot_r, pot_c)) {
                int degree = get_degree(pot_r, pot_c);
                
                // If this move is better (lower degree) or it's the first valid move found
                if (next_r == -1 || degree < min_degree) {
                    min_degree = degree;
                    next_r = pot_r;
                    next_c = pot_c;
                } else if (degree == min_degree) {
                    // Tie-breaking rule (inferred from sample case):
                    // choose the move with the largest row, then the largest column.
                    if (pot_r > next_r || (pot_r == next_r && pot_c > next_c)) {
                        next_r = pot_r;
                        next_c = pot_c;
                    }
                }
            }
        }

        // If no valid move was found, the knight is trapped and the tour ends
        if (next_r == -1) {
            break;
        }

        // Update the current position to the chosen next square
        curr_r = next_r;
        curr_c = next_c;
    }

    // Output the results in the specified format
    std::cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i].first << " " << path[i].second;
        if (i < path.size() - 1) {
            std::cout << "\n";
        }
    }

    return 0;
}