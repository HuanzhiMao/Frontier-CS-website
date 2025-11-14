#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <map>

using namespace std;

int n, m, k;
vector<string> initial_grid, target_grid, current_grid;
vector<vector<string>> presets;
vector<pair<int, int>> preset_dims;
vector<tuple<int, int, int>> operations;

void apply_swap(int r1, int c1, int r2, int c2) {
    swap(current_grid[r1 - 1][c1 - 1], current_grid[r2 - 1][c2 - 1]);
}

void apply_preset(int p_idx, int r, int c) {
    int np = preset_dims[p_idx - 1].first;
    int mp = preset_dims[p_idx - 1].second;
    for (int i = 0; i < np; ++i) {
        for (int j = 0; j < mp; ++j) {
            current_grid[r + i - 1][c + j - 1] = presets[p_idx - 1][i][j];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> k;

    initial_grid.resize(n);
    for (int i = 0; i < n; ++i) cin >> initial_grid[i];
    
    target_grid.resize(n);
    for (int i = 0; i < n; ++i) cin >> target_grid[i];
    
    presets.resize(k);
    preset_dims.resize(k);
    for (int i = 0; i < k; ++i) {
        int np, mp;
        cin >> np >> mp;
        preset_dims[i] = {np, mp};
        presets[i].resize(np);
        for (int j = 0; j < np; ++j) cin >> presets[i][j];
    }

    current_grid = initial_grid;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (current_grid[i - 1][j - 1] == target_grid[i - 1][j - 1]) {
                continue;
            }

            while (current_grid[i - 1][j - 1] != target_grid[i - 1][j - 1]) {
                if (operations.size() > 400000) { 
                    cout << -1 << endl;
                    return 0;
                }

                // Phase 1: Search and Swap
                pair<int, int> found_loc = {-1, -1};
                for (int r_search = i; r_search <= n; ++r_search) {
                    for (int c_search = 1; c_search <= m; ++c_search) {
                        if (r_search == i && c_search <= j) continue;
                        if (current_grid[r_search - 1][c_search - 1] == target_grid[i - 1][j - 1]) {
                            found_loc = {r_search, c_search};
                            goto found_jelly;
                        }
                    }
                }
                
            found_jelly:
                if (found_loc.first != -1) {
                    int r_start = found_loc.first;
                    int c_start = found_loc.second;
                    
                    for(int r = r_start; r > i; --r) {
                        operations.emplace_back(-3, r, c_start);
                        apply_swap(r, c_start, r - 1, c_start);
                    }
                    if (c_start > j) {
                        for (int c = c_start; c > j; --c) {
                            operations.emplace_back(-2, i, c);
                            apply_swap(i, c, i, c - 1);
                        }
                    } else { 
                        for (int c = c_start; c < j; ++c) {
                            operations.emplace_back(-1, i, c);
                            apply_swap(i, c, i, c + 1);
                        }
                    }
                    break; 
                }

                // Phase 2: Use Preset
                tuple<int, int, int> best_preset_op = {-1, -1, -1};
                int max_score = -1;

                for (int p = 1; p <= k; ++p) {
                    int np = preset_dims[p - 1].first;
                    int mp = preset_dims[p - 1].second;
                    bool has_needed = false;
                    for(int pr=0; pr<np; ++pr) {
                        for(int pc=0; pc<mp; ++pc) {
                            if (presets[p-1][pr][pc] == target_grid[i-1][j-1]) {
                                has_needed = true; break;
                            }
                        }
                        if(has_needed) break;
                    }
                    if(!has_needed) continue;
                    
                    for (int r_apply = 1; r_apply <= n - np + 1; ++r_apply) {
                        for (int c_apply = 1; c_apply <= m - mp + 1; ++c_apply) {
                            if (r_apply < i || (r_apply == i && c_apply < j)) continue;

                            int score = 0;
                            for (int pr = 0; pr < np; ++pr) {
                                for (int pc = 0; pc < mp; ++pc) {
                                    int cur_r = r_apply + pr;
                                    int cur_c = c_apply + pc;
                                    if (cur_r > i || (cur_r == i && cur_c >= j)) {
                                       if (presets[p-1][pr][pc] == target_grid[cur_r - 1][cur_c - 1]) {
                                           score++;
                                       }
                                    }
                                }
                            }
                            if (score > max_score) {
                                max_score = score;
                                best_preset_op = {p, r_apply, c_apply};
                            }
                        }
                    }
                }

                if (get<0>(best_preset_op) == -1) {
                    cout << -1 << endl;
                    return 0;
                }
                
                operations.push_back(best_preset_op);
                apply_preset(get<0>(best_preset_op), get<1>(best_preset_op), get<2>(best_preset_op));
            }
        }
    }

    cout << operations.size() << "\n";
    for (const auto& op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << "\n";
    }

    return 0;
}