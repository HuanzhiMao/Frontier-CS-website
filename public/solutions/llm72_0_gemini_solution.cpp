#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <unordered_map>

using namespace std;

// Represents the static properties of a vehicle
struct VehicleInfo {
    int id;
    int len;
    bool is_horiz;
    int fixed_coord; // row for horizontal, col for vertical
};

// A state is defined by the variable coordinates of all vehicles
using State = vector<int>;

// Custom hasher for using State (vector<int>) in unordered_map
struct StateHasher {
    size_t operator()(const State& s) const {
        size_t h = s.size();
        for (int i : s) {
            h ^= (size_t)i + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

// Global variables to hold puzzle information
vector<VehicleInfo> vehicle_infos;
State initial_state;
int red_car_idx = -1;

// Parses the input grid to identify vehicles and their properties
void parse_input() {
    vector<vector<int>> board(6, vector<int>(6));
    map<int, vector<pair<int, int>>> vehicle_coords;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            cin >> board[i][j];
            if (board[i][j] != 0) {
                vehicle_coords[board[i][j]].push_back({i, j});
            }
        }
    }

    for (auto const& [id, coords] : vehicle_coords) {
        VehicleInfo v_info;
        v_info.id = id;
        v_info.len = coords.size();
        
        int r_min = 6, c_min = 6;
        for(auto p : coords) {
            r_min = min(r_min, p.first);
            c_min = min(c_min, p.second);
        }

        if (coords.size() > 1 && coords[0].first == coords[1].first) {
            v_info.is_horiz = true;
            v_info.fixed_coord = r_min;
        } else {
            v_info.is_horiz = false;
            v_info.fixed_coord = c_min;
        }
        vehicle_infos.push_back(v_info);
    }

    sort(vehicle_infos.begin(), vehicle_infos.end(), [](const VehicleInfo& a, const VehicleInfo& b){
        return a.id < b.id;
    });

    for(size_t i=0; i<vehicle_infos.size(); ++i) {
        if (vehicle_infos[i].id == 1) red_car_idx = i;
    }

    initial_state.resize(vehicle_infos.size());
    for(size_t i = 0; i < vehicle_infos.size(); ++i) {
        int id = vehicle_infos[i].id;
        int r_min = 6, c_min = 6;
        for(auto p : vehicle_coords[id]) {
            r_min = min(r_min, p.first);
            c_min = min(c_min, p.second);
        }
        if (vehicle_infos[i].is_horiz) {
            initial_state[i] = c_min;
        } else {
            initial_state[i] = r_min;
        }
    }
}

// Recursively finds all valid placements of other vehicles around the red car in its pre-goal position
void generate_pre_goal_states_recursive(int k, State& current_pos, bool board[6][6], queue<State>& q, unordered_map<State, int, StateHasher>& dist) {
    if (k == (int)vehicle_infos.size()) {
        q.push(current_pos);
        dist[current_pos] = 1;
        return;
    }

    if (k == red_car_idx) {
        generate_pre_goal_states_recursive(k + 1, current_pos, board, q, dist);
        return;
    }
    
    const auto& v_info = vehicle_infos[k];
    if (v_info.is_horiz) {
        for (int c = 0; c <= 6 - v_info.len; ++c) {
            bool possible = true;
            for (int i = 0; i < v_info.len; ++i) {
                if (board[v_info.fixed_coord][c + i]) {
                    possible = false;
                    break;
                }
            }
            if (possible) {
                for (int i = 0; i < v_info.len; ++i) board[v_info.fixed_coord][c + i] = true;
                current_pos[k] = c;
                generate_pre_goal_states_recursive(k + 1, current_pos, board, q, dist);
                for (int i = 0; i < v_info.len; ++i) board[v_info.fixed_coord][c + i] = false;
            }
        }
    } else {
        for (int r = 0; r <= 6 - v_info.len; ++r) {
            bool possible = true;
            for (int i = 0; i < v_info.len; ++i) {
                if (board[r + i][v_info.fixed_coord]) {
                    possible = false;
                    break;
                }
            }
            if (possible) {
                for (int i = 0; i < v_info.len; ++i) board[r + i][v_info.fixed_coord] = true;
                current_pos[k] = r;
                generate_pre_goal_states_recursive(k + 1, current_pos, board, q, dist);
                for (int i = 0; i < v_info.len; ++i) board[r + i][v_info.fixed_coord] = false;
            }
        }
    }
}

// Runs a reverse BFS from all pre-goal states to calculate the minimum steps to solve from any state
unordered_map<State, int, StateHasher> run_reverse_bfs() {
    unordered_map<State, int, StateHasher> dist;
    queue<State> q;

    State s(vehicle_infos.size());
    bool board[6][6] = {false};
    
    const auto& red_car_info = vehicle_infos[red_car_idx];
    s[red_car_idx] = 5; // Pre-goal position for a length-2 car

    for (int i = 0; i < red_car_info.len; ++i) {
        board[red_car_info.fixed_coord][s[red_car_idx] + i] = true;
    }
    
    generate_pre_goal_states_recursive(0, s, board, q, dist);

    while (!q.empty()) {
        State u = q.front();
        q.pop();

        int board_u[6][6] = {0};
        for (size_t i = 0; i < vehicle_infos.size(); ++i) {
            const auto& v = vehicle_infos[i];
            if (v.is_horiz) {
                for (int j = 0; j < v.len; ++j) board_u[v.fixed_coord][u[i] + j] = v.id;
            } else {
                for (int j = 0; j < v.len; ++j) board_u[u[i] + j][v.fixed_coord] = v.id;
            }
        }

        for (size_t i = 0; i < vehicle_infos.size(); ++i) {
            const auto& v_info = vehicle_infos[i];
            
            if (v_info.is_horiz) {
                if (u[i] + v_info.len < 6 && board_u[v_info.fixed_coord][u[i] + v_info.len] == 0) {
                    State next_s = u; next_s[i]++;
                    if (dist.find(next_s) == dist.end()) { dist[next_s] = dist[u] + 1; q.push(next_s); }
                }
                if (u[i] > 0 && board_u[v_info.fixed_coord][u[i] - 1] == 0) {
                    State next_s = u; next_s[i]--;
                    if (dist.find(next_s) == dist.end()) { dist[next_s] = dist[u] + 1; q.push(next_s); }
                }
            } else {
                if (u[i] + v_info.len < 6 && board_u[u[i] + v_info.len][v_info.fixed_coord] == 0) {
                    State next_s = u; next_s[i]++;
                    if (dist.find(next_s) == dist.end()) { dist[next_s] = dist[u] + 1; q.push(next_s); }
                }
                if (u[i] > 0 && board_u[u[i] - 1][v_info.fixed_coord] == 0) {
                    State next_s = u; next_s[i]--;
                    if (dist.find(next_s) == dist.end()) { dist[next_s] = dist[u] + 1; q.push(next_s); }
                }
            }
        }
    }
    return dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    parse_input();
    
    auto dist_to_solve = run_reverse_bfs();

    State best_state = initial_state;
    int max_dist = 0;
    if (dist_to_solve.count(initial_state)) {
        max_dist = dist_to_solve[initial_state];
    }

    queue<State> q_fwd;
    q_fwd.push(initial_state);
    unordered_map<State, pair<State, pair<int, char>>, StateHasher> parent_fwd;
    unordered_map<State, bool, StateHasher> visited_fwd;
    visited_fwd[initial_state] = true;

    while (!q_fwd.empty()) {
        State u = q_fwd.front();
        q_fwd.pop();
        
        if (dist_to_solve.count(u) && dist_to_solve[u] > max_dist) {
            max_dist = dist_to_solve[u];
            best_state = u;
        }

        int board_u[6][6] = {0};
        for (size_t i = 0; i < vehicle_infos.size(); ++i) {
            const auto& v = vehicle_infos[i];
            if (v.is_horiz) {
                for (int j = 0; j < v.len; ++j) board_u[v.fixed_coord][u[i] + j] = v.id;
            } else {
                for (int j = 0; j < v.len; ++j) board_u[u[i] + j][v.fixed_coord] = v.id;
            }
        }

        for (size_t i = 0; i < vehicle_infos.size(); ++i) {
            const auto& v_info = vehicle_infos[i];
            
            if (v_info.is_horiz) {
                if (u[i] + v_info.len < 6 && board_u[v_info.fixed_coord][u[i] + v_info.len] == 0) {
                    State next_s = u; next_s[i]++;
                    if (visited_fwd.find(next_s) == visited_fwd.end()) {
                        visited_fwd[next_s] = true; parent_fwd[next_s] = {u, {v_info.id, 'R'}}; q_fwd.push(next_s);
                    }
                }
                if (u[i] > 0 && board_u[v_info.fixed_coord][u[i] - 1] == 0) {
                    State next_s = u; next_s[i]--;
                    if (visited_fwd.find(next_s) == visited_fwd.end()) {
                        visited_fwd[next_s] = true; parent_fwd[next_s] = {u, {v_info.id, 'L'}}; q_fwd.push(next_s);
                    }
                }
            } else {
                if (u[i] + v_info.len < 6 && board_u[u[i] + v_info.len][v_info.fixed_coord] == 0) {
                    State next_s = u; next_s[i]++;
                    if (visited_fwd.find(next_s) == visited_fwd.end()) {
                        visited_fwd[next_s] = true; parent_fwd[next_s] = {u, {v_info.id, 'D'}}; q_fwd.push(next_s);
                    }
                }
                if (u[i] > 0 && board_u[u[i] - 1][v_info.fixed_coord] == 0) {
                    State next_s = u; next_s[i]--;
                    if (visited_fwd.find(next_s) == visited_fwd.end()) {
                        visited_fwd[next_s] = true; parent_fwd[next_s] = {u, {v_info.id, 'U'}}; q_fwd.push(next_s);
                    }
                }
            }
        }
    }

    vector<pair<int, char>> path;
    State curr = best_state;
    while (curr != initial_state) {
        auto p = parent_fwd.at(curr);
        path.push_back(p.second);
        curr = p.first;
    }
    reverse(path.begin(), path.end());

    cout << max_dist << " " << path.size() << endl;
    for (const auto& move : path) {
        cout << move.first << " " << move.second << endl;
    }

    return 0;
}