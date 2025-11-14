#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <utility>

// --- Timer ---
auto start_time = std::chrono::high_resolution_clock::now();
double get_time() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time).count();
}

// --- Random Number Generator ---
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int rand_int(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rng);
}
double rand_double(double min, double max) {
    return std::uniform_real_distribution<double>(min, max)(rng);
}

// --- Problem Constants and Globals ---
const int N = 20;
const double TIME_LIMIT = 2.9;
int M;
std::vector<std::string> S;
const int K = N * N;

struct Placement {
    int r, c, d; // d=0: horizontal, d=1: vertical
};

// --- State Variables ---
std::vector<Placement> placements;
std::vector<Placement> best_placements;
int counts[N][N][8];
long long current_energy;
long long best_energy;

// --- Helper Functions ---
int char_to_int(char ch) { return ch - 'A'; }
char int_to_char(int i) { return 'A' + i; }

long long calculate_cell_energy(int r, int c) {
    int total = 0;
    int max_freq = 0;
    for (int i = 0; i < 8; ++i) {
        total += counts[r][c][i];
        if (counts[r][c][i] > max_freq) {
            max_freq = counts[r][c][i];
        }
    }
    long long conflicts = total - max_freq;
    long long dots = (total == 0);
    return K * conflicts - dots;
}

long long calculate_total_energy() {
    long long total_energy = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            total_energy += calculate_cell_energy(i, j);
        }
    }
    return total_energy;
}

// --- Main Logic ---
void greedy_init() {
    placements.resize(M);
    std::vector<std::vector<char>> grid(N, std::vector<char>(N, '.'));
    
    std::vector<int> p(M);
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](int i, int j) {
        return S[i].length() > S[j].length();
    });

    for (int i_idx : p) {
        const auto& s = S[i_idx];
        long long best_score = -2e18; // Use a very small number
        Placement best_p = {0, 0, 0};

        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                for (int d = 0; d < 2; ++d) {
                    long long current_score = 0;
                    if (d == 0) { // horizontal
                        for (size_t k = 0; k < s.length(); ++k) {
                            char grid_char = grid[r][(c + k) % N];
                            if (grid_char != '.') {
                                if (grid_char == s[k]) current_score += 2;
                                else current_score -= 100;
                            } else {
                                current_score += 1;
                            }
                        }
                    } else { // vertical
                        for (size_t k = 0; k < s.length(); ++k) {
                            char grid_char = grid[(r + k) % N][c];
                            if (grid_char != '.') {
                                if (grid_char == s[k]) current_score += 2;
                                else current_score -= 100;
                            } else {
                                current_score += 1;
                            }
                        }
                    }
                    if (current_score > best_score) {
                        best_score = current_score;
                        best_p = {r, c, d};
                    }
                }
            }
        }
        placements[i_idx] = best_p;
        if (best_p.d == 0) {
            for (size_t k = 0; k < s.length(); ++k) grid[best_p.r][(best_p.c + k) % N] = s[k];
        } else {
            for (size_t k = 0; k < s.length(); ++k) grid[(best_p.r + k) % N][best_p.c] = s[k];
        }
    }

    std::fill(&counts[0][0][0], &counts[0][0][0] + N * N * 8, 0);
    for (int i = 0; i < M; ++i) {
        const auto& s = S[i];
        const auto& p = placements[i];
        if (p.d == 0) { // horizontal
            for (size_t k = 0; k < s.length(); ++k) counts[p.r][(p.c + k) % N][char_to_int(s[k])]++;
        } else { // vertical
            for (size_t k = 0; k < s.length(); ++k) counts[(p.r + k) % N][p.c][char_to_int(s[k])]++;
        }
    }
}

