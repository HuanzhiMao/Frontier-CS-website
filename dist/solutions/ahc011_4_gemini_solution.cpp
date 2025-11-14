#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>
#include <cmath>
#include <chrono>
#include <random>
#include <queue>

using namespace std;

// --- Timer and Random Number Generator ---
auto start_time = chrono::steady_clock::now();
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int N, T_max;
vector<vector<int>> initial_board;
pair<int, int> initial_empty_pos;

const int DR[] = {-1, 1, 0, 0}; // U, D
const int DC[] = {0, 0, -1, 1}; // L, R
const string MOVE_CHARS = "UDLR";

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

int evaluate(const vector<vector<int>>& board) {
    DSU dsu(N * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] == 0) continue;
            // Down
            if (i + 1 < N && board[i+1][j] != 0 && (board[i][j] & 8) && (board[i+1][j] & 2)) {
                dsu.unite(i * N + j, (i + 1) * N + j);
            }
            // Right
            if (j + 1 < N && board[i][j+1] != 0 && (board[i][j] & 4) && (board[i][j+1] & 1)) {
                dsu.unite(i * N + j, i * N + j + 1);
            }
        }
    }
    
    map<int, int> component_edges;
    map<int, int> component_vertices;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] == 0) continue;
            int root = dsu.find(i * N + j);
            component_vertices[root]++;
            if (i + 1 < N && board[i+1][j] != 0 && (board[i][j] & 8) && (board[i+1][j] & 2)) {
                component_edges[root]++;
            }
            if (j + 1 < N && board[i][j+1] != 0 && (board[i][j] & 4) && (board[i][j+1] & 1)) {
                component_edges[root]++;
            }
        }
    }
    
    int max_tree_size = 0;
    for (auto const& [root, v_count] : component_vertices) {
        int e_count = component_edges.count(root) ? component_edges[root] : 0;
        if (e_count == v_count - 1) {
            max_tree_size = max(max_tree_size, v_count);
        }
    }
    if (max_tree_size == 0 && (N*N-1) > 0) return 1;
    return max_tree_size;
}

vector<vector<int>> find_target_board() {
    vector<int> tiles_val;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (initial_board[i][j] != 0) {
                tiles_val.push_back(initial_board[i][j]);
            }
        }
    }
    shuffle(tiles_val.begin(), tiles_val.end(), rng);
    
    vector<vector<int>> current_board(N, vector<int>(N));
    vector<pair<int,int>> tile_locations;
    int k = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == N - 1 && j == N - 1) {
                current_board[i][j] = 0;
            } else {
                current_board[i][j] = tiles_val[k++];
                tile_locations.push_back({i,j});
            }
        }
    }
    
    vector<vector<int>> best_board = current_board;
    int current_score = evaluate(current_board);
    int max_score = current_score;

    double start_temp = 5.0;
    double end_temp = 0.01;
    double time_limit = (N >= 9) ? 1.8 : 0.8;

    double time_elapsed = 0;
    while (true) {
        time_elapsed = chrono::duration<double>(chrono::steady_clock::now() - start_time).count();
        if (time_elapsed > time_limit) break;
        if (max_score == N*N-1) break;
        
        double temp = start_temp * pow(end_temp / start_temp, time_elapsed / time_limit);
        
        uniform_int_distribution<int> dist(0, tile_locations.size() - 1);
        int idx1 = dist(rng);
        int idx2 = dist(rng);
        if (idx1 == idx2) continue;
        
        pair<int,int> p1 = tile_locations[idx1];
        pair<int,int> p2 = tile_locations[idx2];
        
        swap(current_board[p1.first][p1.second], current_board[p2.first][p2.second]);
        int new_score = evaluate(current_board);
        
        if (new_score > current_score || uniform_real_distribution<>(0.0, 1.0)(rng) < exp((new_score - current_score) / temp)) {
            current_score = new_score;
            if (current_score > max_score) {
                 max_score = current_score;
                 best_board = current_board;
            }
        } else {
            swap(current_board[p1.first][p1.second], current_board[p2.first][p2.second]);
        }
    }
    return best_board;
}

