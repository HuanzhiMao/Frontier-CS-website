#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <tuple>

using namespace std;

const int N = 30;
const double TIME_LIMIT = 1.95; // seconds

// di/dj for movement directions: 0:L, 1:U, 2:R, 3:D
const int di[] = {0, -1, 0, 1};
const int dj[] = {-1, 0, 1, 0};

// to[tile_type][entry_dir] -> exit_dir
// entry_dir: 0:from L, 1:from U, 2:from R, 3:from D
const int to[8][4] = {
    {1, 0, -1, -1}, {3, -1, -1, 0}, {-1, -1, 3, 2}, {-1, 2, 1, -1},
    {1, 0, 3, 2},   {3, 2, 1, 0},   {2, -1, 0, -1}, {-1, 3, -1, 1},
};

int initial_tiles[N][N];
mt19937 engine;

// Calculates the tile type after r rotations of 90 degrees CCW
int rotate_tile(int type, int r) {
    if (0 <= type && type <= 3) {
        return (type + r) % 4;
    }
    if (4 <= type && type <= 5) {
        return 4 + ((type - 4 + r) % 2);
    }
    if (6 <= type && type <= 7) {
        return 6 + ((type - 6 + r) % 2);
    }
    return -1; // Should not happen
}

// Calculates the score for a given configuration of rotations
long long calculate_score(const vector<vector<int>>& rotations) {
    int current_tiles[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_tiles[i][j] = rotate_tile(initial_tiles[i][j], rotations[i][j]);
        }
    }

    bool visited[N][N][4] = {};
    vector<int> loops;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int d_start = 0; d_start < 4; ++d_start) {
                if (visited[i][j][d_start]) continue;

                vector<tuple<int, int, int>> path;
                int r = i, c = j, d = d_start;
                int length = 0;
                bool is_loop = true;

                while (true) {
                    if (visited[r][c][d]) {
                        is_loop = false;
                        break;
                    }
                    path.emplace_back(r, c, d);

                    int cur_tile_type = current_tiles[r][c];
                    int d2 = to[cur_tile_type][d];
                    
                    if (d2 == -1) {
                        is_loop = false;
                        break;
                    }

                    r += di[d2];
                    c += dj[d2];
                    length++;
                    
                    if (r < 0 || r >= N || c < 0 || c >= N) {
                        is_loop = false;
                        break;
                    }

                    d = (d2 + 2) % 4;

                    if (r == i && c == j && d == d_start) {
                        break;
                    }
                }
                
                for(const auto& t : path) {
                    visited[get<0>(t)][get<1>(t)][get<2>(t)] = true;
                }

                if (is_loop && length > 0) {
                    loops.push_back(length);
                }
            }
        }
    }

    sort(loops.rbegin(), loops.rend());
    if (loops.size() < 2) return 0;
    return (long long)loops[0] * loops[1];
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    random_device rd;
    engine.seed(rd());

    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_tiles[i][j] = row[j] - '0';
        }
    }

    auto start_time = chrono::high_resolution_clock::now();

    vector<vector<int>> current_rotations(N, vector<int>(N, 0));
    uniform_int_distribution<int> rot_dist(0, 3);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_rotations[i][j] = rot_dist(engine);
        }
    }

    vector<vector<int>> best_rotations = current_rotations;
    long long current_score = calculate_score(current_rotations);
    long long best_score = current_score;

    double T0 = 10000, T1 = 1;
    uniform_int_distribution<int> dist_N(0, N - 1);
    uniform_int_distribution<int> dist_rot_change(1, 3);
    uniform_real_distribution<double> dist_real(0.0, 1.0);

    while (true) {
        auto current_time = chrono::high_resolution_clock::now();
        double time_elapsed = chrono::duration<double>(current_time - start_time).count();
        if (time_elapsed > TIME_LIMIT) {
            break;
        }

        int r = dist_N(engine);
        int c = dist_N(engine);
        int old_rot = current_rotations[r][c];
        int rot_change = dist_rot_change(engine);
        current_rotations[r][c] = (old_rot + rot_change) % 4;
        
        long long new_score = calculate_score(current_rotations);

        double temp = T0 * pow(T1 / T0, time_elapsed / TIME_LIMIT);
        double prob = exp((double)(new_score - current_score) / temp);

        if (prob > dist_real(engine)) {
            current_score = new_score;
            if (current_score > best_score) {
                best_score = current_score;
                best_rotations = current_rotations;
            }
        } else {
            current_rotations[r][c] = old_rot;
        }
    }

    string output = "";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            output += to_string(best_rotations[i][j]);
        }
    }
    cout << output << endl;
    
    return 0;
}