#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

const int N = 20;

struct Oni {
    int r, c;
};

struct Move {
    char dir;
    int index;
    int shifts;
    std::vector<Oni> removed_oni;

    bool operator<(const Move& other) const {
        long long cost_per_oni_1 = (long long)this->shifts * other.removed_oni.size();
        long long cost_per_oni_2 = (long long)other.shifts * this->removed_oni.size();
        if (cost_per_oni_1 != cost_per_oni_2) {
            return cost_per_oni_1 < cost_per_oni_2;
        }
        if (this->removed_oni.size() != other.removed_oni.size()) {
            return this->removed_oni.size() > other.removed_oni.size();
        }
        return this->shifts < other.shifts;
    }
};

bool is_oni[N][N];
std::vector<int> fuku_in_row[N]; // Fukunokami columns in each row
std::vector<int> fuku_in_col[N]; // Fukunokami rows in each column
int oni_count = 0;

void apply_move(const Move& move, std::vector<std::pair<char, int>>& solution) {
    for (const auto& oni : move.removed_oni) {
        if(is_oni[oni.r][oni.c]){
            is_oni[oni.r][oni.c] = false;
            oni_count--;
        }
    }

    char restore_dir;
    if (move.dir == 'L') restore_dir = 'R';
    else if (move.dir == 'R') restore_dir = 'L';
    else if (move.dir == 'U') restore_dir = 'D';
    else restore_dir = 'U';

    for (int i = 0; i < move.shifts; ++i) {
        solution.push_back({move.dir, move.index});
    }
    for (int i = 0; i < move.shifts; ++i) {
        solution.push_back({restore_dir, move.index});
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_val;
    std::cin >> n_val;

    for (int i = 0; i < N; ++i) {
        std::string row_str;
        std::cin >> row_str;
        for (int j = 0; j < N; ++j) {
            if (row_str[j] == 'x') {
                is_oni[i][j] = true;
                oni_count++;
            } else if (row_str[j] == 'o') {
                fuku_in_row[i].push_back(j);
                fuku_in_col[j].push_back(i);
            }
        }
    }
    
    for(int i = 0; i < N; ++i) {
        std::sort(fuku_in_row[i].begin(), fuku_in_row[i].end());
        std::sort(fuku_in_col[i].begin(), fuku_in_col[i].end());
    }

    std::vector<std::pair<char, int>> solution;

    while (oni_count > 0) {
        std::vector<Move> candidates;

        // Rows
        for (int i = 0; i < N; ++i) {
            std::vector<Oni> current_oni;
            int min_c = N, max_c = -1;
            for (int j = 0; j < N; ++j) {
                if (is_oni[i][j]) {
                    current_oni.push_back({i, j});
                    min_c = std::min(min_c, j);
                    max_c = std::max(max_c, j);
                }
            }

            if (current_oni.empty()) continue;

            // Shift Left
            bool can_shift_left = true;
            if (!fuku_in_row[i].empty() && fuku_in_row[i][0] <= max_c) {
                can_shift_left = false;
            }
            if (can_shift_left) {
                int shifts = max_c + 1;
                candidates.push_back({'L', i, shifts, current_oni});
            }

            // Shift Right
            bool can_shift_right = true;
            if (!fuku_in_row[i].empty() && fuku_in_row[i].back() >= min_c) {
                can_shift_right = false;
            }
            if (can_shift_right) {
                int shifts = N - min_c;
                candidates.push_back({'R', i, shifts, current_oni});
            }
        }

        // Columns
        for (int j = 0; j < N; ++j) {
            std::vector<Oni> current_oni;
            int min_r = N, max_r = -1;
            for (int i = 0; i < N; ++i) {
                if (is_oni[i][j]) {
                    current_oni.push_back({i, j});
                    min_r = std::min(min_r, i);
                    max_r = std::max(max_r, i);
                }
            }

            if (current_oni.empty()) continue;

            // Shift Up
            bool can_shift_up = true;
            if (!fuku_in_col[j].empty() && fuku_in_col[j][0] <= max_r) {
                can_shift_up = false;
            }
            if (can_shift_up) {
                int shifts = max_r + 1;
                candidates.push_back({'U', j, shifts, current_oni});
            }

            // Shift Down
            bool can_shift_down = true;
            if (!fuku_in_col[j].empty() && fuku_in_col[j].back() >= min_r) {
                can_shift_down = false;
            }
            if (can_shift_down) {
                int shifts = N - min_r;
                candidates.push_back({'D', j, shifts, current_oni});
            }
        }
        
        std::sort(candidates.begin(), candidates.end());
        apply_move(candidates[0], solution);
    }

    for (const auto& op : solution) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}