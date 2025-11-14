#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

// All internal logic uses 0-based indexing.
// I/O is 1-based.

int n, m, k;
vector<string> target_grid;
vector<pair<int, int>> preset_dims;
vector<vector<string>> presets;
vector<tuple<int, int, int>> operations;

// Function to convert char to a unique int from 0-61
int char_to_int(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 26;
    return c - '0' + 52;
}

// Function to get multiset counts of a grid
void get_counts(const vector<string>& grid, vector<int>& counts) {
    fill(counts.begin(), counts.end(), 0);
    for (const auto& row : grid) {
        for (char c : row) {
            counts[char_to_int(c)]++;
        }
    }
}

// Apply a preset operation
void apply_preset(vector<string>& grid, int p_idx, int r, int c) {
    int np = preset_dims[p_idx].first;
    int mp = preset_dims[p_idx].second;
    for (int i = 0; i < np; ++i) {
        for (int j = 0; j < mp; ++j) {
            grid[r + i][c + j] = presets[p_idx][i][j];
        }
    }
    operations.emplace_back(p_idx + 1, r + 1, c + 1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> k;

    vector<string> initial_grid(n);
    for (int i = 0; i < n; ++i) cin >> initial_grid[i];

    target_grid.resize(n);
    for (int i = 0; i < n; ++i) cin >> target_grid[i];

    preset_dims.resize(k);
    presets.resize(k);
    for (int i = 0; i < k; ++i) {
        int np, mp;
        cin >> np >> mp;
        preset_dims[i] = {np, mp};
        presets[i].resize(np);
        for (int j = 0; j < np; ++j) {
            cin >> presets[i][j];
        }
    }

    vector<string> current_grid = initial_grid;

    // Part 1: Greedily apply presets based on net gain of correct cells
    for (int iter = 0; iter < 400; ++iter) {
        int best_net_gain = 0;
        int best_p = -1, best_r = -1, best_c = -1;

        for (int p = 0; p < k; ++p) {
            int np = preset_dims[p].first;
            int mp = preset_dims[p].second;
            for (int r = 0; r <= n - np; ++r) {
                for (int c = 0; c <= m - mp; ++c) {
                    int current_gain = 0;
                    for (int i = 0; i < np; ++i) {
                        for (int j = 0; j < mp; ++j) {
                            if (presets[p][i][j] == target_grid[r + i][c + j]) {
                                current_gain++;
                            }
                            if (current_grid[r + i][c + j] == target_grid[r + i][c + j]) {
                                current_gain--;
                            }
                        }
                    }
                    if (current_gain > best_net_gain) {
                        best_net_gain = current_gain;
                        best_p = p;
                        best_r = r;
                        best_c = c;
                    }
                }
            }
        }

        if (best_net_gain > 0) {
            apply_preset(current_grid, best_p, best_r, best_c);
        } else {
            break;
        }
    }

    // After presets, check if multisets match. If not, unsolvable.
    vector<int> current_counts(62);
    vector<int> target_counts(62);
    get_counts(current_grid, current_counts);
    get_counts(target_grid, target_counts);

    if (current_counts != target_counts) {
        cout << -1 << endl;
        return 0;
    }

    // Part 2: Permute jellies into their final positions
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (current_grid[i][j] != target_grid[i][j]) {
                int found_r = -1, found_c = -1;
                for (int r = i; r < n; ++r) {
                    int start_c = (r == i) ? j + 1 : 0;
                    for (int c = start_c; c < m; ++c) {
                        if (current_grid[r][c] == target_grid[i][j]) {
                            found_r = r;
                            found_c = c;
                            goto found_jelly;
                        }
                    }
                }
            found_jelly:
                int curr_r = found_r, curr_c = found_c;
                
                // Move horizontally to target column j
                while (curr_c > j) {
                    swap(current_grid[curr_r][curr_c], current_grid[curr_r][curr_c - 1]);
                    operations.emplace_back(-2, curr_r + 1, curr_c + 1);
                    curr_c--;
                }
                while (curr_c < j) {
                    swap(current_grid[curr_r][curr_c], current_grid[curr_r][curr_c + 1]);
                    operations.emplace_back(-1, curr_r + 1, curr_c + 1);
                    curr_c++;
                }
                
                // Move vertically to target row i
                while (curr_r > i) {
                    swap(current_grid[curr_r][j], current_grid[curr_r - 1][j]);
                    operations.emplace_back(-3, curr_r + 1, j + 1);
                    curr_r--;
                }
            }
        }
    }

    cout << operations.size() << endl;
    for (const auto& op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << endl;
    }

    return 0;
}