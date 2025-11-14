#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <map>
#include <tuple>

// Constants
const int N = 50;
const int MAX_PATH_LENGTH = 5000;
const double TIME_LIMIT = 1.95;

// Global data
int si, sj;
int t[N][N];
int p[N][N];
int num_tiles = 0;

std::vector<std::vector<std::pair<int, int>>> tile_squares;
std::vector<long long> tile_values;
std::vector<int> tile_sizes;

// Directions
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_char[] = {'U', 'D', 'L', 'R'};

// RNG
std::mt19937 rng;

void read_and_precompute() {
    std::cin >> si >> sj;
    std::vector<std::vector<int>> raw_t(N, std::vector<int>(N));
    std::map<int, int> raw_to_compact;
    int next_id = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int raw_tile_id;
            std::cin >> raw_tile_id;
            if (raw_to_compact.find(raw_tile_id) == raw_to_compact.end()) {
                raw_to_compact[raw_tile_id] = next_id++;
            }
            t[i][j] = raw_to_compact[raw_tile_id];
        }
    }
    num_tiles = next_id;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> p[i][j];
        }
    }
    
    tile_squares.resize(num_tiles);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            tile_squares[t[i][j]].push_back({i, j});
        }
    }
    
    tile_values.resize(num_tiles);
    tile_sizes.resize(num_tiles);
    for (int i = 0; i < num_tiles; ++i) {
        long long current_tile_value = 0;
        for (auto const& sq : tile_squares[i]) {
            current_tile_value += p[sq.first][sq.second];
        }
        tile_values[i] = current_tile_value;
        tile_sizes[i] = tile_squares[i].size();
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::random_device rd;
    rng.seed(rd());

    read_and_precompute();

    std::string best_path = "";
    long long max_score = -1;
    
    const int BEAM_WIDTH = 2;

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        if (elapsed.count() > TIME_LIMIT) {
            break;
        }

        std::string current_path = "";
        long long current_score = 0;
        int cr = si, cc = sj;

        bool visited_squares[N][N] = {};
        std::vector<bool> visited_tiles(num_tiles, false);

        visited_squares[cr][cc] = true;
        visited_tiles[t[cr][cc]] = true;
        current_score += p[cr][cc];

        for (int move_count = 0; move_count < MAX_PATH_LENGTH; ++move_count) {
            std::vector<std::pair<double, std::tuple<int, int, std::string>>> candidate_moves;
            int current_tile_id = t[cr][cc];

            // Option 1: Move within the current tile
            if (tile_sizes[current_tile_id] == 2) {
                const auto& squares = tile_squares[current_tile_id];
                int or_, oc_;
                if (squares[0].first == cr && squares[0].second == cc) {
                    or_ = squares[1].first; oc_ = squares[1].second;
                } else {
                    or_ = squares[0].first; oc_ = squares[0].second;
                }

                if (!visited_squares[or_][oc_]) {
                    for (int i = 0; i < 4; ++i) {
                        if (cr + dr[i] == or_ && cc + dc[i] == oc_) {
                            double eval_score = p[or_][oc_];
                            candidate_moves.push_back({eval_score, {or_, oc_, std::string(1, move_char[i])}});
                            break;
                        }
                    }
                }
            }

            // Option 2: Move to a new tile
            for (int i = 0; i < 4; ++i) {
                int nr = cr + dr[i];
                int nc = cc + dc[i];
                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;

                int next_tile_id = t[nr][nc];
                if (next_tile_id != current_tile_id && !visited_tiles[next_tile_id]) {
                    double eval_score = tile_values[next_tile_id];
                    std::string path_segment(1, move_char[i]);
                    int final_r = nr, final_c = nc;
                    
                    if (tile_sizes[next_tile_id] == 2) {
                        const auto& squares = tile_squares[next_tile_id];
                        int or_, oc_;
                        if (squares[0].first == nr && squares[0].second == nc) {
                            or_ = squares[1].first; oc_ = squares[1].second;
                        } else {
                            or_ = squares[0].first; oc_ = squares[0].second;
                        }
                        for (int j = 0; j < 4; ++j) {
                            if (nr + dr[j] == or_ && nc + dc[j] == oc_) {
                                path_segment += move_char[j];
                                break;
                            }
                        }
                        final_r = or_; final_c = oc_;
                    }
                    candidate_moves.push_back({eval_score, {final_r, final_c, path_segment}});
                }
            }

            if (candidate_moves.empty()) {
                break;
            }

            std::sort(candidate_moves.rbegin(), candidate_moves.rend());
            
            int choice_range = std::min((int)candidate_moves.size(), BEAM_WIDTH);
            std::uniform_int_distribution<int> dist(0, choice_range - 1);
            auto const& chosen_move = candidate_moves[dist(rng)].second;

            std::string path_segment = std::get<2>(chosen_move);
            current_path += path_segment;
            
            int temp_r = cr, temp_c = cc;
            for (char move_c : path_segment) {
                if (move_c == 'U') temp_r--; else if (move_c == 'D') temp_r++; else if (move_c == 'L') temp_c--; else temp_c++;
                
                if (!visited_squares[temp_r][temp_c]) {
                    visited_squares[temp_r][temp_c] = true;
                    current_score += p[temp_r][temp_c];
                }
                visited_tiles[t[temp_r][temp_c]] = true;
            }
            cr = std::get<0>(chosen_move);
            cc = std::get<1>(chosen_move);
        }

        if (current_score > max_score) {
            max_score = current_score;
            best_path = current_path;
        }
    }

    std::cout << best_path << std::endl;

    return 0;
}