#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <numeric>
#include <algorithm>
#include <queue>
#include <cmath>

// A simple competitive programming setup
void setup_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

const int N = 10;
using Grid = std::array<std::array<int, N>, N>;

// Global state
int flavors[100];
int counts[4] = {0};
int target_col_start[4];
int target_col_end[4];

// Simulates a tilt move on the grid
void apply_tilt(Grid& grid, char move) {
    if (move == 'F') {
        for (int c = 0; c < N; ++c) {
            int write_r = 0;
            for (int read_r = 0; read_r < N; ++read_r) {
                if (grid[read_r][c] != 0) {
                    if (read_r != write_r) {
                        grid[write_r][c] = grid[read_r][c];
                        grid[read_r][c] = 0;
                    }
                    write_r++;
                }
            }
        }
    } else if (move == 'B') {
        for (int c = 0; c < N; ++c) {
            int write_r = N - 1;
            for (int read_r = N - 1; read_r >= 0; --read_r) {
                if (grid[read_r][c] != 0) {
                    if (read_r != write_r) {
                        grid[write_r][c] = grid[read_r][c];
                        grid[read_r][c] = 0;
                    }
                    write_r--;
                }
            }
        }
    } else if (move == 'L') {
        for (int r = 0; r < N; ++r) {
            int write_c = 0;
            for (int read_c = 0; read_c < N; ++read_c) {
                if (grid[r][read_c] != 0) {
                    if (read_c != write_c) {
                        grid[r][write_c] = grid[r][read_c];
                        grid[r][read_c] = 0;
                    }
                    write_c++;
                }
            }
        }
    } else if (move == 'R') {
        for (int r = 0; r < N; ++r) {
            int write_c = N - 1;
            for (int read_c = N - 1; read_c >= 0; --read_c) {
                if (grid[r][read_c] != 0) {
                    if (read_c != write_c) {
                        grid[r][write_c] = grid[r][read_c];
                        grid[r][read_c] = 0;
                    }
                    write_c--;
                }
            }
        }
    }
}

// Calculates the sum of squares of component sizes
long long calculate_connectivity_score(const Grid& grid) {
    long long total_sq_sum = 0;
    std::array<std::array<bool, N>, N> visited{};
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (grid[r][c] != 0 && !visited[r][c]) {
                int current_flavor = grid[r][c];
                int component_size = 0;
                std::queue<std::pair<int, int>> q;

                q.push({r, c});
                visited[r][c] = true;
                component_size++;

                while (!q.empty()) {
                    std::pair<int, int> curr = q.front();
                    q.pop();

                    int dr[] = {-1, 1, 0, 0};
                    int dc[] = {0, 0, -1, 1};
                    for (int i = 0; i < 4; ++i) {
                        int nr = curr.first + dr[i];
                        int nc = curr.second + dc[i];
                        if (nr >= 0 && nr < N && nc >= 0 && nc < N &&
                            !visited[nr][nc] && grid[nr][nc] == current_flavor) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                            component_size++;
                        }
                    }
                }
                total_sq_sum += (long long)component_size * component_size;
            }
        }
    }
    return total_sq_sum;
}

// Calculates sum of distances of candies to their target column bands
int calculate_distance_score(const Grid& grid) {
    int total_dist = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (grid[r][c] != 0) {
                int flavor = grid[r][c];
                int start = target_col_start[flavor];
                int end = target_col_end[flavor];
                if (c < start) {
                    total_dist += (start - c);
                } else if (c > end) {
                    total_dist += (c - end);
                }
            }
        }
    }
    return total_dist;
}

// Reads flavor sequence and determines target regions
void precompute_targets() {
    for (int i = 0; i < 100; ++i) {
        std::cin >> flavors[i];
        counts[flavors[i]]++;
    }
    
    std::vector<std::pair<int, int>> sorted_flavors;
    for (int i = 1; i <= 3; ++i) {
        sorted_flavors.push_back({counts[i], i});
    }
    std::sort(sorted_flavors.rbegin(), sorted_flavors.rend());

    std::vector<int> f_order;
    f_order.push_back(sorted_flavors[0].second); // Largest
    f_order.push_back(sorted_flavors[2].second); // Smallest
    f_order.push_back(sorted_flavors[1].second); // Medium
    
    std::vector<double> ideal_widths_f;
    for(int f : f_order) {
        ideal_widths_f.push_back(10.0 * counts[f] / 100.0);
    }
    
    std::vector<int> widths(3);
    int total_width = 0;
    std::vector<std::pair<double, int>> remainders;
    for(int i=0; i<3; ++i) {
        widths[i] = floor(ideal_widths_f[i]);
        total_width += widths[i];
        remainders.push_back({ideal_widths_f[i] - widths[i], i});
    }

    std::sort(remainders.rbegin(), remainders.rend());
    
    int remaining_width = 10 - total_width;
    for(int i=0; i<remaining_width; ++i) {
        widths[remainders[i].second]++;
    }

    int current_col = 0;
    for (int i=0; i<3; ++i) {
        int f = f_order[i];
        int w = widths[i];
        if (w > 0) {
            target_col_start[f] = current_col;
            target_col_end[f] = current_col + w - 1;
            current_col += w;
        } else {
            // Handle zero-width case
            target_col_start[f] = current_col;
            target_col_end[f] = current_col - 1;
        }
    }
}

int main() {
    setup_io();
    precompute_targets();

    Grid grid{};
    
    for (int t = 0; t < 100; ++t) {
        int p;
        std::cin >> p;

        int empty_count = 0;
        int r_new = -1, c_new = -1;
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (grid[r][c] == 0) {
                    empty_count++;
                    if (empty_count == p) {
                        r_new = r;
                        c_new = c;
                        break;
                    }
                }
            }
            if (r_new != -1) break;
        }
        grid[r_new][c_new] = flavors[t];

        if (t == 99) {
            break;
        }

        char best_move = ' ';
        double best_score = -1e18;

        char moves[] = {'F', 'B', 'L', 'R'};
        for (char move : moves) {
            Grid temp_grid = grid;
            apply_tilt(temp_grid, move);
            
            long long conn_score = calculate_connectivity_score(temp_grid);
            int dist_score = calculate_distance_score(temp_grid);

            // Heuristic to balance connectivity (short-term goal) and distance to target (long-term goal).
            // The weights shift over time from prioritizing distance to prioritizing connectivity.
            double w_conn = t + 1.0;
            double w_dist = 99.0 - t;
            double current_score = w_conn * conn_score - w_dist * dist_score;

            if (current_score > best_score) {
                best_score = current_score;
                best_move = move;
            }
        }
        
        std::cout << best_move << std::endl;
        apply_tilt(grid, best_move);
    }

    return 0;
}