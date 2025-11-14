#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <map>
#include <algorithm>

using namespace std;

// Globals to be accessible by helper functions
int n, m, k;
vector<pair<int, int>> preset_dims;
vector<vector<string>> presets;

struct Operation {
    int op, r, c;
};

// Apply an operation to the grid. Coordinates are 1-based.
void apply_op(vector<string>& grid, const Operation& op_in) {
    Operation op = op_in;
    if (op.op > 0) { // Preset
        int p_idx = op.op - 1;
        int np = preset_dims[p_idx].first;
        int mp = preset_dims[p_idx].second;
        for (int i = 0; i < np; ++i) {
            for (int j = 0; j < mp; ++j) {
                grid[op.r - 1 + i][op.c - 1 + j] = presets[p_idx][i][j];
            }
        }
    } else if (op.op == 0) { // Rotate
        int r = op.r - 1, c = op.c - 1;
        char temp = grid[r][c];
        grid[r][c] = grid[r + 1][c];
        grid[r + 1][c] = grid[r + 1][c + 1];
        grid[r + 1][c + 1] = grid[r][c + 1];
        grid[r][c + 1] = temp;
    } else { // Swap
        int r1 = op.r - 1, c1 = op.c - 1;
        int r2 = r1, c2 = c1;
        if (op.op == -1) c2++;      // (r,c) with (r,c+1)
        else if (op.op == -2) c2--; // (r,c) with (r,c-1)
        else if (op.op == -3) r2--; // (r,c) with (r-1,c)
        else if (op.op == -4) r2++; // (r,c) with (r+1,c)
        swap(grid[r1][c1], grid[r2][c2]);
    }
}

map<char, int> get_counts(const vector<string>& grid) {
    map<char, int> counts;
    for (const auto& row : grid) {
        for (char ch : row) {
            counts[ch]++;
        }
    }
    return counts;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> k;
    vector<string> initial(n), target(n);

    for (int i = 0; i < n; ++i) cin >> initial[i];
    for (int i = 0; i < n; ++i) cin >> target[i];

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

    vector<string> current = initial;
    vector<Operation> ops;

    for (int op_count = 0; op_count < 400; ++op_count) {
        int best_score = 0;
        Operation best_op = {-5, -1, -1};

        pair<int, int> mismatch = {-1, -1};
        bool all_matched = true;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                if (current[r][c] != target[r][c]) {
                    mismatch = {r, c};
                    all_matched = false;
                    break;
                }
            }
            if (!all_matched) break;
        }
        
        if (all_matched) break;

        for (int p = 0; p < k; ++p) {
            int np = preset_dims[p].first;
            int mp = preset_dims[p].second;
            for (int dr = 0; dr < np; ++dr) {
                for (int dc = 0; dc < mp; ++dc) {
                    int r = mismatch.first - dr;
                    int c = mismatch.second - dc;

                    if (r >= 0 && r + np <= n && c >= 0 && c + mp <= m) {
                        if (presets[p][dr][dc] != target[mismatch.first][mismatch.second]) continue;
                        
                        int current_score = 0;
                        for (int i = 0; i < np; ++i) {
                            for (int j = 0; j < mp; ++j) {
                                bool was_correct = (current[r + i][c + j] == target[r + i][c + j]);
                                bool will_be_correct = (presets[p][i][j] == target[r + i][c + j]);
                                if (!was_correct && will_be_correct) {
                                    current_score++;
                                } else if (was_correct && !will_be_correct) {
                                    current_score--;
                                }
                            }
                        }
                        if (current_score > best_score) {
                            best_score = current_score;
                            best_op = {p + 1, r + 1, c + 1};
                        }
                    }
                }
            }
        }
        
        if (best_op.op != -5) {
            ops.push_back(best_op);
            apply_op(current, best_op);
        } else {
             break;
        }
    }

    if (get_counts(current) != get_counts(target)) {
        cout << -1 << endl;
        return 0;
    }
    
    map<char, vector<int>> current_pos_map, target_pos_map;
    for(int i=0; i<n; ++i) for(int j=0; j<m; ++j) {
        current_pos_map[current[i][j]].push_back(i*m+j);
        target_pos_map[target[i][j]].push_back(i*m+j);
    }

    vector<int> current_perm(n * m);
    for(auto const& [key, val] : current_pos_map) {
        for(size_t i=0; i < val.size(); ++i) {
            current_perm[val[i]] = target_pos_map[key][i];
        }
    }

    int cycles = 0;
    vector<bool> visited(n*m, false);
    for(int i=0; i<n*m; ++i) {
        if(!visited[i]) {
            cycles++;
            int j=i;
            while(!visited[j]) {
                visited[j] = true;
                j = current_perm[j];
            }
        }
    }

    if ((long long)(n * m - cycles) % 2 != 0) {
        Operation op = {0, 1, 1};
        ops.push_back(op);
        apply_op(current, op);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (current[i][j] != target[i][j]) {
                int r_found = -1, c_found = -1;
                bool found_swap = false;
                for (int r = i; r < n; ++r) {
                    for (int c = (r == i ? j + 1 : 0); c < m; ++c) {
                        if (current[r][c] == target[i][j]) {
                           r_found = r;
                           c_found = c;
                           found_swap = true;
                           break;
                        }
                    }
                    if(found_swap) break;
                }
                
                for (int k = r_found; k > i; --k) {
                    Operation op = {-3, k + 1, c_found + 1};
                    ops.push_back(op);
                    apply_op(current, op);
                }
                if (c_found > j) {
                    for (int k = c_found; k > j; --k) {
                        Operation op = {-2, i + 1, k + 1};
                        ops.push_back(op);
                        apply_op(current, op);
                    }
                } else if (c_found < j) {
                    for (int k = c_found; k < j; ++k) {
                        Operation op = {-1, i + 1, k + 1};
                        ops.push_back(op);
                        apply_op(current, op);
                    }
                }
            }
        }
    }

    cout << ops.size() << endl;
    for (const auto& op : ops) {
        cout << op.op << " " << op.r << " " << op.c << endl;
    }

    return 0;
}