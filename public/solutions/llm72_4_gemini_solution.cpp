#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

const int GRID_SIZE = 6;

struct Vehicle {
    int id;
    int len;
    bool is_horizontal;
    int fixed_coord; // row for H, col for V
};

struct Move {
    int id;
    char dir;
};

// For hashing vector<int> to use in unordered_map/set
struct vector_hash {
    size_t operator()(const vector<int>& v) const {
        size_t seed = v.size();
        for (int i : v) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

vector<Vehicle> vehicles;
map<int, int> id_to_idx;
int red_car_idx = -1;

vector<pair<vector<int>, Move>> get_neighbors(const vector<int>& state) {
    vector<pair<vector<int>, Move>> neighbors;
    int grid[GRID_SIZE][GRID_SIZE] = {0};

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles[i];
        int pos = state[i];
        if (v.is_horizontal) {
            for (int j = 0; j < v.len; ++j) {
                if (pos + j >= 0 && pos + j < GRID_SIZE) {
                    grid[v.fixed_coord][pos + j] = v.id;
                }
            }
        } else {
            for (int j = 0; j < v.len; ++j) {
                if (pos + j >= 0 && pos + j < GRID_SIZE) {
                    grid[pos + j][v.fixed_coord] = v.id;
                }
            }
        }
    }

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles[i];
        int pos = state[i];

        if (v.is_horizontal) {
            // Move right
            int front_c = pos + v.len;
            if (i == red_car_idx) {
                if (pos < GRID_SIZE) { 
                    bool blocked = false;
                    if (front_c < GRID_SIZE && grid[v.fixed_coord][front_c] != 0) {
                        blocked = true;
                    }
                    if (!blocked) {
                        vector<int> next_state = state;
                        next_state[i]++;
                        neighbors.push_back({next_state, {v.id, 'R'}});
                    }
                }
            } else {
                if (front_c < GRID_SIZE && grid[v.fixed_coord][front_c] == 0) {
                    vector<int> next_state = state;
                    next_state[i]++;
                    neighbors.push_back({next_state, {v.id, 'R'}});
                }
            }

            // Move left
            int back_c = pos - 1;
            if (back_c >= 0 && grid[v.fixed_coord][back_c] == 0) {
                vector<int> next_state = state;
                next_state[i]--;
                neighbors.push_back({next_state, {v.id, 'L'}});
            }
        } else { // Vertical
            // Move down
            int front_r = pos + v.len;
            if (front_r < GRID_SIZE && grid[front_r][v.fixed_coord] == 0) {
                vector<int> next_state = state;
                next_state[i]++;
                neighbors.push_back({next_state, {v.id, 'D'}});
            }

            // Move up
            int back_r = pos - 1;
            if (back_r >= 0 && grid[back_r][v.fixed_coord] == 0) {
                vector<int> next_state = state;
                next_state[i]--;
                neighbors.push_back({next_state, {v.id, 'U'}});
            }
        }
    }
    return neighbors;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<vector<int>> board(GRID_SIZE, vector<int>(GRID_SIZE));
    set<int> vehicle_ids_set;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cin >> board[i][j];
            if (board[i][j] != 0) {
                vehicle_ids_set.insert(board[i][j]);
            }
        }
    }

    vector<int> sorted_ids(vehicle_ids_set.begin(), vehicle_ids_set.end());
    
    vehicles.resize(sorted_ids.size());
    for(size_t i = 0; i < sorted_ids.size(); ++i) {
        id_to_idx[sorted_ids[i]] = i;
        if(sorted_ids[i] == 1) red_car_idx = i;
    }

    vector<int> initial_state(vehicles.size());
    vector<bool> processed(101, false);

    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            int id = board[r][c];
            if (id != 0 && !processed[id]) {
                processed[id] = true;
                int idx = id_to_idx[id];
                vehicles[idx].id = id;
                int len = 0;
                bool is_h = (c + 1 < GRID_SIZE && board[r][c + 1] == id);
                vehicles[idx].is_horizontal = is_h;
                if (is_h) {
                    vehicles[idx].fixed_coord = r;
                    initial_state[idx] = c;
                    int cc = c;
                    while (cc < GRID_SIZE && board[r][cc] == id) {
                        len++;
                        cc++;
                    }
                } else {
                    vehicles[idx].fixed_coord = c;
                    initial_state[idx] = r;
                    int rr = r;
                    while (rr < GRID_SIZE && board[rr][c] == id) {
                        len++;
                        rr++;
                    }
                }
                vehicles[idx].len = len;
            }
        }
    }

    queue<pair<vector<int>, vector<Move>>> fwd_q;
    fwd_q.push({initial_state, {}});
    
    unordered_set<vector<int>, vector_hash> fwd_visited;
    fwd_visited.insert(initial_state);

    int max_dist = -1;
    vector<Move> best_path;
    
    int states_to_check = 50000 / (1 + vehicles.size());

    while(!fwd_q.empty() && states_to_check > 0) {
        states_to_check--;
        vector<int> current_state = fwd_q.front().first;
        vector<Move> current_path = fwd_q.front().second;
        fwd_q.pop();

        unordered_map<vector<int>, int, vector_hash> solve_dist;
        queue<vector<int>> solve_q;

        solve_q.push(current_state);
        solve_dist[current_state] = 0;
        int current_puzzle_dist = -1;

        while(!solve_q.empty()){
            vector<int> s = solve_q.front();
            solve_q.pop();

            if(s[red_car_idx] == GRID_SIZE){
                current_puzzle_dist = solve_dist[s];
                break;
            }
            if (solve_dist[s] > 35) { // Pruning solver
                 break;
            }
            
            auto neighbors = get_neighbors(s);
            for(const auto& edge : neighbors){
                const auto& next_s = edge.first;
                if(solve_dist.find(next_s) == solve_dist.end()){
                    solve_dist[next_s] = solve_dist[s] + 1;
                    solve_q.push(next_s);
                }
            }
        }
        
        if (current_puzzle_dist > max_dist) {
            max_dist = current_puzzle_dist;
            best_path = current_path;
        }

        if (current_path.size() >= 20) continue; // Limit generation path length

        auto neighbors = get_neighbors(current_state);
        for(const auto& edge : neighbors){
            const auto& next_s = edge.first;
            const auto& move = edge.second;
            if(fwd_visited.find(next_s) == fwd_visited.end()){
                fwd_visited.insert(next_s);
                vector<Move> next_path = current_path;
                next_path.push_back(move);
                fwd_q.push({next_s, next_path});
            }
        }
    }

    cout << max_dist << " " << best_path.size() << endl;
    for(const auto& move : best_path){
        cout << move.id << " " << move.dir << endl;
    }

    return 0;
}