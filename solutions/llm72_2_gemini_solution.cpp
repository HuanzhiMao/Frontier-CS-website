#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <unordered_map>

using namespace std;

struct Vehicle {
    int id;
    int r, c; // top-left corner for horizontal, or fixed column/row for vertical/horizontal
    int len;
    bool is_horiz;
};

struct Move {
    int id;
    char dir;
};

vector<Vehicle> g_vehicle_props;
int g_num_vehicles;

long long key_from_pos(const vector<int>& pos) {
    long long key = 0;
    for (int p : pos) {
        key = (key << 3) | p;
    }
    return key;
}

vector<int> pos_from_key(long long key) {
    vector<int> pos(g_num_vehicles);
    for (int i = g_num_vehicles - 1; i >= 0; --i) {
        pos[i] = key & 0b111;
        key >>= 3;
    }
    return pos;
}

int solve(const vector<int>& start_pos) {
    long long start_key = key_from_pos(start_pos);
    queue<pair<long long, int>> q;
    q.push({start_key, 0});
    unordered_map<long long, int> dist;
    dist[start_key] = 0;

    while (!q.empty()) {
        long long current_key = q.front().first;
        int d = q.front().second;
        q.pop();

        vector<int> current_pos = pos_from_key(current_key);
        if (current_pos[0] == 6) { // red car (ID 1, at index 0) needs to reach c=6
            return d;
        }

        int board[6][6];
        for(int r=0; r<6; ++r) for(int c=0; c<6; ++c) board[r][c] = 0;

        for (int i = 0; i < g_num_vehicles; ++i) {
            const auto& v = g_vehicle_props[i];
            if (v.is_horiz) {
                for (int k = 0; k < v.len; ++k) {
                    if (current_pos[i] + k < 6)
                        board[v.r][current_pos[i] + k] = v.id;
                }
            } else {
                for (int k = 0; k < v.len; ++k) {
                    if (current_pos[i] + k < 6)
                        board[current_pos[i] + k][v.c] = v.id;
                }
            }
        }
        
        for (int i = 0; i < g_num_vehicles; ++i) {
            const auto& v = g_vehicle_props[i];
            if (v.is_horiz) {
                // R
                int new_c = current_pos[i] + 1;
                bool possible = false;
                if (v.id == 1) {
                    if (new_c <= 6) {
                        int front_c = new_c + v.len - 1;
                        if (front_c < 6) {
                            if (board[v.r][front_c] == 0) possible = true;
                        } else {
                            possible = true;
                        }
                    }
                } else {
                    if (new_c + v.len - 1 < 6) {
                        if (board[v.r][new_c + v.len - 1] == 0) possible = true;
                    }
                }
                if (possible) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_c;
                    long long next_key = key_from_pos(next_pos);
                    if (dist.find(next_key) == dist.end()) {
                        dist[next_key] = d + 1;
                        q.push({next_key, d + 1});
                    }
                }
                // L
                int new_c_l = current_pos[i] - 1;
                if (new_c_l >= 0 && board[v.r][new_c_l] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_c_l;
                    long long next_key = key_from_pos(next_pos);
                    if (dist.find(next_key) == dist.end()) {
                        dist[next_key] = d + 1;
                        q.push({next_key, d + 1});
                    }
                }
            } else { // vertical
                // D
                int new_r = current_pos[i] + 1;
                if (new_r + v.len - 1 < 6 && board[new_r + v.len - 1][v.c] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_r;
                    long long next_key = key_from_pos(next_pos);
                    if (dist.find(next_key) == dist.end()) {
                        dist[next_key] = d + 1;
                        q.push({next_key, d + 1});
                    }
                }
                // U
                int new_r_u = current_pos[i] - 1;
                if (new_r_u >= 0 && board[new_r_u][v.c] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_r_u;
                    long long next_key = key_from_pos(next_pos);
                    if (dist.find(next_key) == dist.end()) {
                        dist[next_key] = d + 1;
                        q.push({next_key, d + 1});
                    }
                }
            }
        }
    }

    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    map<int, vector<pair<int, int>>> positions;
    int initial_board[6][6];
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            cin >> initial_board[r][c];
            if (initial_board[r][c] != 0) {
                positions[initial_board[r][c]].push_back({r, c});
            }
        }
    }

    for (auto const& [id, cells] : positions) {
        Vehicle v;
        v.id = id;
        v.len = cells.size();
        
        int min_r = 10, max_r = -1, min_c = 10, max_c = -1;
        for(auto const& cell : cells) {
            min_r = min(min_r, cell.first);
            max_r = max(max_r, cell.first);
            min_c = min(min_c, cell.second);
            max_c = max(max_c, cell.second);
        }
        v.r = min_r;
        v.c = min_c;
        if (min_r == max_r) v.is_horiz = true;
        else v.is_horiz = false;

        g_vehicle_props.push_back(v);
    }

    sort(g_vehicle_props.begin(), g_vehicle_props.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.id < b.id;
    });
    g_num_vehicles = g_vehicle_props.size();

    vector<int> initial_pos;
    for (const auto& v : g_vehicle_props) {
        initial_pos.push_back(v.is_horiz ? v.c : v.r);
    }
    long long initial_key = key_from_pos(initial_pos);

    queue<pair<long long, vector<Move>>> q;
    q.push({initial_key, {}});
    unordered_map<long long, bool> visited;
    visited[initial_key] = true;

    int best_dist = -1;
    vector<Move> best_path;

    auto start_time = chrono::high_resolution_clock::now();

    best_dist = solve(initial_pos);
    best_path = {};

    while (!q.empty()) {
        auto now = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = now - start_time;
        if (elapsed.count() > 1.95) {
            break;
        }

        long long current_key = q.front().first;
        vector<Move> current_path = q.front().second;
        q.pop();

        vector<int> current_pos = pos_from_key(current_key);
        
        int dist = solve(current_pos);
        if (dist > best_dist) {
            best_dist = dist;
            best_path = current_path;
        }

        int board[6][6];
        for(int r=0; r<6; ++r) for(int c=0; c<6; ++c) board[r][c] = 0;
        for (int i = 0; i < g_num_vehicles; ++i) {
            const auto& v = g_vehicle_props[i];
            if (v.is_horiz) {
                for (int k = 0; k < v.len; ++k) {
                    if (current_pos[i] + k < 6)
                        board[v.r][current_pos[i] + k] = v.id;
                }
            } else {
                for (int k = 0; k < v.len; ++k) {
                    if (current_pos[i] + k < 6)
                        board[current_pos[i] + k][v.c] = v.id;
                }
            }
        }
        
        for (int i = 0; i < g_num_vehicles; ++i) {
            const auto& v = g_vehicle_props[i];
            if (v.is_horiz) {
                // R
                int new_c = current_pos[i] + 1;
                bool possible = false;
                if (v.id == 1) { // red car
                    if (new_c <= 6) {
                        int front_c = new_c + v.len - 1;
                        if (front_c < 6) { if(board[v.r][front_c] == 0) possible = true; } 
                        else { possible = true; }
                    }
                } else {
                    if (new_c + v.len - 1 < 6) {
                        if (board[v.r][new_c + v.len - 1] == 0) possible = true;
                    }
                }
                if (possible) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_c;
                    long long next_key = key_from_pos(next_pos);
                    if (visited.find(next_key) == visited.end()) {
                        visited[next_key] = true;
                        vector<Move> next_path = current_path;
                        next_path.push_back({v.id, 'R'});
                        q.push({next_key, next_path});
                    }
                }
                // L
                int new_c_l = current_pos[i] - 1;
                if (new_c_l >= 0 && board[v.r][new_c_l] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_c_l;
                    long long next_key = key_from_pos(next_pos);
                    if (visited.find(next_key) == visited.end()) {
                        visited[next_key] = true;
                        vector<Move> next_path = current_path;
                        next_path.push_back({v.id, 'L'});
                        q.push({next_key, next_path});
                    }
                }
            } else { // vertical
                // D
                int new_r = current_pos[i] + 1;
                if (new_r + v.len - 1 < 6 && board[new_r + v.len - 1][v.c] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_r;
                    long long next_key = key_from_pos(next_pos);
                    if (visited.find(next_key) == visited.end()) {
                        visited[next_key] = true;
                        vector<Move> next_path = current_path;
                        next_path.push_back({v.id, 'D'});
                        q.push({next_key, next_path});
                    }
                }
                // U
                int new_r_u = current_pos[i] - 1;
                if (new_r_u >= 0 && board[new_r_u][v.c] == 0) {
                    vector<int> next_pos = current_pos;
                    next_pos[i] = new_r_u;
                    long long next_key = key_from_pos(next_pos);
                    if (visited.find(next_key) == visited.end()) {
                        visited[next_key] = true;
                        vector<Move> next_path = current_path;
                        next_path.push_back({v.id, 'U'});
                        q.push({next_key, next_path});
                    }
                }
            }
        }
    }

    cout << best_dist << " " << best_path.size() << endl;
    for (const auto& move : best_path) {
        cout << move.id << " " << move.dir << endl;
    }

    return 0;
}