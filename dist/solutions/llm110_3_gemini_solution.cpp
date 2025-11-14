#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>

const int R = 8;
const int C = 14;

bool find_path_recursive(int r, int c, const std::string& s, int k, const std::vector<std::vector<int>>& grid);

bool can_read(int n, const std::vector<std::vector<int>>& grid) {
    if (n <= 0) return false;
    std::string s = std::to_string(n);
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == (s[0] - '0')) {
                if (find_path_recursive(r, c, s, 1, grid)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool find_path_recursive(int r, int c, const std::string& s, int k, const std::vector<std::vector<int>>& grid) {
    if (k == s.length()) {
        return true;
    }

    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
            if (grid[nr][nc] == (s[k] - '0')) {
                if (find_path_recursive(nr, nc, s, k + 1, grid)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int calculate_initial_score(const std::vector<std::vector<int>>& grid) {
    int s = 0;
    while (can_read(s + 1, grid)) {
        s++;
    }
    return s;
}

int calculate_new_score(const std::vector<std::vector<int>>& grid, int old_best_score) {
    if (can_read(old_best_score + 1, grid)) {
        int current_score = old_best_score + 1;
        while (can_read(current_score + 1, grid)) {
            current_score++;
        }
        return current_score;
    }

    if (old_best_score > 0 && !can_read(old_best_score, grid)) {
        int low = 1, high = old_best_score, first_unreadable = old_best_score;
        while(low <= high) {
            int mid = low + (high - low) / 2;
            if (mid == 0) {
                low = mid + 1;
                continue;
            }
            if (can_read(mid, grid)) {
                low = mid + 1;
            } else {
                first_unreadable = mid;
                high = mid - 1;
            }
        }
        return first_unreadable - 1;
    }
    
    return old_best_score;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::mt19937 rng(42);

    std::uniform_int_distribution<int> row_dist(0, R - 1);
    std::uniform_int_distribution<int> col_dist(0, C - 1);
    std::uniform_int_distribution<int> digit_dist(0, 9);
    std::uniform_int_distribution<int> mutation_count_dist(1, 3);

    std::vector<std::vector<int>> best_grid(R, std::vector<int>(C));

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            best_grid[i][j] = (i * 4 + j / 2) % 10;
        }
    }

    int best_score = calculate_initial_score(best_grid);

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (duration > 59000) {
            break;
        }

        std::vector<std::vector<int>> current_grid = best_grid;
        
        int mutations = mutation_count_dist(rng);
        for(int i = 0; i < mutations; ++i) {
            int r = row_dist(rng);
            int c = col_dist(rng);
            int d = digit_dist(rng);
            if (current_grid[r][c] != d) {
                current_grid[r][c] = d;
            } else {
                current_grid[r][c] = digit_dist(rng);
            }
        }

        int current_score = calculate_new_score(current_grid, best_score);

        if (current_score >= best_score) {
            best_grid = current_grid;
            best_score = current_score;
        }
    }

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            std::cout << best_grid[i][j];
        }
        std::cout << '\n';
    }

    return 0;
}