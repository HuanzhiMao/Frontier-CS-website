#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <array>
#include <cmath>

using namespace std;

const int SIZE = 10;

using Grid = array<array<int, SIZE>, SIZE>;

int flavors[100];
int d[4] = {0, 0, 0, 0};

bool use_vertical_split = true;
int region_starts[4];
int region_ends[4];

// Weights for evaluation function
const int W1 = 10; // Region cost
const int W2 = 5;  // Clustering bonus (same neighbors)
const int W3 = 1;  // Boundary penalty (different neighbors)


void setup_regions() {
    int w[4], h[4];

    // Vertical split widths and badness
    int wc1 = (SIZE * d[1] + 50) / 100;
    int wc2 = (SIZE * (d[1] + d[2]) + 50) / 100;
    w[1] = wc1;
    w[2] = wc2 - wc1;
    w[3] = SIZE - wc1 - w[2];
    long long badness_v = 1LL * (SIZE * w[1] - d[1]) * (SIZE * w[1] - d[1]) +
                         1LL * (SIZE * w[2] - d[2]) * (SIZE * w[2] - d[2]) +
                         1LL * (SIZE * w[3] - d[3]) * (SIZE * w[3] - d[3]);

    // Horizontal split heights and badness
    int hc1 = (SIZE * d[1] + 50) / 100;
    int hc2 = (SIZE * (d[1] + d[2]) + 50) / 100;
    h[1] = hc1;
    h[2] = hc2 - hc1;
    h[3] = SIZE - hc1 - h[2];
    long long badness_h = 1LL * (SIZE * h[1] - d[1]) * (SIZE * h[1] - d[1]) +
                         1LL * (SIZE * h[2] - d[2]) * (SIZE * h[2] - d[2]) +
                         1LL * (SIZE * h[3] - d[3]) * (SIZE * h[3] - d[3]);

    if (badness_v <= badness_h) {
        use_vertical_split = true;
        region_starts[1] = 0;
        region_ends[1] = w[1] - 1;
        region_starts[2] = w[1];
        region_ends[2] = w[1] + w[2] - 1;
        region_starts[3] = w[1] + w[2];
        region_ends[3] = SIZE - 1;
    } else {
        use_vertical_split = false;
        region_starts[1] = 0;
        region_ends[1] = h[1] - 1;
        region_starts[2] = h[1];
        region_ends[2] = h[1] + h[2] - 1;
        region_starts[3] = h[1] + h[2];
        region_ends[3] = SIZE - 1;
    }
}

pair<int, int> p_to_rc(const Grid& grid, int p) {
    int empty_count = 0;
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid[r][c] == 0) {
                empty_count++;
                if (empty_count == p) {
                    return {r, c};
                }
            }
        }
    }
    return {-1, -1};
}

void apply_tilt(Grid& grid, char dir) {
    if (dir == 'F') {
        for (int c = 0; c < SIZE; ++c) {
            int empty_r = 0;
            for (int r = 0; r < SIZE; ++r) {
                if (grid[r][c] != 0) {
                    if (r != empty_r) {
                        grid[empty_r][c] = grid[r][c];
                        grid[r][c] = 0;
                    }
                    empty_r++;
                }
            }
        }
    } else if (dir == 'B') {
        for (int c = 0; c < SIZE; ++c) {
            int empty_r = SIZE - 1;
            for (int r = SIZE - 1; r >= 0; --r) {
                if (grid[r][c] != 0) {
                    if (r != empty_r) {
                        grid[empty_r][c] = grid[r][c];
                        grid[r][c] = 0;
                    }
                    empty_r--;
                }
            }
        }
    } else if (dir == 'L') {
        for (int r = 0; r < SIZE; ++r) {
            int empty_c = 0;
            for (int c = 0; c < SIZE; ++c) {
                if (grid[r][c] != 0) {
                    if (c != empty_c) {
                        grid[r][empty_c] = grid[r][c];
                        grid[r][c] = 0;
                    }
                    empty_c++;
                }
            }
        }
    } else if (dir == 'R') {
        for (int r = 0; r < SIZE; ++r) {
            int empty_c = SIZE - 1;
            for (int c = SIZE - 1; c >= 0; --c) {
                if (grid[r][c] != 0) {
                    if (c != empty_c) {
                        grid[r][empty_c] = grid[r][c];
                        grid[r][c] = 0;
                    }
                    empty_c--;
                }
            }
        }
    }
}

int evaluate(const Grid& grid) {
    int score = 0;
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid[r][c] == 0) continue;
            int f = grid[r][c];

            if (use_vertical_split) {
                int c_start = region_starts[f];
                int c_end = region_ends[f];
                if (c < c_start) score += W1 * (c_start - c);
                else if (c > c_end) score += W1 * (c - c_end);
            } else {
                int r_start = region_starts[f];
                int r_end = region_ends[f];
                if (r < r_start) score += W1 * (r_start - r);
                else if (r > r_end) score += W1 * (r - r_end);
            }

            int same_neighbors = 0;
            int diff_neighbors = 0;
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE && grid[nr][nc] != 0) {
                    if (grid[nr][nc] == f) same_neighbors++;
                    else diff_neighbors++;
                }
            }
            score -= W2 * same_neighbors * same_neighbors;
            score += W3 * diff_neighbors;
        }
    }
    return score;
}

char tie_break(const vector<char>& best_dirs, int r, int c, int f) {
    if (best_dirs.size() == 1) {
        return best_dirs[0];
    }
    
    vector<char> preferred;
    if (use_vertical_split) {
        if (c < region_starts[f]) preferred.push_back('R');
        if (c > region_ends[f]) preferred.push_back('L');
        if (r < SIZE / 2) preferred.push_back('B');
        else preferred.push_back('F');
    } else {
        if (r < region_starts[f]) preferred.push_back('B');
        if (r > region_ends[f]) preferred.push_back('F');
        if (c < SIZE / 2) preferred.push_back('R');
        else preferred.push_back('L');
    }

    for (char p_dir : preferred) {
        for (char b_dir : best_dirs) {
            if (p_dir == b_dir) {
                return p_dir;
            }
        }
    }
    
    return best_dirs[0];
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    for (int i = 0; i < 100; ++i) {
        cin >> flavors[i];
        d[flavors[i]]++;
    }

    setup_regions();

    Grid grid{};
    for (auto& row : grid) {
        row.fill(0);
    }

    for (int t = 0; t < 100; ++t) {
        int p;
        cin >> p;
        pair<int, int> pos = p_to_rc(grid, p);
        grid[pos.first][pos.second] = flavors[t];
        
        if (t == 99) break;

        char dirs[] = {'F', 'B', 'L', 'R'};
        int min_score = 1e9;
        vector<char> best_dirs;

        for (char dir : dirs) {
            Grid temp_grid = grid;
            apply_tilt(temp_grid, dir);
            int current_score = evaluate(temp_grid);

            if (current_score < min_score) {
                min_score = current_score;
                best_dirs.clear();
                best_dirs.push_back(dir);
            } else if (current_score == min_score) {
                best_dirs.push_back(dir);
            }
        }

        char best_dir = tie_break(best_dirs, pos.first, pos.second, flavors[t]);
        
        cout << best_dir << endl;
        
        apply_tilt(grid, best_dir);
    }

    return 0;
}