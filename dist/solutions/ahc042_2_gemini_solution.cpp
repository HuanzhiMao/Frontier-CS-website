#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

const int N = 20;

struct Oni {
    int id;
    int r, c;
    bool removed;
};

struct Move {
    char dir;
    int line;
};

char initial_board[N][N];
std::vector<Oni> onis;
bool safe_dir[N][N][4]; // 0:U, 1:D, 2:L, 3:R

void precompute_safety() {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            // Check Up
            safe_dir[r][c][0] = true;
            for (int i = 0; i < r; ++i) {
                if (initial_board[i][c] == 'o') {
                    safe_dir[r][c][0] = false;
                    break;
                }
            }
            // Check Down
            safe_dir[r][c][1] = true;
            for (int i = r + 1; i < N; ++i) {
                if (initial_board[i][c] == 'o') {
                    safe_dir[r][c][1] = false;
                    break;
                }
            }
            // Check Left
            safe_dir[r][c][2] = true;
            for (int j = 0; j < c; ++j) {
                if (initial_board[r][j] == 'o') {
                    safe_dir[r][c][2] = false;
                    break;
                }
            }
            // Check Right
            safe_dir[r][c][3] = true;
            for (int j = c + 1; j < N; ++j) {
                if (initial_board[r][j] == 'o') {
                    safe_dir[r][c][3] = false;
                    break;
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_in;
    std::cin >> n_in;
    std::vector<std::string> C(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> C[i];
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            initial_board[i][j] = C[i][j];
            if (initial_board[i][j] == 'x') {
                onis.push_back({(int)onis.size(), i, j, false});
            }
        }
    }

    precompute_safety();

    std::vector<Move> solution;
    int onis_removed_count = 0;
    const int total_onis = onis.size();

    while (onis_removed_count < total_onis) {
        int best_cost = -1;
        std::vector<int> best_oni_indices;
        char best_dir = ' ';
        int best_line = -1;

        // Sweeps UP
        for (int c = 0; c < N; ++c) {
            std::vector<int> candidates;
            int max_r = -1;
            for (const auto& oni : onis) {
                if (!oni.removed && oni.c == c && safe_dir[oni.r][oni.c][0]) {
                    candidates.push_back(oni.id);
                    max_r = std::max(max_r, oni.r);
                }
            }
            if (!candidates.empty()) {
                int combined_cost = 2 * (max_r + 1);
                if (best_cost == -1 || 
                    (long long)combined_cost * best_oni_indices.size() < (long long)best_cost * candidates.size() ||
                    ((long long)combined_cost * best_oni_indices.size() == (long long)best_cost * candidates.size() && candidates.size() > best_oni_indices.size())) {
                    best_cost = combined_cost;
                    best_oni_indices = candidates;
                    best_dir = 'U';
                    best_line = c;
                }
            }
        }
        
        // Sweeps DOWN
        for (int c = 0; c < N; ++c) {
            std::vector<int> candidates;
            int min_r = N;
            for (const auto& oni : onis) {
                if (!oni.removed && oni.c == c && safe_dir[oni.r][oni.c][1]) {
                    candidates.push_back(oni.id);
                    min_r = std::min(min_r, oni.r);
                }
            }
            if (!candidates.empty()) {
                int combined_cost = 2 * (N - min_r);
                 if (best_cost == -1 || 
                    (long long)combined_cost * best_oni_indices.size() < (long long)best_cost * candidates.size() ||
                    ((long long)combined_cost * best_oni_indices.size() == (long long)best_cost * candidates.size() && candidates.size() > best_oni_indices.size())) {
                    best_cost = combined_cost;
                    best_oni_indices = candidates;
                    best_dir = 'D';
                    best_line = c;
                }
            }
        }

        // Sweeps LEFT
        for (int r = 0; r < N; ++r) {
            std::vector<int> candidates;
            int max_c = -1;
            for (const auto& oni : onis) {
                if (!oni.removed && oni.r == r && safe_dir[oni.r][oni.c][2]) {
                    candidates.push_back(oni.id);
                    max_c = std::max(max_c, oni.c);
                }
            }
            if (!candidates.empty()) {
                int combined_cost = 2 * (max_c + 1);
                if (best_cost == -1 || 
                    (long long)combined_cost * best_oni_indices.size() < (long long)best_cost * candidates.size() ||
                    ((long long)combined_cost * best_oni_indices.size() == (long long)best_cost * candidates.size() && candidates.size() > best_oni_indices.size())) {
                    best_cost = combined_cost;
                    best_oni_indices = candidates;
                    best_dir = 'L';
                    best_line = r;
                }
            }
        }

        // Sweeps RIGHT
        for (int r = 0; r < N; ++r) {
            std::vector<int> candidates;
            int min_c = N;
            for (const auto& oni : onis) {
                if (!oni.removed && oni.r == r && safe_dir[oni.r][oni.c][3]) {
                    candidates.push_back(oni.id);
                    min_c = std::min(min_c, oni.c);
                }
            }
            if (!candidates.empty()) {
                int combined_cost = 2 * (N - min_c);
                if (best_cost == -1 || 
                    (long long)combined_cost * best_oni_indices.size() < (long long)best_cost * candidates.size() ||
                    ((long long)combined_cost * best_oni_indices.size() == (long long)best_cost * candidates.size() && candidates.size() > best_oni_indices.size())) {
                    best_cost = combined_cost;
                    best_oni_indices = candidates;
                    best_dir = 'R';
                    best_line = r;
                }
            }
        }

        int num_moves = 0;
        char move_dir = ' ';
        char return_dir = ' ';
        
        switch(best_dir) {
            case 'U': {
                int max_r = -1;
                for(int id : best_oni_indices) max_r = std::max(max_r, onis[id].r);
                num_moves = max_r + 1;
                move_dir = 'U'; return_dir = 'D';
                break;
            }
            case 'D': {
                int min_r = N;
                for(int id : best_oni_indices) min_r = std::min(min_r, onis[id].r);
                num_moves = N - min_r;
                move_dir = 'D'; return_dir = 'U';
                break;
            }
            case 'L': {
                int max_c = -1;
                for(int id : best_oni_indices) max_c = std::max(max_c, onis[id].c);
                num_moves = max_c + 1;
                move_dir = 'L'; return_dir = 'R';
                break;
            }
            case 'R': {
                int min_c = N;
                for(int id : best_oni_indices) min_c = std::min(min_c, onis[id].c);
                num_moves = N - min_c;
                move_dir = 'R'; return_dir = 'L';
                break;
            }
        }

        for (int i = 0; i < num_moves; ++i) {
            solution.push_back({move_dir, best_line});
        }
        for (int i = 0; i < num_moves; ++i) {
            solution.push_back({return_dir, best_line});
        }

        for (int oni_idx : best_oni_indices) {
            if (!onis[oni_idx].removed) {
                onis[oni_idx].removed = true;
                onis_removed_count++;
            }
        }
    }
    
    for (const auto& move : solution) {
        std::cout << move.dir << " " << move.line << "\n";
    }

    return 0;
}