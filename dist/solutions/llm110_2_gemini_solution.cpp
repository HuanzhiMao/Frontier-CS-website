#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

// Grid dimensions
const int ROWS = 8;
const int COLS = 14;

// Grid type
using Grid = std::vector<std::vector<int>>;

// PRNG
std::mt19937 rng;

// Adjacency directions (8-way)
const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool is_valid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

// Function to check if a number can be read from the grid
bool can_read(const Grid& grid, int n) {
    if (n <= 0) return true;
    std::string s = std::to_string(n);
    int len = s.length();
    
    std::vector<std::pair<int, int>> current_positions;
    
    // Find all possible starting positions for the first digit
    int first_digit = s[0] - '0';
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c] == first_digit) {
                current_positions.push_back({r, c});
            }
        }
    }
    
    if (current_positions.empty()) return false;
    
    // Iteratively find positions for subsequent digits
    for (int i = 1; i < len; ++i) {
        std::vector<std::pair<int, int>> next_positions;
        int target_digit = s[i] - '0';
        
        std::vector<std::vector<bool>> visited_next(ROWS, std::vector<bool>(COLS, false));

        for (const auto& pos : current_positions) {
            for (int j = 0; j < 8; ++j) {
                int nr = pos.first + dr[j];
                int nc = pos.second + dc[j];
                
                if (is_valid(nr, nc) && grid[nr][nc] == target_digit && !visited_next[nr][nc]) {
                    next_positions.push_back({nr, nc});
                    visited_next[nr][nc] = true;
                }
            }
        }
        
        if (next_positions.empty()) return false;
        current_positions = std::move(next_positions);
    }
    
    return true;
}

// Function to calculate the score of a grid by finding the first unreadable number
int calculate_score(const Grid& grid) {
    int score = 0;
    while (can_read(grid, score + 1)) {
        score++;
    }
    return score;
}

void print_grid(const Grid& grid) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            std::cout << grid[r][c];
        }
        std::cout << '\n';
    }
}

// Generates a grid with a sequential pattern: 012...901...
Grid generate_sequential_grid() {
    Grid grid(ROWS, std::vector<int>(COLS));
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            grid[r][c] = (r * COLS + c) % 10;
        }
    }
    return grid;
}

// Generates a grid with a diagonal wave pattern: (r+c)%10
Grid generate_sum_grid() {
    Grid grid(ROWS, std::vector<int>(COLS));
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            grid[r][c] = (r + c) % 10;
        }
    }
    return grid;
}

// Generates a completely random grid
Grid generate_random_grid() {
    Grid grid(ROWS, std::vector<int>(COLS));
    std::uniform_int_distribution<int> digit_dist(0, 9);
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            grid[r][c] = digit_dist(rng);
        }
    }
    return grid;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    auto total_start_time = std::chrono::steady_clock::now();
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    Grid overall_best_grid(ROWS, std::vector<int>(COLS, 0));
    int overall_best_score = -1;

    int num_restarts = 6;
    long long time_limit_ms = 58000; // Run for 58 seconds to be safe

    for (int run = 0; run < num_restarts; ++run) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - total_start_time).count();
        if (elapsed >= time_limit_ms) break;

        long long time_for_this_run = (time_limit_ms - elapsed) / (num_restarts - run);
        auto run_end_time = now + std::chrono::milliseconds(time_for_this_run);

        Grid current_grid;
        switch (run % 3) {
            case 0: current_grid = generate_sequential_grid(); break;
            case 1: current_grid = generate_sum_grid(); break;
            case 2: current_grid = generate_random_grid(); break;
        }

        int current_score = calculate_score(current_grid);

        if (current_score > overall_best_score) {
            overall_best_score = current_score;
            overall_best_grid = current_grid;
        }

        double temperature = 10.0;
        double cooling_rate = 0.99995;

        std::uniform_int_distribution<int> row_dist(0, ROWS - 1);
        std::uniform_int_distribution<int> col_dist(0, COLS - 1);
        std::uniform_int_distribution<int> digit_dist(0, 9);
        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        
        now = std::chrono::steady_clock::now();
        while (now < run_end_time) {
            Grid next_grid = current_grid;
            
            int r = row_dist(rng);
            int c = col_dist(rng);
            int old_digit = next_grid[r][c];
            int new_digit = digit_dist(rng);
            while (new_digit == old_digit) {
                new_digit = digit_dist(rng);
            }
            next_grid[r][c] = new_digit;

            int next_score = calculate_score(next_grid);
            
            if (next_score > current_score) {
                current_grid = next_grid;
                current_score = next_score;
            } else {
                if (temperature > 1e-9) {
                    double acceptance_probability = exp((double)(next_score - current_score) / temperature);
                    if (prob_dist(rng) < acceptance_probability) {
                        current_grid = next_grid;
                        current_score = next_score;
                    }
                }
            }

            if (current_score > overall_best_score) {
                overall_best_score = current_score;
                overall_best_grid = current_grid;
            }

            temperature *= cooling_rate;
            now = std::chrono::steady_clock::now();
        }
    }
    
    print_grid(overall_best_grid);

    return 0;
}