#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Global board size
int N;

// Knight move offsets
const int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
const int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

// Helper to check if a square is on the board
inline bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int start_r_1, start_c_1;
    cin >> N >> start_r_1 >> start_c_1;

    // Convert to 0-indexed for internal processing
    int r = start_r_1 - 1;
    int c = start_c_1 - 1;

    // Data structures for the algorithm
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    vector<vector<int>> degrees(N, vector<int>(N, 0));
    vector<pair<int, int>> path;

    // Pre-calculate initial degrees for all squares
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < 8; ++k) {
                if (is_valid(i + dr[k], j + dc[k])) {
                    degrees[i][j]++;
                }
            }
        }
    }

    // Start the path from the given coordinates
    path.push_back({r, c});
    visited[r][c] = true;

    // Update degrees of neighbors of the starting square
    for (int k = 0; k < 8; ++k) {
        int neighbor_r = r + dr[k];
        int neighbor_c = c + dc[k];
        if (is_valid(neighbor_r, neighbor_c)) {
            degrees[neighbor_r][neighbor_c]--;
        }
    }

    // Main loop to build the path
    for (int i = 1; i < N * N; ++i) {
        int next_r = -1, next_c = -1;
        int min_degree = 9; // Initialize with a value > max possible degree (8)

        // Apply Warnsdorff's rule
        for (int j = 0; j < 8; ++j) {
            int candidate_r = r + dr[j];
            int candidate_c = c + dc[j];

            if (is_valid(candidate_r, candidate_c) && !visited[candidate_r][candidate_c]) {
                if (degrees[candidate_r][candidate_c] < min_degree) {
                    min_degree = degrees[candidate_r][candidate_c];
                    next_r = candidate_r;
                    next_c = candidate_c;
                }
            }
        }

        // If a valid move was found
        if (next_r != -1) {
            r = next_r;
            c = next_c;
            
            path.push_back({r, c});
            visited[r][c] = true;

            // Update degrees of the new neighbors
            for (int k = 0; k < 8; ++k) {
                int neighbor_r = r + dr[k];
                int neighbor_c = c + dc[k];
                if (is_valid(neighbor_r, neighbor_c)) {
                    degrees[neighbor_r][neighbor_c]--;
                }
            }
        } else {
            // No more valid moves, end the path.
            break;
        }
    }

    // Output the result
    cout << path.size() << "\n";
    for (const auto& p : path) {
        // Convert back to 1-indexed for output
        cout << p.first + 1 << " " << p.second + 1 << "\n";
    }

    return 0;
}