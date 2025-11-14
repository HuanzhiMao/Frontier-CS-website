#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <utility>

const int N = 10;

// Represents the 10x10 grid.
struct Board {
    int grid[N][N]{};
};

// Applies a tilt operation to the board.
void apply_tilt(Board& b, char move) {
    if (move == 'F') {
        for (int j = 0; j < N; ++j) {
            int write_idx = 0;
            for (int i = 0; i < N; ++i) {
                if (b.grid[i][j] != 0) {
                    if (i != write_idx) {
                        b.grid[write_idx][j] = b.grid[i][j];
                        b.grid[i][j] = 0;
                    }
                    write_idx++;
                }
            }
        }
    } else if (move == 'B') {
        for (int j = 0; j < N; ++j) {
            int write_idx = N - 1;
            for (int i = N - 1; i >= 0; --i) {
                if (b.grid[i][j] != 0) {
                    if (i != write_idx) {
                        b.grid[write_idx][j] = b.grid[i][j];
                        b.grid[i][j] = 0;
                    }
                    write_idx--;
                }
            }
        }
    } else if (move == 'L') {
        for (int i = 0; i < N; ++i) {
            int write_idx = 0;
            for (int j = 0; j < N; ++j) {
                if (b.grid[i][j] != 0) {
                    if (j != write_idx) {
                        b.grid[i][write_idx] = b.grid[i][j];
                        b.grid[i][j] = 0;
                    }
                    write_idx++;
                }
            }
        }
    } else if (move == 'R') {
        for (int i = 0; i < N; ++i) {
            int write_idx = N - 1;
            for (int j = N - 1; j >= 0; --j) {
                if (b.grid[i][j] != 0) {
                    if (j != write_idx) {
                        b.grid[i][write_idx] = b.grid[i][j];
                        b.grid[i][j] = 0;
                    }
                    write_idx--;
                }
            }
        }
    }
}

// Global variables for evaluation function to access target regions.
int region_of_flavor[4];
int boundaries[3][2];

// Evaluates a board state. Lower score is better.
// The score is a weighted sum of two components:
// 1. Distance penalty: How far candies are from their target regions.
// 2. Connectivity bonus: How many same-flavored candies are adjacent.
// The weights change over time to shift focus from positioning to clustering.
double evaluate(const Board& b, int turn) {
    long long dist_penalty = 0;
    long long conn_bonus = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (b.grid[i][j] == 0) continue;

            int flavor = b.grid[i][j];
            int region = region_of_flavor[flavor];
            int c_start = boundaries[region][0];
            int c_end = boundaries[region][1];

            // 1. Distance penalty (squared distance to target column range)
            int dist = 0;
            if (j < c_start) dist = c_start - j;
            else if (j > c_end) dist = j - c_end;
            dist_penalty += dist * dist;

            // 2. Connectivity bonus (number of same-flavor neighbor pairs)
            if (j + 1 < N && b.grid[i][j + 1] == flavor) {
                conn_bonus++;
            }
            if (i + 1 < N && b.grid[i + 1][j] == flavor) {
                conn_bonus++;
            }
        }
    }
    
    // Weights that change with turn number
    double w_dist = 101.0 - turn;
    double w_conn = 8.0 * turn; // Tuned constant

    return w_dist * dist_penalty - w_conn * conn_bonus;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // --- Preprocessing: Read flavors and define target regions ---
    std::vector<int> f(100);
    int d[4] = {0};
    for (int i = 0; i < 100; ++i) {
        std::cin >> f[i];
        d[f[i]]++;
    }

    // Sort flavors by their total count
    std::vector<std::pair<int, int>> counts;
    for (int i = 1; i <= 3; ++i) {
        counts.push_back({d[i], i});
    }
    std::sort(counts.begin(), counts.end());
    
    int flavor_s = counts[0].second; // smallest count
    int flavor_m = counts[1].second; // medium count
    int flavor_l = counts[2].second; // largest count

    int d_s = counts[0].first;
    int d_m = counts[1].first;
    int d_l = counts[2].first;
    
    // Assign flavors to regions: smallest and medium on edges, largest in middle
    int d_map[3] = {d_s, d_l, d_m};
    region_of_flavor[flavor_s] = 0; // Left region
    region_of_flavor[flavor_l] = 1; // Middle region
    region_of_flavor[flavor_m] = 2; // Right region

    // Partition the 10 columns proportionally to flavor counts
    std::vector<std::pair<int, int>> remainders;
    int cols[3];
    int total_cols = 0;
    for(int i = 0; i < 3; ++i) {
        cols[i] = (10 * d_map[i]) / 100;
        total_cols += cols[i];
        remainders.push_back({(10 * d_map[i]) % 100, i});
    }
    std::sort(remainders.rbegin(), remainders.rend());
    
    for(int i = 0; i < 10 - total_cols; ++i) {
        cols[remainders[i].second]++;
    }

    boundaries[0][0] = 0;
    boundaries[0][1] = cols[0] - 1;
    boundaries[1][0] = cols[0];
    boundaries[1][1] = cols[0] + cols[1] - 1;
    boundaries[2][0] = cols[0] + cols[1];
    boundaries[2][1] = 9;

    Board board;

    // --- Main Loop: Process 100 turns ---
    for (int t = 1; t <= 100; ++t) {
        int p;
        std::cin >> p;
        
        // Find p-th empty cell and place the new candy
        int empty_count = 0;
        int r_new = -1, c_new = -1;
        for (int i = 0; i < N && r_new == -1; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board.grid[i][j] == 0) {
                    empty_count++;
                    if (empty_count == p) {
                        r_new = i;
                        c_new = j;
                        break;
                    }
                }
            }
        }
        board.grid[r_new][c_new] = f[t - 1];
        
        if (t == 100) break;
        
        // Find the best move by simulating all 4 possible tilts
        double best_score = 1e18;
        char best_move = ' ';
        
        const char moves[] = {'F', 'B', 'L', 'R'};
        for (char move : moves) {
            Board next_board = board;
            apply_tilt(next_board, move);
            double current_score = evaluate(next_board, t);
            
            if (current_score < best_score) {
                best_score = current_score;
                best_move = move;
            }
        }
        
        std::cout << best_move << std::endl;
        
        // Update the main board state with the chosen move
        apply_tilt(board, best_move);
    }

    return 0;
}