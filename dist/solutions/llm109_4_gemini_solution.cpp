#include <iostream>
#include <vector>
#include <utility>

// Globals for convenience, as they are used across multiple functions.
int N;
std::vector<std::vector<char>> visited;

// 8 possible knight moves. The order affects tie-breaking in the Warnsdorff's rule
// implementation. Any fixed, consistent order is acceptable.
int dr[] = {1, 1, 2, 2, -1, -1, -2, -2};
int dc[] = {2, -2, 1, -1, 2, -2, 1, -1};

// Checks if a square (r, c) is within the N x N board.
bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

// Warnsdorff's rule helper: counts the number of unvisited onward moves from a square.
// This is also known as the degree of the vertex in the graph of possible moves.
int get_degree(int r, int c) {
    int degree = 0;
    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid(nr, nc) && !visited[nr][nc]) {
            degree++;
        }
    }
    return degree;
}

int main() {
    // Use fast I/O for performance, which is important for large N.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int start_r, start_c;
    std::cin >> N >> start_r >> start_c;

    // Adjust to 0-indexed for internal array/vector access.
    start_r--;
    start_c--;

    // Initialize the visited board with all squares marked as unvisited (0).
    visited.assign(N, std::vector<char>(N, 0));
    
    // Vector to store the sequence of moves (the path).
    std::vector<std::pair<int, int>> path;
    path.reserve(N * N); // Pre-allocate memory for efficiency.

    int curr_r = start_r;
    int curr_c = start_c;

    // Build the path step-by-step for N*N moves.
    for (int step = 0; step < N * N; ++step) {
        path.push_back({curr_r, curr_c});
        visited[curr_r][curr_c] = 1; // Mark the current square as visited.

        // If all squares have been visited, the tour is complete.
        if (path.size() == (size_t)N * N) {
            break;
        }

        int next_r = -1;
        int next_c = -1;
        int min_degree = 9; // A knight's maximum degree is 8. Initialize with a higher value.

        // Find the next move using Warnsdorff's rule:
        // Move to the unvisited neighbor with the fewest onward moves.
        for (int i = 0; i < 8; ++i) {
            int nr = curr_r + dr[i];
            int nc = curr_c + dc[i];

            if (is_valid(nr, nc) && !visited[nr][nc]) {
                int degree = get_degree(nr, nc);
                if (degree < min_degree) {
                    min_degree = degree;
                    next_r = nr;
                    next_c = nc;
                }
            }
        }
        
        // If a valid next move is found, update the current position.
        // For N>=6, a full tour is always possible, so we should always find a next move.
        if (next_r != -1) {
             curr_r = next_r;
             curr_c = next_c;
        } else {
             // This case implies the heuristic got stuck before visiting all squares.
             // Given the problem constraints (N>=6), this is not expected to happen.
             // If it were to happen, the loop would break, and we would output the longest path found.
             break;
        }
    }

    // Output the results in the required format.
    std::cout << path.size() << "\n";
    for (const auto& p : path) {
        // Convert back to 1-indexed for the final output.
        std::cout << p.first + 1 << " " << p.second + 1 << "\n";
    }

    return 0;
}