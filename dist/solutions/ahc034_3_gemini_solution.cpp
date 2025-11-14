#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

struct Pos {
    int r, c;
};

int dist(const Pos& a, const Pos& b) {
    return abs(a.r - b.r) + abs(a.c - b.c);
}

const int N_GRID = 20;
vector<vector<int>> h(N_GRID, vector<int>(N_GRID));
vector<Pos> sources;
vector<Pos> sinks;

Pos cur_pos = {0, 0};
int cur_load = 0;
vector<string> ops;

void move_to(const Pos& target) {
    while (cur_pos.r < target.r) {
        ops.push_back("D");
        cur_pos.r++;
    }
    while (cur_pos.r > target.r) {
        ops.push_back("U");
        cur_pos.r--;
    }
    while (cur_pos.c < target.c) {
        ops.push_back("R");
        cur_pos.c++;
    }
    while (cur_pos.c > target.c) {
        ops.push_back("L");
        cur_pos.c--;
    }
}

void load(int r, int c, int amount) {
    ops.push_back("+" + to_string(amount));
    h[r][c] -= amount;
    cur_load += amount;
}

void unload(int r, int c, int amount) {
    ops.push_back("-" + to_string(amount));
    h[r][c] += amount;
    cur_load -= amount;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N_in;
    cin >> N_in;

    for (int i = 0; i < N_GRID; ++i) {
        for (int j = 0; j < N_GRID; ++j) {
            cin >> h[i][j];
            if (h[i][j] > 0) {
                sources.push_back({i, j});
            } else if (h[i][j] < 0) {
                sinks.push_back({i, j});
            }
        }
    }

    while (!sources.empty() || cur_load > 0) {
        if (cur_load == 0) {
            if (sources.empty()) break;
            
            int best_s_idx = -1;
            long long min_eval = -1;

            for (int i = 0; i < sources.size(); ++i) {
                Pos s = sources[i];
                if (sinks.empty()) {
                    continue;
                }

                int min_dist_st = 1e9;
                for (int j = 0; j < sinks.size(); ++j) {
                    min_dist_st = min(min_dist_st, dist(s, sinks[j]));
                }
                
                long long eval = (long long)dist(cur_pos, s) * 100 + (long long)min_dist_st * (100 + h[s.r][s.c]);
                
                if (best_s_idx == -1 || eval < min_eval) {
                    min_eval = eval;
                    best_s_idx = i;
                }
            }
            
            Pos target_s = sources[best_s_idx];
            move_to(target_s);
            
            int amount_to_load = h[target_s.r][target_s.c];
            load(target_s.r, target_s.c, amount_to_load);
            
            sources.erase(sources.begin() + best_s_idx);

        } else { // cur_load > 0
            if (sinks.empty()) break; 
            
            int best_t_idx = -1;
            double min_eval = -1.0;
            
            for (int i = 0; i < sinks.size(); ++i) {
                Pos t = sinks[i];
                long long unload_amount = min(cur_load, -h[t.r][t.c]);
                if (unload_amount == 0) continue;

                double eval = (double)dist(cur_pos, t) * (100.0 + cur_load) / unload_amount;
                
                if (best_t_idx == -1 || eval < min_eval) {
                    min_eval = eval;
                    best_t_idx = i;
                }
            }
            
            Pos target_t = sinks[best_t_idx];
            move_to(target_t);
            
            int amount_to_unload = min(cur_load, -h[target_t.r][target_t.c]);
            unload(target_t.r, target_t.c, amount_to_unload);

            if (h[target_t.r][target_t.c] == 0) {
                sinks.erase(sinks.begin() + best_t_idx);
            }
        }
    }

    for (const string& op : ops) {
        cout << op << "\n";
    }

    return 0;
}