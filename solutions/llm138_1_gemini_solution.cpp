#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

struct Move {
    int op, r, c;
};

struct Preset {
    int id;
    int n, m;
    vector<string> grid;
    map<char, int> counts;
};

int n, m, k;
vector<string> initial_grid, target_grid;
vector<Preset> presets;
vector<Move> moves;

vector<string> current_grid;

map<char, int> get_counts(const vector<string>& grid) {
    map<char, int> counts;
    for (const auto& row : grid) {
        for (char cell : row) {
            counts[cell]++;
        }
    }
    return counts;
}

map<char, int> get_subgrid_counts(int r_start, int c_start, int h, int w) {
    map<char, int> counts;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            counts[current_grid[r_start + i][c_start + j]]++;
        }
    }
    return counts;
}

long long calculate_dist(const map<char, int>& c1, const map<char, int>& c2) {
    map<char, int> temp_c1 = c1;
    map<char, int> temp_c2 = c2;
    long long dist = 0;
    
    for(auto const& [key, val] : temp_c1) {
        if(temp_c2.count(key)) {
            dist += abs(val - temp_c2[key]);
            temp_c2.erase(key);
        } else {
            dist += val;
        }
    }
    for(auto const& [key, val] : temp_c2) {
        dist += val;
    }
    return dist;
}

void apply_preset(int p_idx, int r, int c) {
    moves.push_back({presets[p_idx].id, r + 1, c + 1});
    for (int i = 0; i < presets[p_idx].n; ++i) {
        for (int j = 0; j < presets[p_idx].m; ++j) {
            current_grid[r + i][c + j] = presets[p_idx].grid[i][j];
        }
    }
}

void apply_swap(int r1, int c1, int r2, int c2) {
    swap(current_grid[r1][c1], current_grid[r2][c2]);
    if (r2 == r1 + 1) { // down
        moves.push_back({-4, r1 + 1, c1 + 1});
    } else if (r2 == r1 - 1) { // up
        moves.push_back({-3, r2 + 1, c2 + 1});
    } else if (c2 == c1 + 1) { // right
        moves.push_back({-1, r1 + 1, c1 + 1});
    } else { // left
        moves.push_back({-2, r1 + 1, c1 + 1});
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> k;
    initial_grid.resize(n);
    target_grid.resize(n);

    for (int i = 0; i < n; ++i) cin >> initial_grid[i];
    for (int i = 0; i < n; ++i) cin >> target_grid[i];
    
    presets.resize(k);
    for (int i = 0; i < k; ++i) {
        presets[i].id = i + 1;
        cin >> presets[i].n >> presets[i].m;
        presets[i].grid.resize(presets[i].n);
        for (int j = 0; j < presets[i].n; ++j) {
            cin >> presets[i].grid[j];
        }
        presets[i].counts = get_counts(presets[i].grid);
    }

    current_grid = initial_grid;
    map<char, int> target_counts = get_counts(target_grid);

    for (int iter = 0; iter < 400; ++iter) {
        map<char, int> current_counts = get_counts(current_grid);
        long long min_dist = calculate_dist(current_counts, target_counts);

        if (min_dist == 0) {
            break;
        }

        int best_p_idx = -1, best_r = -1, best_c = -1;

        for (int p_idx = 0; p_idx < k; ++p_idx) {
            int pn = presets[p_idx].n;
            int pm = presets[p_idx].m;
            for (int r = 0; r <= n - pn; ++r) {
                map<char, int> removed_counts = get_subgrid_counts(r, 0, pn, pm);
                for (int c = 0; c <= m - pm; ++c) {
                    if (c > 0) {
                        for (int i = 0; i < pn; ++i) {
                            removed_counts[current_grid[r+i][c-1]]--;
                            if (removed_counts[current_grid[r+i][c-1]] == 0) {
                                removed_counts.erase(current_grid[r+i][c-1]);
                            }
                            removed_counts[current_grid[r+i][c+pm-1]]++;
                        }
                    }
                    
                    map<char, int> next_counts = current_counts;
                    for (auto const& [key, val] : removed_counts) {
                        next_counts[key] -= val;
                    }
                    for (auto const& [key, val] : presets[p_idx].counts) {
                        next_counts[key] += val;
                    }

                    long long new_dist = calculate_dist(next_counts, target_counts);
                    if (new_dist < min_dist) {
                        min_dist = new_dist;
                        best_p_idx = p_idx;
                        best_r = r;
                        best_c = c;
                    }
                }
            }
        }
        
        if (best_p_idx != -1) {
            apply_preset(best_p_idx, best_r, best_c);
        } else {
            break;
        }
    }

    map<char, int> final_counts = get_counts(current_grid);
    if (final_counts != target_counts) {
        cout << -1 << endl;
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (current_grid[i][j] != target_grid[i][j]) {
                int found_r = -1, found_c = -1;
                bool is_found = false;
                for (int r = i; r < n; ++r) {
                    for (int c = (r == i ? j + 1 : 0); c < m; ++c) {
                        if (current_grid[r][c] == target_grid[i][j]) {
                            found_r = r;
                            found_c = c;
                            is_found = true;
                            break;
                        }
                    }
                    if(is_found) break;
                }
            
                int cur_r = found_r, cur_c = found_c;
                while (cur_c > j) {
                    apply_swap(cur_r, cur_c, cur_r, cur_c - 1);
                    cur_c--;
                }
                while (cur_r > i) {
                    apply_swap(cur_r, cur_c, cur_r - 1, cur_c);
                    cur_r--;
                }
            }
        }
    }
    
    cout << moves.size() << endl;
    for (const auto& move : moves) {
        cout << move.op << " " << move.r << " " << move.c << endl;
    }

    return 0;
}