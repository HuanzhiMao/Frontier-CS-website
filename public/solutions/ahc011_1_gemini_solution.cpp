#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <queue>

using namespace std;

// --- Timer ---
auto start_time = chrono::steady_clock::now();
double time_limit = 2800;
bool is_time_over() {
    auto now = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > time_limit;
}

// --- Random Number Generator ---
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// --- Globals ---
int N, T;
const int DR[] = {-1, 1, 0, 0}; // U, D, L, R
const int DC[] = {0, 0, -1, 1};
const char MOVE_CHARS[] = {'U', 'D', 'L', 'R'};

struct Tile {
    int id;
    int type;
    int r, c;
};

struct DSU {
    vector<int> parent;
    vector<int> sz;
    vector<int> edges;
    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        edges.assign(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            edges[root_i] += edges[root_j];
        }
        edges[root_i]++;
    }
};

bool has_up(int type) { return (type >> 1) & 1; }
bool has_down(int type) { return (type >> 3) & 1; }
bool has_left(int type) { return (type >> 0) & 1; }
bool has_right(int type) { return (type >> 2) & 1; }

int get_largest_tree_size(int n, const vector<vector<int>>& board) {
    if (n * n - 1 == 0) return 0;
    DSU dsu(n * n);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (board[r][c] == 0) continue;
            if (r + 1 < n && board[r + 1][c] != 0 && has_down(board[r][c]) && has_up(board[r + 1][c])) {
                dsu.unite(r * n + c, (r + 1) * n + c);
            }
            if (c + 1 < n && board[r][c + 1] != 0 && has_right(board[r][c]) && has_left(board[r][c + 1])) {
                dsu.unite(r * n + c, r * n + c + 1);
            }
        }
    }

    int max_tree_size = 0;
    vector<bool> visited_roots(n * n, false);
    for (int i = 0; i < n * n; ++i) {
        if (board[i / n][i % n] == 0) continue;
        int root = dsu.find(i);
        if (!visited_roots[root]) {
            visited_roots[root] = true;
            if (dsu.edges[root] == dsu.sz[root] - 1) {
                max_tree_size = max(max_tree_size, dsu.sz[root]);
            }
        }
    }
    return max_tree_size;
}

int get_max_component_size(int n, const vector<vector<int>>& board) {
    if (n * n - 1 == 0) return 0;
    DSU dsu(n * n);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (board[r][c] == 0) continue;
            if (r + 1 < n && board[r + 1][c] != 0 && has_down(board[r][c]) && has_up(board[r + 1][c])) {
                dsu.unite(r * n + c, (r + 1) * n + c);
            }
            if (c + 1 < n && board[r][c + 1] != 0 && has_right(board[r][c]) && has_left(board[r][c + 1])) {
                dsu.unite(r * n + c, r * n + c + 1);
            }
        }
    }

    int max_sz = 0;
    for (int i = 0; i < n * n; ++i) {
        if (board[i / n][i % n] == 0) continue;
        if (dsu.parent[i] == i) {
            max_sz = max(max_sz, dsu.sz[i]);
        }
    }
    return max_sz;
}

