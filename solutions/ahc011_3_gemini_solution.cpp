#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <map>
#include <random>

using namespace std;

auto start_time = chrono::steady_clock::now();

const int DR[] = {-1, 1, 0, 0};
const int DC[] = {0, 0, -1, 1};
const char MOVE_CHARS[] = {'U', 'D', 'L', 'R'};
const int OPPOSITE_MOVE[] = {1, 0, 3, 2};

int N, T;

struct Board {
    vector<vector<int>> tiles;
    int empty_r, empty_c;

    Board() {
        tiles.resize(N, vector<int>(N));
    }

    void find_empty() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (tiles[i][j] == 0) {
                    empty_r = i;
                    empty_c = j;
                    return;
                }
            }
        }
    }
};

struct DSU {
    vector<int> parent;
    vector<int> num_nodes;
    vector<int> num_edges;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        num_nodes.assign(n, 1);
        num_edges.assign(n, 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (num_nodes[root_i] < num_nodes[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            num_nodes[root_i] += num_nodes[root_j];
            num_edges[root_i] += num_edges[root_j];
        }
        num_edges[root_i]++;
    }
};

int calculate_score(const Board& board) {
    DSU dsu(N * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board.tiles[i][j] == 0) continue;
            if (i + 1 < N && board.tiles[i+1][j] != 0 && (board.tiles[i][j] & 8) && (board.tiles[i+1][j] & 2)) {
                dsu.unite(i * N + j, (i + 1) * N + j);
            }
            if (j + 1 < N && board.tiles[i][j+1] != 0 && (board.tiles[i][j] & 4) && (board.tiles[i][j+1] & 1)) {
                dsu.unite(i * N + j, i * N + j + 1);
            }
        }
    }

    int max_tree_size = 0;
    vector<bool> visited_roots(N*N, false);
    for (int i = 0; i < N * N; ++i) {
        if (board.tiles[i/N][i%N] != 0) {
            int root = dsu.find(i);
            if (!visited_roots[root]) {
                if (dsu.num_edges[root] == dsu.num_nodes[root] - 1) {
                    max_tree_size = max(max_tree_size, dsu.num_nodes[root]);
                }
                visited_roots[root] = true;
            }
        }
    }
    return max_tree_size;
}

struct State {
    Board board;
    string path;
    int score;
    int last_move_dir = -1;

    bool operator<(const State& other) const {
        if (score != other.score) {
            return score < other.score;
        }
        return path.length() > other.path.length();
    }
};

int hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> T;
    Board initial_board;
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            initial_board.tiles[i][j] = hex_to_int(row[j]);
        }
    }
    initial_board.find_empty();

    string best_path = "";
    int max_score = 0;
    int best_full_tree_len = T + 1;

    State initial_state;
    initial_state.board = initial_board;
    initial_state.score = calculate_score(initial_board);
    max_score = initial_state.score;
    if(max_score == N*N - 1) {
        best_full_tree_len = 0;
        best_path = "";
    }

    const int BEAM_WIDTH = 150;
    vector<State> beam;
    beam.push_back(initial_state);
    
    map<vector<vector<int>>, int> visited;
    visited[initial_board.tiles] = 0;

    int time_limit_ms = 1950;
    
    for (int k = 0; k < T; ++k) {
        auto current_time = chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > time_limit_ms) {
            break;
        }

        map<vector<vector<int>>, State> next_states_map;
        for (const auto& s : beam) {
            for (int dir = 0; dir < 4; ++dir) {
                if (s.last_move_dir != -1 && dir == OPPOSITE_MOVE[s.last_move_dir]) continue;

                int tile_r = s.board.empty_r + DR[dir];
                int tile_c = s.board.empty_c + DC[dir];

                if (tile_r >= 0 && tile_r < N && tile_c >= 0 && tile_c < N) {
                    State next_s;
                    next_s.board = s.board;
                    swap(next_s.board.tiles[s.board.empty_r][s.board.empty_c], next_s.board.tiles[tile_r][tile_c]);
                    next_s.board.empty_r = tile_r;
                    next_s.board.empty_c = tile_c;
                    next_s.path = s.path + MOVE_CHARS[dir];
                    next_s.last_move_dir = dir;

                    if (!next_states_map.count(next_s.board.tiles) || next_states_map[next_s.board.tiles].path.length() > next_s.path.length()) {
                        next_states_map[next_s.board.tiles] = next_s;
                    }
                }
            }
        }
        
        vector<State> next_beam;
        for (auto const& [key, val] : next_states_map) {
             if (!visited.count(key) || visited[key] > val.path.length()) {
                 next_beam.push_back(val);
             }
        }
        
        if (next_beam.empty()) break;
        
        for (auto& s : next_beam) {
            s.score = calculate_score(s.board);
        }

        sort(next_beam.rbegin(), next_beam.rend());
        if (next_beam.size() > BEAM_WIDTH) {
            next_beam.resize(BEAM_WIDTH);
        }
        
        beam = next_beam;
        for(auto const& s : beam) {
            visited[s.board.tiles] = s.path.length();
            if (s.score > max_score) {
                max_score = s.score;
                best_path = s.path;
            }
            if (s.score == N * N - 1) {
                 if (s.path.length() < best_full_tree_len) {
                    best_full_tree_len = s.path.length();
                    best_path = s.path;
                }
            }
        }
    }

    cout << best_path << endl;

    return 0;
}