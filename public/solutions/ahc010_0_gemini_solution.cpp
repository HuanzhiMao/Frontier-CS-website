#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <tuple>
#include <array>

const int N = 30;
const int TIME_LIMIT_MS = 1950;

// Directions: 0:L, 1:U, 2:R, 3:D
const int di[] = {0, -1, 0, 1};
const int dj[] = {-1, 0, 1, 0};

// to[tile_type][entry_direction] -> exit_direction
const int to[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1}
};

// rotate_table[tile_type] -> new_tile_type after 90 deg ccw rotation
const int rotate_table[8] = {1, 2, 3, 0, 5, 4, 7, 6};

int initial_tiles[N][N];
int rotations[N][N];
int current_tiles[N][N];
int best_rotations[N][N];

std::mt19937 rng;

int rotated_tiles[8][4];

void precompute_rotations() {
    for (int t = 0; t < 8; ++t) {
        rotated_tiles[t][0] = t;
        for (int r = 1; r < 4; ++r) {
            rotated_tiles[t][r] = rotate_table[rotated_tiles[t][r - 1]];
        }
    }
}

void update_all_current_tiles() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_tiles[i][j] = rotated_tiles[initial_tiles[i][j]][rotations[i][j]];
        }
    }
}

long long calculate_score() {
    bool visited[N][N][4] = {};
    std::vector<int> loops;

    for (int i_start = 0; i_start < N; ++i_start) {
        for (int j_start = 0; j_start < N; ++j_start) {
            for (int d_start = 0; d_start < 4; ++d_start) {
                if (visited[i_start][j_start][d_start]) continue;

                int i = i_start, j = j_start, d = d_start;
                int length = 0;
                
                while (true) {
                    visited[i][j][d] = true;
                    int tile_type = current_tiles[i][j];
                    int d_exit = to[tile_type][d];
                    
                    if (d_exit == -1) { length = 0; break; }
                    
                    visited[i][j][d_exit] = true;

                    int ni = i + di[d_exit];
                    int nj = j + dj[d_exit];
                    
                    if (ni < 0 || ni >= N || nj < 0 || nj >= N) { length = 0; break; }
                    
                    int nd = (d_exit + 2) % 4;
                    i = ni; j = nj; d = nd;
                    length++;
                    
                    if (i == i_start && j == j_start && d == d_start) break;
                    
                    if (visited[i][j][d]) {
                        length = 0;
                        break;
                    }
                }

                if (length > 0) loops.push_back(length);
            }
        }
    }

    if (loops.size() < 2) return 0;
    std::sort(loops.rbegin(), loops.rend());
    return (long long)loops[0] * loops[1];
}

void solve() {
    auto start_time = std::chrono::high_resolution_clock::now();
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    
    std::uniform_int_distribution<> dist_coord(0, N - 1);
    std::uniform_int_distribution<> dist_rot(0, 3);
    std::uniform_int_distribution<> dist_rot_change(1, 3);
    std::uniform_real_distribution<> dist_prob(0.0, 1.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            rotations[i][j] = dist_rot(rng);
        }
    }

    update_all_current_tiles();
    long long current_score = calculate_score();
    long long best_score = current_score;
    std::copy(&rotations[0][0], &rotations[0][0] + N * N, &best_rotations[0][0]);

    double T_start = 50000;
    double T_end = 1;
    
    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() > TIME_LIMIT_MS) {
            break;
        }
        
        double progress = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() / (double)TIME_LIMIT_MS;
        double T = T_start * pow(T_end / T_start, progress);

        int r = dist_coord(rng);
        int c = dist_coord(rng);
        
        int old_rot = rotations[r][c];
        int new_rot = (old_rot + dist_rot_change(rng)) % 4;
        
        rotations[r][c] = new_rot;
        int old_tile_type = current_tiles[r][c];
        current_tiles[r][c] = rotated_tiles[initial_tiles[r][c]][new_rot];

        long long new_score = calculate_score();
        
        double delta = (double)new_score - current_score;

        if (delta > 0 || exp(delta / T) > dist_prob(rng)) {
            current_score = new_score;
            if (current_score > best_score) {
                best_score = current_score;
                std::copy(&rotations[0][0], &rotations[0][0] + N * N, &best_rotations[0][0]);
            }
        } else {
            rotations[r][c] = old_rot;
            current_tiles[r][c] = old_tile_type;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << best_rotations[i][j];
        }
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    precompute_rotations();

    for (int i = 0; i < N; ++i) {
        std::string row;
        std::cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_tiles[i][j] = row[j] - '0';
        }
    }

    solve();

    return 0;
}