string solve_puzzle(const vector<vector<int>>& initial_board, const vector<Tile>& initial_tiles, const vector<pair<int, int>>& target_pos) {
    vector<Tile> current_tiles = initial_tiles;
    vector<int> tile_at_pos(N * N);
    int empty_r, empty_c;

    for (const auto& tile : current_tiles) {
        tile_at_pos[tile.r * N + tile.c] = tile.id;
    }
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (initial_board[r][c] == 0) {
                empty_r = r;
                empty_c = c;
                tile_at_pos[r * N + c] = -1;
            }
        }
    }

    auto move_empty = [&](int tr, int tc, int avoid_r, int avoid_c) {
        string path = "";
        queue<pair<pair<int, int>, string>> q;
        q.push({{empty_r, empty_c}, ""});
        vector<vector<bool>> visited(N, vector<bool>(N, false));
        visited[empty_r][empty_c] = true;

        while (!q.empty()) {
            auto curr = q.front();
            q.pop();
            int r = curr.first.first;
            int c = curr.first.second;
            string p = curr.second;

            if (r == tr && c == tc) {
                path = p;
                break;
            }

            for (int i = 0; i < 4; ++i) {
                int nr = r + DR[i];
                int nc = c + DC[i];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N && !(nr == avoid_r && nc == avoid_c) && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    q.push({{nr, nc}, p + MOVE_CHARS[i]});
                }
            }
        }
        return path;
    };

    string solution = "";
    vector<int> p(N * N - 1);
    for (int i = 0; i < N * N - 1; ++i) p[i] = i;

    for (int iter = 0; iter < 2; ++iter) {
        for (int tile_id : p) {
            if (solution.length() >= T) break;

            int tr = target_pos[tile_id].first;
            int tc = target_pos[tile_id].second;
            
            while (current_tiles[tile_id].r != tr || current_tiles[tile_id].c != tc) {
                if (solution.length() >= T) break;
                
                int& r = current_tiles[tile_id].r;
                int& c = current_tiles[tile_id].c;

                int dr = (tr > r) ? 1 : (tr < r) ? -1 : 0;
                int dc = (tc > c) ? 1 : (tc < c) ? -1 : 0;
                
                int move_dir = -1;
                if(dr != 0 && empty_r == r + dr && empty_c == c) {
                     move_dir = (dr == 1) ? 1 : 0; // D or U
                } else if (dc != 0 && empty_c == c + dc && empty_r == r){
                     move_dir = (dc == 1) ? 3 : 2; // R or L
                }

                if (move_dir != -1) {
                    solution += MOVE_CHARS[move_dir];
                    swap(tile_at_pos[r * N + c], tile_at_pos[empty_r * N + empty_c]);
                    empty_r = r; empty_c = c;
                    r += DR[move_dir]; c += DC[move_dir];
                } else {
                    int empty_tr = -1, empty_tc = -1;
                    if(dr != 0) { empty_tr = r + dr; empty_tc = c; }
                    else if (dc != 0) { empty_tr = r; empty_tc = c + dc; }

                    string path = move_empty(empty_tr, empty_tc, r, c);
                    if (path.length() + solution.length() > T) {
                        path = path.substr(0, T - solution.length());
                    }
                    solution += path;
                    for(char m : path){
                        if(m == 'U'){ empty_r--; }
                        else if(m == 'D'){ empty_r++; }
                        else if(m == 'L'){ empty_c--; }
                        else if(m == 'R'){ empty_c++; }
                    }
                }
            }
        }
    }

    return solution;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> T;
    vector<vector<int>> initial_board(N, vector<int>(N));
    vector<Tile> initial_tiles;
    int tile_id_counter = 0;
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) {
            char c = row[j];
            if (c >= '0' && c <= '9') initial_board[i][j] = c - '0';
            else initial_board[i][j] = c - 'a' + 10;
            if (initial_board[i][j] != 0) {
                initial_tiles.push_back({tile_id_counter++, initial_board[i][j], i, j});
            }
        }
    }

    vector<pair<int, int>> initial_pos(N * N - 1);
    for (const auto& tile : initial_tiles) {
        initial_pos[tile.id] = {tile.r, tile.c};
    }

    vector<int> current_perm(N * N - 1);
    for (int i = 0; i < N * N - 1; ++i) current_perm[i] = i;

    vector<pair<int, int>> grid_pos;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (r == N - 1 && c == N - 1) continue;
            grid_pos.push_back({r, c});
        }
    }

    auto calculate_energy = [&](const vector<int>& perm) {
        vector<vector<int>> board(N, vector<int>(N, 0));
        long long manhattan_dist = 0;
        for (int i = 0; i < N * N - 1; ++i) {
            int tile_id = perm[i];
            int r = grid_pos[i].first;
            int c = grid_pos[i].second;
            board[r][c] = initial_tiles[tile_id].type;
            manhattan_dist += abs(r - initial_pos[tile_id].first) + abs(c - initial_pos[tile_id].second);
        }

        if (get_largest_tree_size(N, board) == N * N - 1) {
            return manhattan_dist;
        }

        return (long long)(N * N - 1 - get_max_component_size(N, board)) * 1000000 + manhattan_dist;
    };

    long long current_energy = calculate_energy(current_perm);
    vector<int> best_perm = current_perm;
    long long best_energy = current_energy;

    double start_temp = 5000;
    double end_temp = 0.1;

    while (!is_time_over()) {
        int i = rng() % (N * N - 1);
        int j = rng() % (N * N - 1);
        if (i == j) continue;

        swap(current_perm[i], current_perm[j]);
        long long new_energy = calculate_energy(current_perm);

        double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count();
        double temp = start_temp + (end_temp - start_temp) * elapsed_ms / time_limit;
        
        if (new_energy < current_energy || (double)rng() / rng.max() < exp((double)(current_energy - new_energy) / temp)) {
            current_energy = new_energy;
            if (current_energy < best_energy) {
                best_energy = current_energy;
                best_perm = current_perm;
            }
        } else {
            swap(current_perm[i], current_perm[j]);
        }
    }

    vector<pair<int, int>> target_pos(N * N - 1);
    for (int i = 0; i < N * N - 1; ++i) {
        target_pos[best_perm[i]] = grid_pos[i];
    }

    string solution = solve_puzzle(initial_board, initial_tiles, target_pos);
    cout << solution << endl;

    return 0;
}