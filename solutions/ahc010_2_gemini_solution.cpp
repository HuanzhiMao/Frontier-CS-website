#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <tuple>
#include <climits>

#pragma GCC optimize("O3")

using namespace std;

const int N = 30;
const double TIME_LIMIT = 2.95; 

struct XorShift {
    unsigned int x, y, z, w;

    XorShift() {
        x = 123456789;
        y = 362436069;
        z = 521288629;
        w = 88675123;
    }

    unsigned int next() {
        unsigned int t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    int randint(int a, int b) {
        return a + next() % (b - a + 1);
    }
    
    double rand_double(double a, double b) {
        return a + (double)next() / UINT_MAX * (b - a);
    }
};

XorShift rng;

// Problem-specific data
vector<vector<int>> initial_tiles(N, vector<int>(N));

const int di[] = {0, -1, 0, 1}; // L, U, R, D
const int dj[] = {-1, 0, 1, 0};

const int to[8][4] = {
    {1, 0, -1, -1}, {3, -1, -1, 0}, {-1, -1, 3, 2}, {-1, 2, 1, -1},
    {1, 0, 3, 2}, {3, 2, 1, 0}, {2, -1, 0, -1}, {-1, 3, -1, 1},
};

const int rotate90[] = {1, 2, 3, 0, 5, 4, 7, 6};

int get_tile_type(int i, int j, int rot) {
    int type = initial_tiles[i][j];
    for (int k = 0; k < rot; ++k) {
        type = rotate90[type];
    }
    return type;
}

long long calculate_score(const vector<vector<int>>& current_tiles, vector<int>& loops) {
    loops.clear();
    bool visited[N][N][4] = {};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int d_start = 0; d_start < 4; ++d_start) {
                if (visited[i][j][d_start]) continue;
                
                int type = current_tiles[i][j];
                if (to[type][d_start] == -1) continue;

                int cur_i = i, cur_j = j, cur_d = d_start;
                int len = 0;
                bool is_loop = false;

                while (true) {
                    if (visited[cur_i][cur_j][cur_d]) {
                        is_loop = false;
                        break;
                    }
                    
                    visited[cur_i][cur_j][cur_d] = true;
                    int d_out = to[current_tiles[cur_i][cur_j]][cur_d];
                    visited[cur_i][cur_j][d_out] = true;
                    
                    cur_i += di[d_out];
                    cur_j += dj[d_out];
                    len++;

                    if (cur_i < 0 || cur_i >= N || cur_j < 0 || cur_j >= N) {
                        is_loop = false;
                        break;
                    }
                    
                    cur_d = (d_out + 2) % 4;

                    if (cur_i == i && cur_j == j && cur_d == d_start) {
                        is_loop = true;
                        break;
                    }
                }

                if (is_loop) {
                    loops.push_back(len);
                }
            }
        }
    }

    if (loops.size() < 2) return 0;
    sort(loops.rbegin(), loops.rend());
    return (long long)loops[0] * loops[1];
}


void solve() {
    auto start_time = chrono::steady_clock::now();

    vector<vector<int>> current_rotations(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_rotations[i][j] = rng.randint(0, 3);
        }
    }
    
    vector<vector<int>> current_tiles(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_tiles[i][j] = get_tile_type(i, j, current_rotations[i][j]);
        }
    }

    vector<int> loops;
    long long current_score = calculate_score(current_tiles, loops);
    vector<vector<int>> best_rotations = current_rotations;
    long long best_score = current_score;

    double T_start = 50000;
    double T_end = 1.0;
    int iter = 0;

    while (true) {
        iter++;
        auto now = chrono::steady_clock::now();
        double elapsed_time = chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
        if (elapsed_time > TIME_LIMIT) {
            break;
        }

        int r = rng.randint(0, N - 1);
        int c = rng.randint(0, N - 1);
        int old_rot = current_rotations[r][c];
        int new_rot = (old_rot + rng.randint(1, 3)) % 4;

        int old_tile_type = current_tiles[r][c];
        current_rotations[r][c] = new_rot;
        current_tiles[r][c] = get_tile_type(r, c, new_rot);
        
        vector<int> new_loops;
        long long new_score = calculate_score(current_tiles, new_loops);

        double temp = T_start * pow(T_end / T_start, elapsed_time / TIME_LIMIT);
        
        if (new_score > current_score || rng.rand_double(0.0, 1.0) < exp((double)(new_score - current_score) / temp)) {
            current_score = new_score;
            if (new_score > best_score) {
                best_score = new_score;
                best_rotations = current_rotations;
            }
        } else {
            current_rotations[r][c] = old_rot;
            current_tiles[r][c] = old_tile_type;
        }
    }

    string output_str = "";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            output_str += to_string(best_rotations[i][j]);
        }
    }
    cout << output_str << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_tiles[i][j] = row[j] - '0';
        }
    }

    solve();

    return 0;
}