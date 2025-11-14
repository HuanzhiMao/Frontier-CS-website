#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

using namespace std;

const int N = 20;

struct Board {
    vector<string> grid;

    Board() : grid(N, string(N, '.')) {}

    void shift_row_left(int r) {
        for (int j = 0; j < N - 1; ++j) {
            grid[r][j] = grid[r][j + 1];
        }
        grid[r][N - 1] = '.';
    }

    void shift_row_right(int r) {
        for (int j = N - 1; j > 0; --j) {
            grid[r][j] = grid[r][j - 1];
        }
        grid[r][0] = '.';
    }

    void shift_col_up(int c) {
        for (int i = 0; i < N - 1; ++i) {
            grid[i][c] = grid[i + 1][c];
        }
        grid[N - 1][c] = '.';
    }

    void shift_col_down(int c) {
        for (int i = N - 1; i > 0; --i) {
            grid[i][c] = grid[i - 1][c];
        }
        grid[0][c] = '.';
    }
};

struct Move {
    char dir;
    int idx;
};

vector<Move> solution;
Board current_board;

void apply_and_record(char dir, int idx, int count) {
    for (int i = 0; i < count; ++i) {
        solution.push_back({dir, idx});
        if (dir == 'L') current_board.shift_row_left(idx);
        else if (dir == 'R') current_board.shift_row_right(idx);
        else if (dir == 'U') current_board.shift_col_up(idx);
        else if (dir == 'D') current_board.shift_col_down(idx);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy;
    cin >> n_dummy;
    for (int i = 0; i < N; ++i) {
        cin >> current_board.grid[i];
    }

    // Phase 1: Greedy removal of exposed Oni
    while (true) {
        pair<int, int> best_metric = {0, 0};
        char best_dir = ' ';
        int best_idx = -1, best_count = -1;

        for (int i = 0; i < N; ++i) {
            int min_o_col = N, max_o_col = -1;
            for (int j = 0; j < N; ++j) {
                if (current_board.grid[i][j] == 'o') {
                    min_o_col = min(min_o_col, j);
                    max_o_col = max(max_o_col, j);
                }
            }

            int oni_count = 0;
            for (int k = 0; k < min_o_col; ++k) {
                if (current_board.grid[i][k] == 'x') {
                    oni_count++;
                    pair<int, int> current_metric = {oni_count, -(k + 1)};
                    if (current_metric > best_metric) {
                        best_metric = current_metric;
                        best_dir = 'L';
                        best_idx = i;
                        best_count = k + 1;
                    }
                }
            }
            
            oni_count = 0;
            for (int k = N - 1; k > max_o_col; --k) {
                if (current_board.grid[i][k] == 'x') {
                    oni_count++;
                    pair<int, int> current_metric = {oni_count, -(N - k)};
                    if (current_metric > best_metric) {
                        best_metric = current_metric;
                        best_dir = 'R';
                        best_idx = i;
                        best_count = N - k;
                    }
                }
            }
        }

        for (int j = 0; j < N; ++j) {
            int min_o_row = N, max_o_row = -1;
            for (int i = 0; i < N; ++i) {
                if (current_board.grid[i][j] == 'o') {
                    min_o_row = min(min_o_row, i);
                    max_o_row = max(max_o_row, i);
                }
            }

            int oni_count = 0;
            for (int k = 0; k < min_o_row; ++k) {
                if (current_board.grid[k][j] == 'x') {
                    oni_count++;
                    pair<int, int> current_metric = {oni_count, -(k + 1)};
                    if (current_metric > best_metric) {
                        best_metric = current_metric;
                        best_dir = 'U';
                        best_idx = j;
                        best_count = k + 1;
                    }
                }
            }

            oni_count = 0;
            for (int k = N - 1; k > max_o_row; --k) {
                if (current_board.grid[k][j] == 'x') {
                    oni_count++;
                    pair<int, int> current_metric = {oni_count, -(N - k)};
                    if (current_metric > best_metric) {
                        best_metric = current_metric;
                        best_dir = 'D';
                        best_idx = j;
                        best_count = N - k;
                    }
                }
            }
        }
        
        if (best_metric.first == 0) {
            break;
        }

        apply_and_record(best_dir, best_idx, best_count);
    }
    
    // Phase 2: Targeted removal for remaining internal Oni
    while (true) {
        int r = -1, c = -1;
        for (int i = 0; i < N && r == -1; ++i) {
            for (int j = 0; j < N; ++j) {
                if (current_board.grid[i][j] == 'x') {
                    r = i;
                    c = j;
                    break;
                }
            }
        }
        if (r == -1) break;

        long long best_cost = -1;
        char best_dir = ' ';
        
        int blockers_up = 0; for (int i = 0; i < r; ++i) if (current_board.grid[i][c] == 'o') blockers_up++;
        long long cost_up = 2LL * (r + 1) + 2LL * blockers_up;
        best_cost = cost_up; best_dir = 'U';

        int blockers_down = 0; for (int i = r + 1; i < N; ++i) if (current_board.grid[i][c] == 'o') blockers_down++;
        long long cost_down = 2LL * (N - r) + 2LL * blockers_down;
        if (cost_down < best_cost) { best_cost = cost_down; best_dir = 'D'; }

        int blockers_left = 0; for (int j = 0; j < c; ++j) if (current_board.grid[r][j] == 'o') blockers_left++;
        long long cost_left = 2LL * (c + 1) + 2LL * blockers_left;
        if (cost_left < best_cost) { best_cost = cost_left; best_dir = 'L'; }

        int blockers_right = 0; for (int j = c + 1; j < N; ++j) if (current_board.grid[r][j] == 'o') blockers_right++;
        long long cost_right = 2LL * (N - c) + 2LL * blockers_right;
        if (cost_right < best_cost) { best_cost = cost_right; best_dir = 'R'; }
        
        vector<pair<int, char>> blocker_displacements;
        if (best_dir == 'U' || best_dir == 'D') {
            int start = (best_dir == 'U') ? 0 : r + 1;
            int end = (best_dir == 'U') ? r : N;
            for (int i = start; i < end; ++i) {
                if (current_board.grid[i][c] == 'o') {
                    int min_o_col = N;
                    for (int j = 0; j < N; ++j) if (current_board.grid[i][j] == 'o') min_o_col = min(min_o_col, j);
                    blocker_displacements.push_back({i, (min_o_col > 0) ? 'L' : 'R'});
                }
            }
        } else {
            int start = (best_dir == 'L') ? 0 : c + 1;
            int end = (best_dir == 'L') ? c : N;
            for (int j = start; j < end; ++j) {
                if (current_board.grid[r][j] == 'o') {
                    int min_o_row = N;
                    for (int i = 0; i < N; ++i) if (current_board.grid[i][j] == 'o') min_o_row = min(min_o_row, i);
                    blocker_displacements.push_back({j, (min_o_row > 0) ? 'U' : 'D'});
                }
            }
        }

        for (auto p : blocker_displacements) apply_and_record(p.second, p.first, 1);
        
        int count = 0;
        char restore_dir = ' ';
        if (best_dir == 'U') { count = r + 1; restore_dir = 'D'; }
        else if (best_dir == 'D') { count = N - r; restore_dir = 'U'; }
        else if (best_dir == 'L') { count = c + 1; restore_dir = 'R'; }
        else { count = N - c; restore_dir = 'L'; }
        
        int main_idx = (best_dir == 'U' || best_dir == 'D') ? c : r;
        apply_and_record(best_dir, main_idx, count);
        apply_and_record(restore_dir, main_idx, count);

        reverse(blocker_displacements.begin(), blocker_displacements.end());
        for (auto p : blocker_displacements) {
            char d = p.second;
            if (d == 'L') d = 'R'; else if (d == 'R') d = 'L';
            else if (d == 'U') d = 'D'; else d = 'U';
            apply_and_record(d, p.first, 1);
        }
    }

    for (const auto& m : solution) {
        cout << m.dir << " " << m.idx << "\n";
    }

    return 0;
}