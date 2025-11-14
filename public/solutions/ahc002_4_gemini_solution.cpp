#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <tuple>

// Constants
const int N = 50;
const int DR[] = {-1, 1, 0, 0};
const int DC[] = {0, 0, -1, 1};
const char MOVE_CHAR[] = {'U', 'D', 'L', 'R'};

// Grid and tile information
int si, sj;
int t[N][N];
int p[N][N];
int M; // number of tiles
std::vector<std::vector<std::pair<int, int>>> tile_squares;
std::vector<long long> tile_value;
std::vector<int> tile_size;

// Random number generator
std::mt19937 rng;

// Timer
std::chrono::high_resolution_clock::time_point start_time;

// Best solution found
long long best_score = -1;
std::string best_path;

struct Candidate {
    long long value;
    int r, c;
    char move;
    int tile_id;

    bool operator>(const Candidate& other) const {
        return value > other.value;
    }
};

char get_move_char(int r1, int c1, int r2, int c2) {
    if (r2 < r1) return 'U';
    if (r2 > r1) return 'D';
    if (c2 < c1) return 'L';
    return 'R';
}

std::pair<int, int> get_other_square(int tile_id, int r, int c) {
    if (tile_squares[tile_id][0].first == r && tile_squares[tile_id][0].second == c) {
        return tile_squares[tile_id][1];
    }
    return tile_squares[tile_id][0];
}

void run_simulation() {
    std::string current_path = "";
    long long current_score = 0;
    std::vector<bool> visited_tiles(M, false);
    int r = si;
    int c = sj;

    int start_tile_id = t[r][c];
    visited_tiles[start_tile_id] = true;
    current_score += p[r][c];

    if (tile_size[start_tile_id] == 2) {
        auto other = get_other_square(start_tile_id, r, c);
        current_path += get_move_char(r, c, other.first, other.second);
        r = other.first;
        c = other.second;
        current_score += p[r][c];
    }

    while (true) {
        std::vector<Candidate> candidates;
        for (int i = 0; i < 4; ++i) {
            int nr = r + DR[i];
            int nc = c + DC[i];

            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                int next_tile_id = t[nr][nc];
                if (!visited_tiles[next_tile_id]) {
                    candidates.push_back({tile_value[next_tile_id], nr, nc, MOVE_CHAR[i], next_tile_id});
                }
            }
        }

        if (candidates.empty()) {
            break;
        }

        std::sort(candidates.begin(), candidates.end(), std::greater<Candidate>());
        
        int k = std::min((int)candidates.size(), 3);
        std::uniform_int_distribution<int> dist(0, k - 1);
        int choice_idx = dist(rng);
        const auto& chosen = candidates[choice_idx];
        
        current_path += chosen.move;
        r = chosen.r;
        c = chosen.c;
        int next_tile_id = chosen.tile_id;

        visited_tiles[next_tile_id] = true;
        current_score += p[r][c];

        if (tile_size[next_tile_id] == 2) {
            auto other = get_other_square(next_tile_id, r, c);
            current_path += get_move_char(r, c, other.first, other.second);
            r = other.first;
            c = other.second;
            current_score += p[r][c];
        }
    }

    if (current_score > best_score) {
        best_score = current_score;
        best_path = current_path;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    start_time = std::chrono::high_resolution_clock::now();
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    std::cin >> si >> sj;
    int max_t_id = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> t[i][j];
            max_t_id = std::max(max_t_id, t[i][j]);
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> p[i][j];
        }
    }
    M = max_t_id + 1;

    tile_squares.resize(M);
    tile_value.assign(M, 0);
    tile_size.assign(M, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int id = t[i][j];
            tile_squares[id].push_back({i, j});
            tile_value[id] += p[i][j];
        }
    }
    for (int i = 0; i < M; ++i) {
        tile_size[i] = tile_squares[i].size();
    }

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() > 2800) {
            break;
        }
        run_simulation();
    }

    std::cout << best_path << std::endl;

    return 0;
}