#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

// --- Timer ---
auto start_time = chrono::high_resolution_clock::now();
double time_limit = 1.95;
bool is_time_up() {
    auto current_time = chrono::high_resolution_clock::now();
    return chrono::duration<double>(current_time - start_time).count() > time_limit;
}

// --- Random Number Generator ---
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// --- Problem Constants and Globals ---
const int N = 50;
int si, sj;
int t[N][N];
int p[N][N];
const int di[] = {-1, 1, 0, 0};
const int dj[] = {0, 0, -1, 1};

struct Tile {
    int id;
    vector<pair<int, int>> squares;
    long long total_score = 0;
    vector<int> adj_tiles;
};

vector<Tile> tiles;

// --- Helper Functions ---
char get_move_char(int r1, int c1, int r2, int c2) {
    if (r2 == r1 - 1) return 'U';
    if (r2 == r1 + 1) return 'D';
    if (c2 == c1 - 1) return 'L';
    if (c2 == c1 + 1) return 'R';
    return '?'; // Should not happen
}

// --- Main Logic ---
void preprocess() {
    map<int, vector<pair<int, int>>> tile_squares;
    int max_tile_id = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            tile_squares[t[i][j]].push_back({i, j});
            max_tile_id = max(max_tile_id, t[i][j]);
        }
    }
    tiles.resize(max_tile_id + 1);

    for (auto const& [id, squares_vec] : tile_squares) {
        tiles[id].id = id;
        tiles[id].squares = squares_vec;
        long long score = 0;
        for (auto const& sq : squares_vec) {
            score += p[sq.first][sq.second];
        }
        tiles[id].total_score = score;
    }

    for (int i = 0; i <= max_tile_id; ++i) {
        if (tiles[i].squares.empty()) continue;
        set<int> neighbors;
        for (auto const& sq : tiles[i].squares) {
            for (int k = 0; k < 4; ++k) {
                int ni = sq.first + di[k];
                int nj = sq.second + dj[k];
                if (ni >= 0 && ni < N && nj >= 0 && nj < N && t[ni][nj] != i) {
                    neighbors.insert(t[ni][nj]);
                }
            }
        }
        tiles[i].adj_tiles.assign(neighbors.begin(), neighbors.end());
    }
}

void extend_greedily(vector<int>& path, long long& score, set<int>& visited) {
    while (true) {
        int last_tile_id = path.back();
        int best_next_tile_id = -1;
        long long max_score_eval = -1;

        for (int neighbor_tile_id : tiles[last_tile_id].adj_tiles) {
            if (visited.find(neighbor_tile_id) == visited.end()) {
                long long score_eval = tiles[neighbor_tile_id].total_score;
                if (score_eval > max_score_eval) {
                    max_score_eval = score_eval;
                    best_next_tile_id = neighbor_tile_id;
                }
            }
        }

        if (best_next_tile_id == -1) break;

        path.push_back(best_next_tile_id);
        visited.insert(best_next_tile_id);
        score += tiles[best_next_tile_id].total_score;
    }
}

string generate_move_string(const vector<int>& tile_path) {
    if (tile_path.empty() || tile_path.size() == 1) return "";
    
    string moves = "";
    pair<int, int> current_pos = {si, sj};

    for (size_t i = 0; i < tile_path.size() - 1; ++i) {
        int current_tile_id = tile_path[i];
        int next_tile_id = tile_path[i+1];

        pair<int, int> exit_sq = {-1, -1}, entry_sq = {-1, -1};

        // Try to find a direct connection (cost 1 move)
        for (int k = 0; k < 4; ++k) {
            int ni = current_pos.first + di[k];
            int nj = current_pos.second + dj[k];
            if (ni >= 0 && ni < N && nj >= 0 && nj < N && t[ni][nj] == next_tile_id) {
                exit_sq = current_pos;
                entry_sq = {ni, nj};
                break;
            }
        }

        // If no direct connection, find any connection (cost 2 moves)
        if (exit_sq.first == -1) {
            for (const auto& cur_sq : tiles[current_tile_id].squares) {
                if (cur_sq == current_pos) continue;
                for (int k = 0; k < 4; ++k) {
                    int ni = cur_sq.first + di[k];
                    int nj = cur_sq.second + dj[k];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < N && t[ni][nj] == next_tile_id) {
                        exit_sq = cur_sq;
                        entry_sq = {ni, nj};
                        break;
                    }
                }
                if (exit_sq.first != -1) break;
            }
        }
        
        // Append moves
        if (current_pos != exit_sq) {
            moves += get_move_char(current_pos.first, current_pos.second, exit_sq.first, exit_sq.second);
        }
        moves += get_move_char(exit_sq.first, exit_sq.second, entry_sq.first, entry_sq.second);

        current_pos = entry_sq;
    }
    return moves;
}

void solve() {
    cin >> si >> sj;
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) cin >> t[i][j];
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) cin >> p[i][j];

    preprocess();

    // Initial greedy solution
    vector<int> best_tile_path;
    long long best_score = 0;
    {
        vector<int> path;
        long long score = 0;
        set<int> visited;
        int start_tile_id = t[si][sj];
        
        path.push_back(start_tile_id);
        visited.insert(start_tile_id);
        score += tiles[start_tile_id].total_score;
        
        extend_greedily(path, score, visited);
        
        best_tile_path = path;
        best_score = score;
    }

    vector<int> current_tile_path = best_tile_path;
    long long current_score = best_score;

    double start_temp = 5000;
    double end_temp = 10;
    
    while (!is_time_up()) {
        if (current_tile_path.size() <= 1) continue;

        // Perturbation: Truncate and regrow
        int k = uniform_int_distribution<int>(1, current_tile_path.size() - 1)(rng);
        
        vector<int> prefix_path(current_tile_path.begin(), current_tile_path.begin() + k);
        set<int> visited;
        long long prefix_score = 0;
        for (int tile_id : prefix_path) {
            visited.insert(tile_id);
            prefix_score += tiles[tile_id].total_score;
        }

        int last_tile_id = prefix_path.back();
        vector<int> candidates;
        for (int neighbor : tiles[last_tile_id].adj_tiles) {
            if (visited.find(neighbor) == visited.end()) {
                candidates.push_back(neighbor);
            }
        }

        if (candidates.empty()) continue;

        int next_tile_id = candidates[uniform_int_distribution<int>(0, candidates.size() - 1)(rng)];
        
        vector<int> new_path = prefix_path;
        long long new_score = prefix_score;
        
        new_path.push_back(next_tile_id);
        visited.insert(next_tile_id);
        new_score += tiles[next_tile_id].total_score;
        
        extend_greedily(new_path, new_score, visited);

        // SA decision
        double temp = start_temp + (end_temp - start_temp) * (chrono::duration<double>(chrono::high_resolution_clock::now() - start_time).count() / time_limit);
        
        if (new_score > current_score) {
            current_score = new_score;
            current_tile_path = new_path;
        } else {
             if (temp > 1e-9) {
                double probability = exp((double)(new_score - current_score) / temp);
                if (uniform_real_distribution<double>(0.0, 1.0)(rng) < probability) {
                    current_score = new_score;
                    current_tile_path = new_path;
                }
            }
        }
        
        if (current_score > best_score) {
            best_score = current_score;
            best_tile_path = current_tile_path;
        }
    }

    cout << generate_move_string(best_tile_path) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}