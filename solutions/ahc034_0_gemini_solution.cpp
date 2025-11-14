#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>

using namespace std;

int N_val;
vector<vector<int>> h;
int cur_r, cur_c;
int load;
vector<string> ops;

// Moves the truck from its current position to (r2, c2)
// and updates current position.
void move(int r2, int c2) {
    int dr = r2 - cur_r;
    int dc = c2 - cur_c;

    if (dr > 0) {
        for (int i = 0; i < dr; ++i) ops.push_back("D");
    } else {
        for (int i = 0; i < -dr; ++i) ops.push_back("U");
    }

    if (dc > 0) {
        for (int i = 0; i < dc; ++i) ops.push_back("R");
    } else {
        for (int i = 0; i < -dc; ++i) ops.push_back("L");
    }
    cur_r = r2;
    cur_c = c2;
}

// Loads d units of soil. Updates state and adds operation to list.
void do_load(int d) {
    ops.push_back("+" + to_string(d));
    load += d;
    h[cur_r][cur_c] -= d;
}

// Unloads d units of soil. Updates state and adds operation to list.
void do_unload(int d) {
    ops.push_back("-" + to_string(d));
    load -= d;
    h[cur_r][cur_c] += d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N_val;
    h.resize(N_val, vector<int>(N_val));
    vector<pair<int, int>> sources_pos, sinks_pos;

    for (int i = 0; i < N_val; ++i) {
        for (int j = 0; j < N_val; ++j) {
            cin >> h[i][j];
            if (h[i][j] > 0) {
                sources_pos.push_back({i, j});
            } else if (h[i][j] < 0) {
                sinks_pos.push_back({i, j});
            }
        }
    }

    cur_r = 0;
    cur_c = 0;
    load = 0;

    while (!sources_pos.empty()) {
        if (load == 0) {
            // Collection phase: truck is empty. Find the best source to start a trip.
            pair<int, int> best_s = {-1, -1};
            long long min_cost = -1;
            int best_s_idx = -1;

            for (int i = 0; i < sources_pos.size(); ++i) {
                const auto& s = sources_pos[i];
                if (sinks_pos.empty()) continue; // Should not happen due to sum(h)=0
                
                pair<int, int> t_s = {-1, -1};
                int min_dist_st = 1e9;
                
                // Find sink t_s closest to s
                for (const auto& t : sinks_pos) {
                    int d = abs(s.first - t.first) + abs(s.second - t.second);
                    if (d < min_dist_st) {
                        min_dist_st = d;
                        t_s = t;
                    } else if (d == min_dist_st) {
                        if (t_s.first == -1 || t.first < t_s.first || (t.first == t_s.first && t.second < t_s.second)) {
                            t_s = t;
                        }
                    }
                }
                
                // Cost is travel to source (empty) + travel to nearest sink (loaded)
                long long cost_to_s = (long long)(abs(cur_r - s.first) + abs(cur_c - s.second)) * 100;
                long long cost_s_to_t = (long long)min_dist_st * (100 + h[s.first][s.second]);
                long long current_total_cost = cost_to_s + cost_s_to_t;
                
                if (min_cost == -1 || current_total_cost < min_cost) {
                    min_cost = current_total_cost;
                    best_s = s;
                    best_s_idx = i;
                } else if (current_total_cost == min_cost) {
                    if (best_s.first == -1 || s.first < best_s.first || (s.first == best_s.first && s.second < best_s.second)) {
                        best_s = s;
                        best_s_idx = i;
                    }
                }
            }
            
            move(best_s.first, best_s.second);
            int amount_to_load = h[cur_r][cur_c];
            do_load(amount_to_load);

            // Remove source from list
            sources_pos[best_s_idx] = sources_pos.back();
            sources_pos.pop_back();
        }
        
        // Delivery phase: truck is loaded. Deliver until empty.
        while (load > 0) {
            if (sinks_pos.empty()) break; // Should not happen

            pair<int, int> best_t = {-1, -1};
            int min_dist = 1e9;
            int best_t_idx = -1;
            
            // Find closest sink
            for (int i = 0; i < sinks_pos.size(); ++i) {
                const auto& t = sinks_pos[i];
                int d = abs(cur_r - t.first) + abs(cur_c - t.second);
                if (d < min_dist) {
                    min_dist = d;
                    best_t = t;
                    best_t_idx = i;
                } else if (d == min_dist) {
                    if (best_t.first == -1 || t.first < best_t.first || (t.first == best_t.first && t.second < best_t.second)) {
                        best_t = t;
                        best_t_idx = i;
                    }
                }
            }
            
            move(best_t.first, best_t.second);
            int amount_to_unload = min(load, -h[cur_r][cur_c]);
            do_unload(amount_to_unload);

            // If sink is satisfied, remove from list
            if (h[cur_r][cur_c] == 0) {
                sinks_pos[best_t_idx] = sinks_pos.back();
                sinks_pos.pop_back();
            }
        }
    }

    for (const auto& op : ops) {
        cout << op << endl;
    }

    return 0;
}