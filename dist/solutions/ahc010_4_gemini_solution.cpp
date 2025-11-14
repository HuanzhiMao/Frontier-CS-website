#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <tuple>

// Constants
constexpr int N = 30;
constexpr int TIME_LIMIT_MS = 1950;

// Directions: 0: left, 1: up, 2: right, 3: down
const int DI[] = {0, -1, 0, 1};
const int DJ[] = {-1, 0, 1, 0};

// Connection logic for each tile type
// to[t][d]: given tile type t and entering from direction d, which direction to exit
const int TO[8][4] = {
    {1, 0, -1, -1}, {3, -1, -1, 0}, {-1, -1, 3, 2}, {-1, 2, 1, -1},
    {1, 0, 3, 2},   {3, 2, 1, 0},   {2, -1, 0, -1}, {-1, 3, -1, 1}
};

// State variables, global for performance
std::array<std::array<int, N>, N> initial_tiles;
std::array<std::array<int, N>, N> current_rotations;
std::array<std::array<std::array<bool, 4>, N>, N> visited;

struct Result {
    long long score;
};

// Fast random number generator
struct XorShift {
    unsigned int x, y, z, w;
    XorShift() : x(123456789), y(362436069), z(521288629), w(88675123) {}
    unsigned int next() {
        unsigned int t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    int next_int(int n) { return next() % n; }
    double next_double() { return (double)next() / std::numeric_limits<unsigned int>::max(); }
};

int get_rotated_type(int initial_type, int r) {
    if (initial_type <= 3) {
        return (initial_type + r) % 4;
    } else if (initial_type <= 5) {
        return 4 + ((initial_type - 4 + r) % 2);
    } else {
        return 6 + ((initial_type - 6 + r) % 2);
    }
}

Result calculate_score() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::fill(visited[i][j].begin(), visited[i][j].end(), false);
        }
    }

    std::vector<int> loops;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int d_start = 0; d_start < 4; ++d_start) {
                if (visited[i][j][d_start]) continue;

                std::vector<std::tuple<int, int, int>> path;
                int ci = i, cj = j, cd = d_start;
                int length = 0;
                bool is_loop = true;

                while (true) {
                    if (visited[ci][cj][cd]) {
                        is_loop = false;
                        break;
                    }
                    path.emplace_back(ci, cj, cd);
                    
                    int type = get_rotated_type(initial_tiles[ci][cj], current_rotations[ci][cj]);
                    int d_out = TO[type][cd];
                    
                    if (d_out == -1) { is_loop = false; break; }
                    
                    path.emplace_back(ci, cj, d_out);
                    
                    int ni = ci + DI[d_out];
                    int nj = cj + DJ[d_out];
                    length++;
                    
                    if (ni < 0 || ni >= N || nj < 0 || nj >= N) {
                        is_loop = false;
                        break;
                    }
                    
                    int nd = (d_out + 2) % 4;
                    
                    if (ni == i && nj == j && nd == d_start) break;
                    
                    ci = ni; cj = nj; cd = nd;
                }
                
                for (const auto& t : path) {
                    visited[std::get<0>(t)][std::get<1>(t)][std::get<2>(t)] = true;
                }

                if (is_loop) {
                    loops.push_back(length);
                }
            }
        }
    }
    
    std::sort(loops.rbegin(), loops.rend());
    
    if (loops.size() < 2) return {0};
    return {(long long)loops[0] * loops[1]};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        std::string row;
        std::cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_tiles[i][j] = row[j] - '0';
        }
    }

    XorShift rng;
    
    std::array<std::array<int, N>, N> best_rotations;
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_rotations[i][j] = rng.next_int(4);
        }
    }

    Result current_res = calculate_score();
    long long best_score = current_res.score;
    best_rotations = current_rotations;

    double start_temp = 5000;
    double end_temp = 1.0;
    
    while(true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        long long elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed_ms >= TIME_LIMIT_MS) break;

        int r = rng.next_int(N);
        int c = rng.next_int(N);
        int old_rot = current_rotations[r][c];
        int new_rot = (old_rot + rng.next_int(3) + 1) % 4;

        current_rotations[r][c] = new_rot;
        Result new_res = calculate_score();

        double temp = start_temp * pow(end_temp / start_temp, (double)elapsed_ms / TIME_LIMIT_MS);
        double prob = exp((double)(new_res.score - current_res.score) / temp);

        if (prob > rng.next_double()) {
            current_res = new_res;
            if (current_res.score > best_score) {
                best_score = current_res.score;
                best_rotations = current_rotations;
            }
        } else {
            current_rotations[r][c] = old_rot;
        }
    }
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << best_rotations[i][j];
        }
    }
    std::cout << std::endl;

    return 0;
}