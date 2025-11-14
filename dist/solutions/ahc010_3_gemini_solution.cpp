#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <array>
#include <tuple>

using namespace std;

constexpr int N = 30;
constexpr double TIME_LIMIT = 1.95;

// Directions: 0:L, 1:U, 2:R, 3:D
const int di[] = {0, -1, 0, 1};
const int dj[] = {-1, 0, 1, 0};

// to[tile_type][entry_port] -> exit_port
const int to[8][4] = {
    {1, 0, -1, -1}, // 0: U-L
    {3, -1, -1, 0}, // 1: U-R
    {-1, -1, 3, 2}, // 2: D-R
    {-1, 2, 1, -1}, // 3: D-L
    {1, 0, 3, 2},   // 4: U-L, D-R
    {3, 2, 1, 0},   // 5: U-R, D-L
    {2, -1, 0, -1}, // 6: L-R
    {-1, 3, -1, 1}  // 7: U-D
};

int initial_tiles[N][N];
int rotated_types[8][4];

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
        x = y;
        y = z;
        z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    int next_int(int n) {
        return next() % n;
    }

    double next_double() {
        return (double)next() / 0xFFFFFFFF;
    }
};

XorShift rnd;

void precompute_rotations() {
    for (int t = 0; t < 8; ++t) {
        int current_t = t;
        for (int r = 0; r < 4; ++r) {
            rotated_types[t][r] = current_t;
            if (current_t <= 3) {
                current_t = (current_t + 1) % 4;
            } else if (current_t <= 5) {
                current_t = 4 + (current_t - 4 + 1) % 2;
            } else {
                current_t = 6 + (current_t - 6 + 1) % 2;
            }
        }
    }
}

long long calculate_score(const array<array<int, N>, N>& rot) {
    array<array<int, N>, N> current_tiles;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_tiles[i][j] = rotated_types[initial_tiles[i][j]][rot[i][j]];
        }
    }

    bool visited_edge[N][N][4] = {false};
    vector<int> loops;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int d_start = 0; d_start < 4; ++d_start) {
                int tile_type = current_tiles[i][j];
                int d_exit = to[tile_type][d_start];
                if (d_exit == -1) continue;

                int ni = i + di[d_exit];
                int nj = j + dj[d_exit];

                int norm_i = i, norm_j = j, norm_d = d_exit;
                if (norm_i > ni || (norm_i == ni && norm_j > nj)) {
                    norm_i = ni;
                    norm_j = nj;
                    norm_d = (d_exit + 2) % 4;
                }
                if (visited_edge[norm_i][norm_j][norm_d]) continue;

                int cur_i = i, cur_j = j;
                int cur_d_entry = d_start;
                int length = 0;
                
                while(true) {
                    int tile_type = current_tiles[cur_i][cur_j];
                    int d_exit_cur = to[tile_type][cur_d_entry];
                    
                    int next_i = cur_i + di[d_exit_cur];
                    int next_j = cur_j + dj[d_exit_cur];
                    
                    length++;
                    cur_i = next_i;
                    cur_j = next_j;
                    cur_d_entry = (d_exit_cur + 2) % 4;

                    if (cur_i == i && cur_j == j && cur_d_entry == d_start) {
                        loops.push_back(length);
                        int temp_i = i, temp_j = j, temp_d = d_start;
                        for(int k=0; k<length; ++k) {
                            int type = current_tiles[temp_i][temp_j];
                            int dexit = to[type][temp_d];
                            int n_i = temp_i + di[dexit];
                            int n_j = temp_j + dj[dexit];
                            int ni_ = temp_i, nj_ = temp_j, nd_ = dexit;
                            if(ni_ > n_i || (ni_ == n_i && nj_ > n_j)) {
                                ni_ = n_i; nj_ = n_j; nd_ = (dexit+2)%4;
                            }
                            visited_edge[ni_][nj_][nd_] = true;

                            temp_i = n_i; temp_j = n_j; temp_d = (dexit+2)%4;
                        }
                        break;
                    }
                    if (length > N*N*2) break; // Should not happen in a loop
                }
            }
        }
    }

    if (loops.size() < 2) {
        return 0;
    }

    sort(loops.rbegin(), loops.rend());
    return (long long)loops[0] * loops[1];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_tiles[i][j] = row[j] - '0';
        }
    }

    precompute_rotations();

    array<array<int, N>, N> current_rot;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            current_rot[i][j] = rnd.next_int(4);
        }
    }
    
    array<array<int, N>, N> best_rot = current_rot;
    long long current_score = calculate_score(current_rot);
    long long best_score = current_score;

    double T_start = 2000.0, T_end = 0.1;

    while (true) {
        auto current_time = chrono::high_resolution_clock::now();
        double elapsed_sec = chrono::duration_cast<chrono::duration<double>>(current_time - start_time).count();
        if (elapsed_sec > TIME_LIMIT) {
            break;
        }

        double T = T_start + (T_end - T_start) * elapsed_sec / TIME_LIMIT;

        int i = rnd.next_int(N);
        int j = rnd.next_int(N);
        int r_old = current_rot[i][j];
        int r_new = (r_old + rnd.next_int(3) + 1) % 4;
        
        current_rot[i][j] = r_new;
        long long new_score = calculate_score(current_rot);

        double delta = (double)new_score - current_score;

        if (delta >= 0 || exp(delta / T) > rnd.next_double()) {
            current_score = new_score;
            if (current_score > best_score) {
                best_score = current_score;
                best_rot = current_rot;
            }
        } else {
            current_rot[i][j] = r_old;
        }
    }

    string ans = "";
    ans.reserve(N * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            ans += to_string(best_rot[i][j]);
        }
    }
    cout << ans << endl;

    return 0;
}