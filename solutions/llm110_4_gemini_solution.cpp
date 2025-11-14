#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <functional>

using namespace std;

const int R = 8;
const int C = 14;

using Grid = vector<vector<int>>;

bool can_read(const Grid& grid, int n) {
    string s = to_string(n);
    int k = s.length();
    if (k == 0) return true;

    vector<pair<int, int>> q;
    vector<vector<bool>> visited(R, vector<bool>(C, false));
    int first_digit = s[0] - '0';

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == first_digit) {
                if (!visited[r][c]) {
                    q.push_back({r, c});
                    visited[r][c] = true;
                }
            }
        }
    }

    for (int i = 1; i < k; ++i) {
        if (q.empty()) return false;
        vector<pair<int, int>> next_q;
        fill(visited.begin(), visited.end(), vector<bool>(C, false));
        int target_digit = s[i] - '0';

        for (auto const& pos : q) {
            int r = pos.first;
            int c = pos.second;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;

                    if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                        if (grid[nr][nc] == target_digit && !visited[nr][nc]) {
                            next_q.push_back({nr, nc});
                            visited[nr][nc] = true;
                        }
                    }
                }
            }
        }
        q = next_q;
    }

    return !q.empty();
}

int get_score(const Grid& grid) {
    int x = 0;
    while (can_read(grid, x + 1)) {
        x++;
    }
    return x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    Grid current_grid(R, vector<int>(C));
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            current_grid[r][c] = uniform_int_distribution<int>(0, 9)(rng);
        }
    }

    Grid best_grid = current_grid;
    int current_score = get_score(current_grid);
    int best_score = current_score;

    double temperature = 2.0;
    double cooling_rate = 0.99995;
    
    uniform_int_distribution<int> dist_r(0, R - 1);
    uniform_int_distribution<int> dist_c(0, C - 1);
    uniform_int_distribution<int> dist_d(0, 9);
    uniform_real_distribution<double> dist_prob(0.0, 1.0);
    
    while (true) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 59500) {
            break;
        }

        Grid next_grid = current_grid;

        int r = dist_r(rng);
        int c = dist_c(rng);
        int old_digit = next_grid[r][c];
        int new_digit = dist_d(rng);
        if (new_digit == old_digit) {
            new_digit = (new_digit + 1) % 10;
        }
        next_grid[r][c] = new_digit;

        int next_score = get_score(next_grid);
        int score_diff = next_score - current_score;

        if (score_diff > 0 || (temperature > 1e-9 && dist_prob(rng) < exp(score_diff / temperature))) {
            current_grid = next_grid;
            current_score = next_score;
        }
        
        if (current_score > best_score) {
            best_score = current_score;
            best_grid = current_grid;
        }

        temperature *= cooling_rate;
    }

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            cout << best_grid[r][c];
        }
        cout << "\n";
    }

    return 0;
}