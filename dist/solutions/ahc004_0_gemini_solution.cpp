#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>

// Constants
constexpr int N = 20;
constexpr int ALPHABET_SIZE = 8;
constexpr double TIME_LIMIT_SEC = 2.8;
constexpr int CONFLICT_WEIGHT = N * N;

// Globals
int M;
std::vector<std::string> S;
std::vector<int> S_len;

// Random number generator
struct Rng {
    std::mt19937 mt;
    Rng() : mt(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {}
    int operator()(int max) { return mt() % max; }
    int operator()(int min, int max) { return min + mt() % (max - min + 1); }
    double prob() { return std::uniform_real_distribution<double>(0.0, 1.0)(mt); }
};
Rng rng;

// Placement representation
struct Placement {
    int r, c, dir; // dir=0 horizontal, dir=1 vertical

    void randomize() {
        r = rng(N);
        c = rng(N);
        dir = rng(2);
    }
    
    bool operator==(const Placement& other) const {
        return r == other.r && c == other.c && dir == other.dir;
    }
};

// Main state for simulated annealing
struct State {
    std::vector<Placement> placements;
    int potential[N][N][ALPHABET_SIZE];
    int coverage[N][N];
    long long score;
    int total_conflicts;
    int filled_cells;

    State() {
        placements.resize(M);
        std::fill(&potential[0][0][0], &potential[0][0][0] + N * N * ALPHABET_SIZE, 0);
        std::fill(&coverage[0][0], &coverage[0][0] + N * N, 0);
        score = 0;
        total_conflicts = 0;
        filled_cells = 0;
    }

    void get_coords(int k, const Placement& p, int& out_r, int& out_c) const {
        if (p.dir == 0) { // horizontal
            out_r = p.r;
            out_c = (p.c + k) % N;
        } else { // vertical
            out_r = (p.r + k) % N;
            out_c = p.c;
        }
    }
    
    int get_conflict_at(int r, int c) const {
        int total = 0;
        int max_freq = 0;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            total += potential[r][c][i];
            if (potential[r][c][i] > max_freq) {
                max_freq = potential[r][c][i];
            }
        }
        return total > 0 ? total - max_freq : 0;
    }

    void update_cell(int r, int c, int char_idx, int delta) {
        total_conflicts -= get_conflict_at(r, c);
        
        if (coverage[r][c] == 1 && delta == -1) filled_cells--;
        if (coverage[r][c] == 0 && delta == 1) filled_cells++;

        potential[r][c][char_idx] += delta;
        coverage[r][c] += delta;

        total_conflicts += get_conflict_at(r, c);
    }
    
    void update_string_placement(int s_idx, const Placement& p, int delta) {
        for (int k = 0; k < S_len[s_idx]; ++k) {
            int r, c;
            get_coords(k, p, r, c);
            int char_idx = S[s_idx][k] - 'A';
            update_cell(r, c, char_idx, delta);
        }
    }
};

void solve() {
    int n_dummy;
    std::cin >> n_dummy >> M;
    S.resize(M);
    S_len.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> S[i];
        S_len[i] = S[i].length();
    }
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    State current_state;
    for (int i = 0; i < M; ++i) {
        current_state.placements[i].randomize();
        current_state.update_string_placement(i, current_state.placements[i], 1);
    }
    current_state.score = (long long)current_state.total_conflicts * CONFLICT_WEIGHT + current_state.filled_cells;
    
    std::vector<Placement> best_placements = current_state.placements;
    long long best_score = current_state.score;
    
    double T_start = 500.0, T_end = 0.1;
    
    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration<double>(current_time - start_time).count();
        if (elapsed_time > TIME_LIMIT_SEC) break;
        
        double progress = elapsed_time / TIME_LIMIT_SEC;
        double T = T_start * pow(T_end / T_start, progress);
        
        int s_idx = rng(M);
        Placement old_p = current_state.placements[s_idx];
        Placement new_p;
        new_p.randomize();
        if (old_p == new_p) continue;

        long long old_score = current_state.score;
        
        current_state.update_string_placement(s_idx, old_p, -1);
        current_state.update_string_placement(s_idx, new_p, 1);
        current_state.placements[s_idx] = new_p;
        current_state.score = (long long)current_state.total_conflicts * CONFLICT_WEIGHT + current_state.filled_cells;
        
        long long delta_score = current_state.score - old_score;

        if (delta_score <= 0 || rng.prob() < exp(-delta_score / T)) {
            if (current_state.score < best_score) {
                best_score = current_state.score;
                best_placements = current_state.placements;
            }
        } else {
            current_state.update_string_placement(s_idx, new_p, -1);
            current_state.update_string_placement(s_idx, old_p, 1);
            current_state.placements[s_idx] = old_p;
            current_state.score = old_score;
        }
    }
    
    State final_state;
    final_state.placements = best_placements;
    for(int i = 0; i < M; ++i) {
        final_state.update_string_placement(i, final_state.placements[i], 1);
    }

    std::vector<std::string> grid(N, std::string(N, '.'));
    
    if (final_state.total_conflicts == 0) {
        for (int i = 0; i < M; ++i) {
            const auto& p = final_state.placements[i];
            for (int k = 0; k < S_len[i]; ++k) {
                int r, c;
                final_state.get_coords(k, p, r, c);
                grid[r][c] = S[i][k];
            }
        }
    } else {
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                int max_freq = 0;
                int best_char_idx = -1;
                for (int i = 0; i < ALPHABET_SIZE; ++i) {
                    if (final_state.potential[r][c][i] > max_freq) {
                        max_freq = final_state.potential[r][c][i];
                        best_char_idx = i;
                    }
                }
                if (best_char_idx != -1) {
                    grid[r][c] = 'A' + best_char_idx;
                }
            }
        }
    }
    
    for (int i = 0; i < N; ++i) {
        std::cout << grid[i] << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}