#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cmath>
#include <cstring>
#include <numeric>
#include <algorithm>

const int N = 20;
int M;
std::vector<std::string> S;

struct Placement {
    int r, c, dir; // dir 0: horizontal, 1: vertical
};

std::vector<Placement> placements;
std::vector<Placement> best_placements;

int char_counts[N][N][8];
int num_options[N][N];
int conflicts;
int best_conflicts;

std::mt19937 rng;

inline int ctoi(char c) { return c - 'A'; }
inline char itoc(int i) { return 'A' + i; }

void update_cell_score(int r, int c, int char_idx, int delta) {
    bool was_conflicted = num_options[r][c] > 1;

    if (char_counts[r][c][char_idx] == 0 && delta > 0) {
        num_options[r][c]++;
    }
    char_counts[r][c][char_idx] += delta;
    if (char_counts[r][c][char_idx] == 0 && delta < 0) {
        num_options[r][c]--;
    }

    bool is_conflicted = num_options[r][c] > 1;

    if (was_conflicted != is_conflicted) {
        if (is_conflicted) {
            conflicts++;
        } else {
            conflicts--;
        }
    }
}

void apply_placement_delta(int s_idx, const Placement& p, int delta) {
    const auto& s = S[s_idx];
    int r = p.r, c = p.c;
    if (p.dir == 0) { // Horizontal
        for (size_t i = 0; i < s.length(); ++i) {
            update_cell_score(r, (c + i) % N, ctoi(s[i]), delta);
        }
    } else { // Vertical
        for (size_t i = 0; i < s.length(); ++i) {
            update_cell_score((r + i) % N, c, ctoi(s[i]), delta);
        }
    }
}

void solve() {
    int N_in;
    std::cin >> N_in >> M;
    S.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> S[i];
    }
    
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    placements.resize(M);
    for (int i = 0; i < M; ++i) {
        placements[i] = {
            std::uniform_int_distribution<int>(0, N - 1)(rng),
            std::uniform_int_distribution<int>(0, N - 1)(rng),
            std::uniform_int_distribution<int>(0, 1)(rng)
        };
    }

    std::memset(char_counts, 0, sizeof(char_counts));
    std::memset(num_options, 0, sizeof(num_options));
    conflicts = 0;
    for (int i = 0; i < M; ++i) {
        apply_placement_delta(i, placements[i], 1);
    }
    best_conflicts = conflicts;
    best_placements = placements;

    auto start_time = std::chrono::steady_clock::now();
    const double TIME_LIMIT_MS = 2950.0;
    double temp_start = 5.0, temp_end = 0.01;

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(current_time - start_time).count();
        if (elapsed_ms > TIME_LIMIT_MS) break;

        double progress = elapsed_ms / TIME_LIMIT_MS;
        double temp = temp_start * pow(temp_end / temp_start, progress);

        int s_idx = std::uniform_int_distribution<int>(0, M - 1)(rng);

        Placement old_p = placements[s_idx];
        Placement new_p;

        if (std::uniform_real_distribution<double>()(rng) < 0.1) { // small move
            new_p = old_p;
            int type = std::uniform_int_distribution<int>(0, 2)(rng);
            if(type == 0) new_p.r = (new_p.r + (int)(rng() % 3) - 1 + N) % N;
            else if(type == 1) new_p.c = (new_p.c + (int)(rng() % 3) - 1 + N) % N;
            else new_p.dir = 1 - new_p.dir;
        } else { // big move
             new_p = {
                std::uniform_int_distribution<int>(0, N - 1)(rng),
                std::uniform_int_distribution<int>(0, N - 1)(rng),
                std::uniform_int_distribution<int>(0, 1)(rng)
            };
        }

        int old_conflicts_val = conflicts;

        apply_placement_delta(s_idx, old_p, -1);
        apply_placement_delta(s_idx, new_p, 1);
        
        int delta_score = conflicts - old_conflicts_val;
        
        if (delta_score <= 0 || std::uniform_real_distribution<double>(0.0, 1.0)(rng) < exp(-delta_score / temp)) {
            placements[s_idx] = new_p;
            if (conflicts < best_conflicts) {
                best_conflicts = conflicts;
                best_placements = placements;
            }
        } else {
            apply_placement_delta(s_idx, new_p, -1);
            apply_placement_delta(s_idx, old_p, 1);
        }
    }

    std::memset(char_counts, 0, sizeof(char_counts));
    for (int i = 0; i < M; ++i) {
        const auto& p = best_placements[i];
        const auto& s = S[i];
        if (p.dir == 0) {
            for (size_t j = 0; j < s.length(); ++j) {
                char_counts[p.r][(p.c + j) % N][ctoi(s[j])]++;
            }
        } else {
            for (size_t j = 0; j < s.length(); ++j) {
                char_counts[(p.r + j) % N][p.c][ctoi(s[j])]++;
            }
        }
    }

    std::vector<std::string> grid(N, std::string(N, '.'));
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int max_count = 0;
            int best_char_idx = -1;
            for (int i = 0; i < 8; ++i) {
                if (char_counts[r][c][i] > max_count) {
                    max_count = char_counts[r][c][i];
                    best_char_idx = i;
                }
            }
            if (best_char_idx != -1) {
                grid[r][c] = itoc(best_char_idx);
            }
        }
    }

    if (best_conflicts == 0) {
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (grid[r][c] == '.') {
                    grid[r][c] = itoc(std::uniform_int_distribution<int>(0, 7)(rng));
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