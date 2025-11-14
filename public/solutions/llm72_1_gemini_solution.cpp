#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>

using namespace std;

struct Vehicle {
    int id;
    int len;
    bool is_horz;
    int fixed_coord;
};

vector<Vehicle> vehicle_infos;
int num_vehicles;

using State = vector<int>;

struct StateHash {
    size_t operator()(const State& s) const {
        size_t h = 0;
        for (int x : s) {
            h ^= hash<int>()(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

bool occupied[6][6];

void fill_board(const State& s, int exclude_id = -1) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            occupied[i][j] = false;
        }
    }
    for (int i = 0; i < num_vehicles; ++i) {
        if (vehicle_infos[i].id == exclude_id) continue;
        
        int var_coord = s[i];
        if (vehicle_infos[i].is_horz) {
            int r = vehicle_infos[i].fixed_coord;
            for (int k = 0; k < vehicle_infos[i].len; ++k) {
                int c = var_coord + k;
                if (r >= 0 && r < 6 && c >= 0 && c < 6) {
                    occupied[r][c] = true;
                }
            }
        } else {
            int c = vehicle_infos[i].fixed_coord;
            for (int k = 0; k < vehicle_infos[i].len; ++k) {
                int r = var_coord + k;
                if (r >= 0 && r < 6 && c >= 0 && c < 6) {
                    occupied[r][c] = true;
                }
            }
        }
    }
}

int solve(const State& start_state) {
    if (start_state[0] == 6) return 0;

    queue<State> q;
    q.push(start_state);
    
    unordered_map<State, int, StateHash> dist;
    dist[start_state] = 0;

    while (!q.empty()) {
        State current_state = q.front();
        q.pop();

        int d = dist[current_state];
        
        if (current_state[0] == 6) {
            return d;
        }

        for (int i = 0; i < num_vehicles; ++i) {
            fill_board(current_state, vehicle_infos[i].id);

            // Move forward
            State next_state_f = current_state;
            next_state_f[i]++;
            bool possible_f = true;
            if (i == 0) { // Red car
                if (next_state_f[i] > 6) { possible_f = false; }
                else {
                    int r = vehicle_infos[i].fixed_coord;
                    int check_c = next_state_f[i] + vehicle_infos[i].len - 1;
                    if (check_c >= 0 && check_c < 6 && occupied[r][check_c]) {
                        possible_f = false;
                    }
                }
            } else { // Other vehicles
                if (vehicle_infos[i].is_horz) {
                    if (next_state_f[i] + vehicle_infos[i].len > 6) { possible_f = false; }
                    else {
                        int r = vehicle_infos[i].fixed_coord;
                        int check_c = next_state_f[i] + vehicle_infos[i].len - 1;
                        if (occupied[r][check_c]) { possible_f = false; }
                    }
                } else {
                    if (next_state_f[i] + vehicle_infos[i].len > 6) { possible_f = false; }
                    else {
                        int c = vehicle_infos[i].fixed_coord;
                        int check_r = next_state_f[i] + vehicle_infos[i].len - 1;
                        if (occupied[check_r][c]) { possible_f = false; }
                    }
                }
            }
            if (possible_f && dist.find(next_state_f) == dist.end()) {
                dist[next_state_f] = d + 1;
                q.push(next_state_f);
            }

            // Move backward
            State next_state_b = current_state;
            next_state_b[i]--;
            bool possible_b = true;
            if (next_state_b[i] < 0) { possible_b = false; }
            else {
                if (vehicle_infos[i].is_horz) {
                    int r = vehicle_infos[i].fixed_coord;
                    int check_c = next_state_b[i];
                    if (occupied[r][check_c]) { possible_b = false; }
                } else {
                    int c = vehicle_infos[i].fixed_coord;
                    int check_r = next_state_b[i];
                    if (occupied[check_r][c]) { possible_b = false; }
                }
            }
            if (possible_b && dist.find(next_state_b) == dist.end()) {
                dist[next_state_b] = d + 1;
                q.push(next_state_b);
            }
        }
    }
    return -1;
}

struct Node {
    State s;
    vector<pair<int, char>> path;
};

State parse_and_init() {
    vector<vector<int>> board(6, vector<int>(6));
    int max_id = 0;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            cin >> board[i][j];
            max_id = max(max_id, board[i][j]);
        }
    }
    num_vehicles = max_id;
    if (num_vehicles == 0) return {};
    vehicle_infos.resize(num_vehicles);
    State initial_state(num_vehicles);
    vector<bool> found(num_vehicles + 1, false);

    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            int id = board[r][c];
            if (id != 0 && !found[id]) {
                found[id] = true;
                Vehicle v;
                v.id = id;
                if (c + 1 < 6 && board[r][c + 1] == id) { // Horizontal
                    v.is_horz = true;
                    v.fixed_coord = r;
                    initial_state[id - 1] = c;
                    int len = 0;
                    while (c + len < 6 && board[r][c + len] == id) len++;
                    v.len = len;
                } else { // Vertical
                    v.is_horz = false;
                    v.fixed_coord = c;
                    initial_state[id - 1] = r;
                    int len = 0;
                    while (r + len < 6 && board[r + len][c] == id) len++;
                    v.len = len;
                }
                vehicle_infos[id - 1] = v;
            }
        }
    }
    return initial_state;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    State initial_state = parse_and_init();
    if(num_vehicles == 0) {
        cout << "0 0" << endl;
        return 0;
    }

    queue<Node> q;
    q.push({initial_state, {}});
    
    unordered_set<State, StateHash> visited_outer;
    visited_outer.insert(initial_state);

    int max_solve_steps = -1;
    vector<pair<int, char>> best_path;

    auto start_time = chrono::high_resolution_clock::now();

    while (!q.empty()) {
        auto now = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = now - start_time;
        if (elapsed.count() > 1.9) {
            break;
        }

        Node current_node = q.front();
        q.pop();

        int d = solve(current_node.s);
        if (d > max_solve_steps) {
            max_solve_steps = d;
            best_path = current_node.path;
        }

        for (int i = 0; i < num_vehicles; ++i) {
            fill_board(current_node.s, vehicle_infos[i].id);
            
            // Move forward
            State next_state_f = current_node.s;
            next_state_f[i]++;
            bool possible_f = true;
            if (i == 0) { // Red car
                if (next_state_f[i] > 6) { possible_f = false; }
                else {
                    int r = vehicle_infos[i].fixed_coord;
                    int check_c = next_state_f[i] + vehicle_infos[i].len - 1;
                    if (check_c < 6 && occupied[r][check_c]) {
                        possible_f = false;
                    }
                }
            } else { // Other vehicles
                if (vehicle_infos[i].is_horz) {
                    if (next_state_f[i] + vehicle_infos[i].len > 6) { possible_f = false; }
                    else {
                        int r = vehicle_infos[i].fixed_coord;
                        int check_c = next_state_f[i] + vehicle_infos[i].len - 1;
                        if (occupied[r][check_c]) { possible_f = false; }
                    }
                } else {
                    if (next_state_f[i] + vehicle_infos[i].len > 6) { possible_f = false; }
                    else {
                        int c = vehicle_infos[i].fixed_coord;
                        int check_r = next_state_f[i] + vehicle_infos[i].len - 1;
                        if (occupied[check_r][c]) { possible_f = false; }
                    }
                }
            }
            if (possible_f && visited_outer.find(next_state_f) == visited_outer.end()) {
                visited_outer.insert(next_state_f);
                auto next_path = current_node.path;
                next_path.push_back({vehicle_infos[i].id, vehicle_infos[i].is_horz ? 'R' : 'D'});
                q.push({next_state_f, next_path});
            }

            // Move backward
            State next_state_b = current_node.s;
            next_state_b[i]--;
            bool possible_b = true;
            if (next_state_b[i] < 0) { possible_b = false; }
            else {
                if (vehicle_infos[i].is_horz) {
                    int r = vehicle_infos[i].fixed_coord;
                    int check_c = next_state_b[i];
                    if (occupied[r][check_c]) { possible_b = false; }
                } else {
                    int c = vehicle_infos[i].fixed_coord;
                    int check_r = next_state_b[i];
                    if (occupied[check_r][c]) { possible_b = false; }
                }
            }
            if (possible_b && visited_outer.find(next_state_b) == visited_outer.end()) {
                visited_outer.insert(next_state_b);
                auto next_path = current_node.path;
                next_path.push_back({vehicle_infos[i].id, vehicle_infos[i].is_horz ? 'L' : 'U'});
                q.push({next_state_b, next_path});
            }
        }
    }

    if (max_solve_steps == -1) {
        max_solve_steps = solve(initial_state);
        best_path.clear();
    }
    
    cout << max_solve_steps << " " << best_path.size() << endl;
    for (const auto& move : best_path) {
        cout << move.first << " " << move.second << endl;
    }

    return 0;
}