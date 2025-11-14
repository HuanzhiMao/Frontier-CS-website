#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

const int SIZE = 10;
using Grid = vector<vector<int>>;

Grid grid(SIZE, vector<int>(SIZE, 0));
vector<int> flavors(100);
vector<int> t_start(4), t_end(4);

Grid tilt(const Grid& g, char dir) {
    Grid next_g(SIZE, vector<int>(SIZE, 0));
    if (dir == 'F') {
        for (int j = 0; j < SIZE; ++j) {
            int current_r = 0;
            for (int i = 0; i < SIZE; ++i) {
                if (g[i][j] != 0) {
                    next_g[current_r++][j] = g[i][j];
                }
            }
        }
    } else if (dir == 'B') {
        for (int j = 0; j < SIZE; ++j) {
            int current_r = SIZE - 1;
            for (int i = SIZE - 1; i >= 0; --i) {
                if (g[i][j] != 0) {
                    next_g[current_r--][j] = g[i][j];
                }
            }
        }
    } else if (dir == 'L') {
        for (int i = 0; i < SIZE; ++i) {
            int current_c = 0;
            for (int j = 0; j < SIZE; ++j) {
                if (g[i][j] != 0) {
                    next_g[i][current_c++] = g[i][j];
                }
            }
        }
    } else if (dir == 'R') {
        for (int i = 0; i < SIZE; ++i) {
            int current_c = SIZE - 1;
            for (int j = SIZE - 1; j >= 0; --j) {
                if (g[i][j] != 0) {
                    next_g[i][current_c--] = g[i][j];
                }
            }
        }
    }
    return next_g;
}

int adjacency_score(const Grid& g) {
    int score = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (g[i][j] != 0) {
                if (i + 1 < SIZE && g[i + 1][j] == g[i][j]) score++;
                if (j + 1 < SIZE && g[i][j + 1] == g[i][j]) score++;
            }
        }
    }
    return score;
}

int distance_score(const Grid& g) {
    int score = 0;
    const int penalty = 20;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (g[i][j] != 0) {
                int flavor = g[i][j];
                int h_dist = max(0, j - t_end[flavor]) + max(0, t_start[flavor] - j);
                if (h_dist == 0) {
                    score += i;
                } else {
                    score += h_dist * penalty;
                }
            }
        }
    }
    return score;
}

void precompute_target_regions() {
    vector<int> d(4, 0);
    for (int f : flavors) {
        d[f]++;
    }

    vector<pair<int, int>> flavor_counts;
    for (int i = 1; i <= 3; ++i) {
        flavor_counts.push_back({d[i], i});
    }
    sort(flavor_counts.rbegin(), flavor_counts.rend());

    int f_large = flavor_counts[0].second;
    int f_mid = flavor_counts[1].second;
    int f_small = flavor_counts[2].second;

    vector<int> final_widths(4, 0);
    final_widths[f_large] = d[f_large] / 10;
    final_widths[f_mid] = d[f_mid] / 10;
    final_widths[f_small] = d[f_small] / 10;

    int rem_cols = 10 - (final_widths[f_large] + final_widths[f_mid] + final_widths[f_small]);

    vector<pair<int, int>> rem_contrib;
    rem_contrib.push_back({d[f_large] % 10, f_large});
    rem_contrib.push_back({d[f_mid] % 10, f_mid});
    rem_contrib.push_back({d[f_small] % 10, f_small});
    sort(rem_contrib.rbegin(), rem_contrib.rend());

    for (int i = 0; i < rem_cols; ++i) {
        final_widths[rem_contrib[i].second]++;
    }

    t_start[f_large] = 0;
    t_end[f_large] = final_widths[f_large] - 1;

    t_start[f_small] = final_widths[f_large];
    t_end[f_small] = final_widths[f_large] + final_widths[f_small] - 1;

    t_start[f_mid] = final_widths[f_large] + final_widths[f_small];
    t_end[f_mid] = 9;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    for (int i = 0; i < 100; ++i) {
        cin >> flavors[i];
    }
    
    precompute_target_regions();

    for (int t = 1; t <= 100; ++t) {
        int p;
        cin >> p;

        int empty_count = 0;
        int r_new = -1, c_new = -1;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (grid[i][j] == 0) {
                    empty_count++;
                    if (empty_count == p) {
                        r_new = i;
                        c_new = j;
                        break;
                    }
                }
            }
            if (r_new != -1) break;
        }
        grid[r_new][c_new] = flavors[t - 1];
        
        char best_move = 'F';
        long long best_score = -5e18; 

        char moves[] = {'F', 'B', 'L', 'R'};
        for (char move : moves) {
            Grid next_grid = tilt(grid, move);
            int adj = adjacency_score(next_grid);
            int dist = distance_score(next_grid);
            
            long long w_adj = (long long)t * t;
            long long w_dist = (101 - t);

            long long current_score = w_adj * adj - w_dist * dist;
            
            if (current_score > best_score) {
                best_score = current_score;
                best_move = move;
            }
        }
        
        cout << best_move << endl;
        if (t < 100) {
          grid = tilt(grid, best_move);
        }
    }

    return 0;
}