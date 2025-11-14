#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cstdint>

// Using std namespace for competitive programming
using namespace std;

// Constants
const int GRID_SIZE = 6;

// Data structures
struct Vehicle {
    int id;
    int len;
    bool is_horiz;
    int fixed_coord;
};

struct Move {
    int vehicle_id;
    char dir;
};

// Global variables for easy access in functions
vector<Vehicle> vehicles;
map<int, int> id_to_idx;
int num_vehicles = 0;

// State representation
using StateVec = vector<int>;
using StateKey = uint64_t;

// State conversion functions
StateKey to_key(const StateVec& s) {
    StateKey key = 0;
    for (int pos : s) {
        // Use 4 bits per position, as position can be up to 6.
        key = (key << 4) | pos;
    }
    return key;
}

StateVec from_key(StateKey key) {
    StateVec s(num_vehicles);
    for (int i = num_vehicles - 1; i >= 0; --i) {
        s[i] = key & 0xF; // 0xF is mask for 4 bits
        key >>= 4;
    }
    return s;
}

// Check if a state is valid (no collisions, within bounds)
bool is_valid(const StateVec& s) {
    int red_car_idx = id_to_idx.at(1);
    for (int i = 0; i < num_vehicles; ++i) {
        int len = vehicles[i].len;
        int pos = s[i];
        if (i == red_car_idx) {
            // Red car can move off board to exit
            if (pos < 0 || pos > GRID_SIZE) return false;
        } else {
            // Other vehicles must stay on board
            if (pos < 0 || pos + len > GRID_SIZE) return false;
        }
    }

    char board[GRID_SIZE][GRID_SIZE];
    memset(board, 0, sizeof(board));

    for (int i = 0; i < num_vehicles; ++i) {
        int var_coord = s[i];
        if (vehicles[i].is_horiz) {
            int r = vehicles[i].fixed_coord;
            for (int k = 0; k < vehicles[i].len; ++k) {
                int c = var_coord + k;
                if (c >= 0 && c < GRID_SIZE) {
                    if (r < 0 || r >= GRID_SIZE) return false;
                    if (board[r][c] != 0) return false;
                    board[r][c] = 1;
                }
            }
        } else {
            int c = vehicles[i].fixed_coord;
            for (int k = 0; k < vehicles[i].len; ++k) {
                int r = var_coord + k;
                if (r >= 0 && r < GRID_SIZE) {
                    if (c < 0 || c >= GRID_SIZE) return false;
                    if (board[r][c] != 0) return false;
                    board[r][c] = 1;
                }
            }
        }
    }
    return true;
}

// Memoization table for solution distances
map<StateKey, int> memo_sol_dist;

// Calculate minimum steps to solve from a given state using BFS
int calculate_sol_dist(StateKey start_key) {
    if (memo_sol_dist.count(start_key)) {
        return memo_sol_dist.at(start_key);
    }

    queue<pair<StateKey, int>> q;
    map<StateKey, int> visited_dist;

    q.push({start_key, 0});
    visited_dist[start_key] = 0;

    int red_car_idx = id_to_idx.at(1);

    while (!q.empty()) {
        auto [curr_key, dist] = q.front();
        q.pop();

        StateVec s_curr = from_key(curr_key);

        if (s_curr[red_car_idx] == GRID_SIZE) {
            memo_sol_dist[start_key] = dist;
            return dist;
        }

        for (int i = 0; i < num_vehicles; ++i) {
            // Move forward
            StateVec s_next_vec_fwd = s_curr;
            s_next_vec_fwd[i]++;
            if (is_valid(s_next_vec_fwd)) {
                StateKey next_key = to_key(s_next_vec_fwd);
                if (!visited_dist.count(next_key)) {
                    visited_dist[next_key] = dist + 1;
                    q.push({next_key, dist + 1});
                }
            }

            // Move backward
            StateVec s_next_vec_bwd = s_curr;
            s_next_vec_bwd[i]--;
            if (is_valid(s_next_vec_bwd)) {
                StateKey next_key = to_key(s_next_vec_bwd);
                if (!visited_dist.count(next_key)) {
                    visited_dist[next_key] = dist + 1;
                    q.push({next_key, dist + 1});
                }
            }
        }
    }

    return -1; // Unsolvable, should not be reached
}

void solve() {
    // Parse input board
    map<int, vector<pair<int, int>>> vehicle_cells;
    int max_id = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int id;
            cin >> id;
            if (id != 0) {
                vehicle_cells[id].push_back({i, j});
                max_id = max(max_id, id);
            }
        }
    }
    
    num_vehicles = max_id;
    vehicles.resize(num_vehicles);
    StateVec initial_state(num_vehicles);
    
    for (int id = 1; id <= num_vehicles; ++id) {
        int idx = id - 1;
        id_to_idx[id] = idx;
        
        auto& cells = vehicle_cells.at(id);
        sort(cells.begin(), cells.end());

        Vehicle v;
        v.id = id;
        v.len = cells.size();
        if (v.len > 1 && cells[0].first == cells[1].first) {
            v.is_horiz = true;
            v.fixed_coord = cells[0].first;
            initial_state[idx] = cells[0].second;
        } else {
            v.is_horiz = false;
            v.fixed_coord = cells[0].second;
            initial_state[idx] = cells[0].first;
        }
        vehicles[idx] = v;
    }

    // BFS to explore reachable states and find the one with max solution difficulty
    queue<pair<StateKey, vector<Move>>> q_reach;
    map<StateKey, bool> visited_reach;

    StateKey initial_key = to_key(initial_state);
    q_reach.push({initial_key, {}});
    visited_reach[initial_key] = true;

    int initial_sol_dist = calculate_sol_dist(initial_key);
    int max_sol_len = initial_sol_dist;
    vector<Move> best_path = {};

    while (!q_reach.empty()) {
        auto [curr_key, path_curr] = q_reach.front();
        q_reach.pop();

        StateVec s_curr = from_key(curr_key);

        for (int i = 0; i < num_vehicles; ++i) {
            // Try moving forward (Right or Down)
            StateVec s_next_fwd = s_curr;
            s_next_fwd[i]++;
            if (is_valid(s_next_fwd)) {
                StateKey next_key = to_key(s_next_fwd);
                if (!visited_reach.count(next_key)) {
                    vector<Move> path_next = path_curr;
                    path_next.push_back({vehicles[i].id, vehicles[i].is_horiz ? 'R' : 'D'});
                    visited_reach[next_key] = true;
                    q_reach.push({next_key, path_next});

                    int sol_dist_next = calculate_sol_dist(next_key);
                    if (sol_dist_next > max_sol_len) {
                        max_sol_len = sol_dist_next;
                        best_path = path_next;
                    }
                }
            }

            // Try moving backward (Left or Up)
            StateVec s_next_bwd = s_curr;
            s_next_bwd[i]--;
            if (is_valid(s_next_bwd)) {
                StateKey next_key = to_key(s_next_bwd);
                if (!visited_reach.count(next_key)) {
                    vector<Move> path_next = path_curr;
                    path_next.push_back({vehicles[i].id, vehicles[i].is_horiz ? 'L' : 'U'});
                    visited_reach[next_key] = true;
                    q_reach.push({next_key, path_next});
                    
                    int sol_dist_next = calculate_sol_dist(next_key);
                    if (sol_dist_next > max_sol_len) {
                        max_sol_len = sol_dist_next;
                        best_path = path_next;
                    }
                }
            }
        }
    }

    // Output the result
    cout << max_sol_len << " " << best_path.size() << endl;
    for (const auto& move : best_path) {
        cout << move.vehicle_id << " " << move.dir << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}