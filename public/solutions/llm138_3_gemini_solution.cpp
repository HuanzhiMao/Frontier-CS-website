#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <algorithm>

using namespace std;

// Data structure to hold preset formula information
struct Preset {
    int id;
    int n, m;
    vector<string> matrix;
};

// Data structure to hold an operation
struct Operation {
    int op, r, c;
};

// Globals to hold problem configuration, accessible by helper functions
int N, M, K;
vector<string> target_grid;
vector<Preset> presets;

// Calculates how many cells a preset would make correct if applied at (r, c)
// relative to the target grid.
int calculate_match_score(const Preset& p, int r, int c) {
    int score = 0;
    for (int i = 0; i < p.n; ++i) {
        for (int j = 0; j < p.m; ++j) {
            if (target_grid[r + i][c + j] == p.matrix[i][j]) {
                score++;
            }
        }
    }
    return score;
}

// Queries a 2D prefix sum array to find the sum over a sub-rectangle
int query_prefix_sum(const vector<vector<int>>& ps, int r, int c, int h, int w) {
    int r2 = r + h;
    int c2 = c + w;
    return ps[r2][c2] - ps[r][c2] - ps[r2][c] + ps[r][c];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read problem size and initial/target configurations
    cin >> N >> M >> K;

    vector<string> initial_grid(N);
    for (int i = 0; i < N; ++i) cin >> initial_grid[i];
    
    target_grid.resize(N);
    for (int i = 0; i < N; ++i) cin >> target_grid[i];
    
    // Read preset formulas
    presets.resize(K);
    for (int i = 0; i < K; ++i) {
        presets[i].id = i + 1;
        cin >> presets[i].n >> presets[i].m;
        presets[i].matrix.resize(presets[i].n);
        for (int j = 0; j < presets[i].n; ++j) {
            cin >> presets[i].matrix[j];
        }
    }

    vector<Operation> operations;
    vector<string> current_grid = initial_grid;
    
    // --- Phase 1: Greedy Preset Application ---

    // Precompute match scores for all presets at all possible locations
    vector<vector<vector<int>>> match_scores(K);
    for (int i = 0; i < K; ++i) {
        match_scores[i].resize(N, vector<int>(M));
        for (int r = 0; r <= N - presets[i].n; ++r) {
            for (int c = 0; c <= M - presets[i].m; ++c) {
                match_scores[i][r][c] = calculate_match_score(presets[i], r, c);
            }
        }
    }

    // Iteratively apply the preset that gives the most improvement
    for (int iter = 0; iter < 400; ++iter) {
        int max_gain = 0;
        tuple<int, int, int> best_move = {-1, -1, -1};
        int best_preset_idx = -1;

        // Build a prefix sum array on the correctness of the current grid
        vector<vector<int>> is_correct(N, vector<int>(M));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                is_correct[i][j] = (current_grid[i][j] == target_grid[i][j]);
            }
        }

        vector<vector<int>> ps(N + 1, vector<int>(M + 1, 0));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                ps[i + 1][j + 1] = is_correct[i][j] + ps[i][j + 1] + ps[i + 1][j] - ps[i][j];
            }
        }

        // Find the best preset to apply
        for (int i = 0; i < K; ++i) {
            for (int r = 0; r <= N - presets[i].n; ++r) {
                for (int c = 0; c <= M - presets[i].m; ++c) {
                    int correct_before = query_prefix_sum(ps, r, c, presets[i].n, presets[i].m);
                    int gain = match_scores[i][r][c] - correct_before;
                    if (gain > max_gain) {
                        max_gain = gain;
                        best_move = {presets[i].id, r, c};
                        best_preset_idx = i;
                    }
                }
            }
        }

        if (max_gain <= 0) {
            break; // No more improvements possible
        }

        // Apply the best move found
        int p_id, r, c;
        tie(p_id, r, c) = best_move;
        operations.push_back({p_id, r + 1, c + 1});
        const auto& p = presets[best_preset_idx];
        for (int i = 0; i < p.n; ++i) {
            for (int j = 0; j < p.m; ++j) {
                current_grid[r + i][c + j] = p.matrix[i][j];
            }
        }
    }

    // --- Phase 2: Check Jelly Counts and Permute ---

    // Check if the multiset of jellies matches the target
    map<char, int> current_counts, target_counts;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            current_counts[current_grid[i][j]]++;
            target_counts[target_grid[i][j]]++;
        }
    }

    if (current_counts != target_counts) {
        cout << -1 << endl;
        return 0;
    }

    // If counts match, rearrange jellies using swaps
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (current_grid[i][j] != target_grid[i][j]) {
                // Find the required jelly in the unprocessed part of the grid
                int found_r = -1, found_c = -1;
                for (int r_search = i; r_search < N; ++r_search) {
                    for (int c_search = (r_search == i ? j + 1 : 0); c_search < M; ++c_search) {
                        if (current_grid[r_search][c_search] == target_grid[i][j]) {
                            found_r = r_search;
                            found_c = c_search;
                            goto found_jelly;
                        }
                    }
                }

            found_jelly:
                // Move the found jelly to position (i, j) with swaps
                if (found_c > j) {
                    for (int c = found_c; c > j; --c) {
                        swap(current_grid[found_r][c], current_grid[found_r][c - 1]);
                        operations.push_back({-2, found_r + 1, c + 1});
                    }
                } else {
                    for (int c = found_c; c < j; ++c) {
                        swap(current_grid[found_r][c], current_grid[found_r][c + 1]);
                        operations.push_back({-1, found_r + 1, c + 1});
                    }
                }
                for (int r = found_r; r > i; --r) {
                    swap(current_grid[r][j], current_grid[r - 1][j]);
                    operations.push_back({-3, r + 1, j + 1});
                }
            }
        }
    }
    
    // Output the results
    cout << operations.size() << endl;
    for (const auto& op : operations) {
        cout << op.op << " " << op.r << " " << op.c << endl;
    }

    return 0;
}