string move_empty_to(pair<int, int> start_pos, pair<int, int> target_pos, const vector<vector<bool>>& obstacles) {
    if (start_pos == target_pos) return "";
    queue<pair<pair<int, int>, string>> q;
    q.push({start_pos, ""});
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    visited[start_pos.first][start_pos.second] = true;
    while (!q.empty()) {
        auto [pos, path] = q.front(); q.pop();
        if (pos == target_pos) { return path; }
        for (int i = 0; i < 4; ++i) {
            int nr = pos.first + DR[i], nc = pos.second + DC[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !visited[nr][nc] && !obstacles[nr][nc]) {
                visited[nr][nc] = true;
                q.push({{nr, nc}, path + MOVE_CHARS[i]});
            }
        }
    }
    return "";
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    cin >> N >> T_max;
    initial_board.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        string row; cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_board[i][j] = (row[j] >= 'a') ? (row[j] - 'a' + 10) : (row[j] - '0');
            if (initial_board[i][j] == 0) initial_empty_pos = {i, j};
        }
    }

    vector<vector<int>> target_board = find_target_board();
    
    map<int, vector<pair<int, int>>> initial_pos_map, target_pos_map;
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j){
        if(initial_board[i][j] != 0) initial_pos_map[initial_board[i][j]].push_back({i,j});
        if(target_board[i][j] != 0) target_pos_map[target_board[i][j]].push_back({i,j});
    }

    map<pair<int, int>, pair<int, int>> initial_to_target;
    for (auto const& [type, target_positions] : target_pos_map) {
        vector<pair<int, int>> current_initial_pos = initial_pos_map[type];
        for (size_t i = 0; i < target_positions.size(); ++i) {
            initial_to_target[current_initial_pos[i]] = target_positions[i];
        }
    }
    map<pair<int, int>, pair<int, int>> target_to_initial;
    for(auto const&[i,t] : initial_to_target) target_to_initial[t] = i;

    vector<vector<int>> current_board = initial_board;
    vector<vector<pair<int,int>>> initial_pos_at(N, vector<pair<int,int>>(N));
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) initial_pos_at[i][j] = {i,j};

    pair<int, int> empty_pos = initial_empty_pos;
    string solution_moves = "";
    vector<vector<bool>> placed(N, vector<bool>(N, false));
    vector<pair<int,int>> placement_order;
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) for (int j = 0; j < N; ++j) placement_order.push_back({i, j});
        else for (int j = N - 1; j >= 0; --j) placement_order.push_back({i, j});
    }

    for (const auto& target_pos : placement_order) {
        if (target_board[target_pos.first][target_pos.second] == 0) continue;

        pair<int, int> tile_initial_pos = target_to_initial[target_pos];
        pair<int, int> tile_current_pos = {-1, -1};
        for(int r=0; r<N; ++r) for(int c=0; c<N; ++c) {
            if(initial_pos_at[r][c] == tile_initial_pos){
                tile_current_pos = {r,c};
                break;
            }
        }
        
        if (tile_current_pos == target_pos) {
            placed[target_pos.first][target_pos.second] = true;
            continue;
        }
        
        vector<vector<bool>> tile_path_obstacles = placed;
        tile_path_obstacles[empty_pos.first][empty_pos.second] = true;

        queue<pair<pair<int, int>, vector<pair<int, int>>>> tile_q;
        tile_q.push({tile_current_pos, {tile_current_pos}});
        map<pair<int,int>, bool> tile_visited;
        tile_visited[tile_current_pos] = true;
        vector<pair<int,int>> tile_path;
        
        while(!tile_q.empty()){
            auto [pos, path] = tile_q.front(); tile_q.pop();
            if(pos == target_pos){
                tile_path = path;
                break;
            }
            for(int i=0; i<4; ++i){
                int nr = pos.first + DR[i], nc = pos.second + DC[i];
                if(nr >= 0 && nr < N && nc >= 0 && nc < N && !tile_visited[{nr,nc}] && !placed[nr][nc]){
                    tile_visited[{nr,nc}] = true;
                    auto new_path = path;
                    new_path.push_back({nr,nc});
                    tile_q.push({{nr,nc}, new_path});
                }
            }
        }

        for(size_t i=0; i < tile_path.size()-1; ++i){
            pair<int, int> p1 = tile_current_pos;
            pair<int, int> p2 = tile_path[i+1];
            
            vector<vector<bool>> obstacles = placed;
            obstacles[p1.first][p1.second] = true;
            string empty_path_moves = move_empty_to(empty_pos, p2, obstacles);

            for(char move : empty_path_moves){
                solution_moves += move;
                int move_idx = string(MOVE_CHARS).find(move);
                pair<int,int> tile_to_move_pos = {empty_pos.first + DR[move_idx], empty_pos.second + DC[move_idx]};
                swap(current_board[empty_pos.first][empty_pos.second], current_board[tile_to_move_pos.first][tile_to_move_pos.second]);
                swap(initial_pos_at[empty_pos.first][empty_pos.second], initial_pos_at[tile_to_move_pos.first][tile_to_move_pos.second]);
                empty_pos = tile_to_move_pos;
            }
            
            int dr = p1.first - empty_pos.first, dc = p1.second - empty_pos.second;
            char move_char = ' ';
            if(dr == -1 && dc == 0) move_char='U'; if(dr == 1 && dc == 0) move_char='D';
            if(dr == 0 && dc == -1) move_char='L'; if(dr == 0 && dc == 1) move_char='R';
            solution_moves += move_char;

            swap(current_board[p1.first][p1.second], current_board[empty_pos.first][empty_pos.second]);
            swap(initial_pos_at[p1.first][p1.second], initial_pos_at[empty_pos.first][empty_pos.second]);
            empty_pos = p1;
            tile_current_pos = p2;
        }
        placed[target_pos.first][target_pos.second] = true;
    }

    cout << solution_moves << endl;

    return 0;
}