void solve() {
    greedy_init();

    current_energy = calculate_total_energy();
    best_energy = current_energy;
    best_placements = placements;

    double T_start = 200.0;
    double T_end = 0.1;
    
    bool visited_cells[N][N];

    while (true) {
        double elapsed_time = get_time();
        if (elapsed_time > TIME_LIMIT) break;

        double T = T_start + (T_end - T_start) * elapsed_time / TIME_LIMIT;
        
        int s_idx = rand_int(0, M - 1);
        Placement p_old = placements[s_idx];
        Placement p_new = {rand_int(0, N - 1), rand_int(0, N - 1), rand_int(0, 1)};

        if (p_old.r == p_new.r && p_old.c == p_new.c && p_old.d == p_new.d) continue;

        const auto& s = S[s_idx];
        std::vector<std::pair<int, int>> affected_coords;
        std::fill(&visited_cells[0][0], &visited_cells[0][0] + N*N, false);
        
        if (p_old.d == 0) {
            for (size_t k = 0; k < s.length(); ++k) { int r = p_old.r, c = (p_old.c + k) % N; if (!visited_cells[r][c]) { affected_coords.push_back({r, c}); visited_cells[r][c] = true; } }
        } else {
            for (size_t k = 0; k < s.length(); ++k) { int r = (p_old.r + k) % N, c = p_old.c; if (!visited_cells[r][c]) { affected_coords.push_back({r, c}); visited_cells[r][c] = true; } }
        }
        if (p_new.d == 0) {
            for (size_t k = 0; k < s.length(); ++k) { int r = p_new.r, c = (p_new.c + k) % N; if (!visited_cells[r][c]) { affected_coords.push_back({r, c}); visited_cells[r][c] = true; } }
        } else {
            for (size_t k = 0; k < s.length(); ++k) { int r = (p_new.r + k) % N, c = p_new.c; if (!visited_cells[r][c]) { affected_coords.push_back({r, c}); visited_cells[r][c] = true; } }
        }

        long long delta_E = 0;
        for (auto const& [r, c] : affected_coords) delta_E -= calculate_cell_energy(r, c);

        if (p_old.d == 0) { for (size_t k = 0; k < s.length(); ++k) counts[p_old.r][(p_old.c + k) % N][char_to_int(s[k])]--; }
        else { for (size_t k = 0; k < s.length(); ++k) counts[(p_old.r + k) % N][p_old.c][char_to_int(s[k])]--; }
        if (p_new.d == 0) { for (size_t k = 0; k < s.length(); ++k) counts[p_new.r][(p_new.c + k) % N][char_to_int(s[k])]++; }
        else { for (size_t k = 0; k < s.length(); ++k) counts[(p_new.r + k) % N][p_new.c][char_to_int(s[k])]++; }

        for (auto const& [r, c] : affected_coords) delta_E += calculate_cell_energy(r, c);

        if (delta_E <= 0 || rand_double(0.0, 1.0) < std::exp(-delta_E / T)) {
            placements[s_idx] = p_new;
            current_energy += delta_E;
            if (current_energy < best_energy) {
                best_energy = current_energy;
                best_placements = placements;
            }
        } else { // Revert
            if (p_new.d == 0) { for (size_t k = 0; k < s.length(); ++k) counts[p_new.r][(p_new.c + k) % N][char_to_int(s[k])]--; }
            else { for (size_t k = 0; k < s.length(); ++k) counts[(p_new.r + k) % N][p_new.c][char_to_int(s[k])]--; }
            if (p_old.d == 0) { for (size_t k = 0; k < s.length(); ++k) counts[p_old.r][(p_old.c + k) % N][char_to_int(s[k])]++; }
            else { for (size_t k = 0; k < s.length(); ++k) counts[(p_old.r + k) % N][p_old.c][char_to_int(s[k])]++; }
        }
    }
}

void output_solution() {
    std::fill(&counts[0][0][0], &counts[0][0][0] + N * N * 8, 0);
    for (int i = 0; i < M; ++i) {
        const auto& s = S[i];
        const auto& p = best_placements[i];
        if (p.d == 0) {
            for (size_t k = 0; k < s.length(); ++k) counts[p.r][(p.c + k) % N][char_to_int(s[k])]++;
        } else {
            for (size_t k = 0; k < s.length(); ++k) counts[(p.r + k) % N][p.c][char_to_int(s[k])]++;
        }
    }

    std::vector<std::string> grid(N, std::string(N, '.'));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int max_freq = 0;
            int best_char_idx = -1;
            for (int k = 0; k < 8; ++k) {
                if (counts[i][j][k] > max_freq) {
                    max_freq = counts[i][j][k];
                    best_char_idx = k;
                }
            }
            if (best_char_idx != -1) grid[i][j] = int_to_char(best_char_idx);
        }
    }

    for (int i = 0; i < N; ++i) {
        std::cout << grid[i] << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_dummy;
    std::cin >> n_dummy >> M;
    S.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> S[i];
    }
    
    solve();
    output_solution();

    return 0;
}