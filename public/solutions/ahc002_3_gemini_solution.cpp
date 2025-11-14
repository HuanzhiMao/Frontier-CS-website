#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <tuple>

using namespace std;

const int N = 50;

int si, sj;
int t[N][N];
int p[N][N];
int num_tiles = 0;
vector<long long> tile_scores;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_char[] = {'U', 'D', 'L', 'R'};

struct State {
    int r, c;
    long long score;
    vector<bool> forbidden_tile;
    vector<vector<bool>> visited_square;
};

// Re-evaluates a path from the start and returns the final state
State evaluate_path(const string& path) {
    State s;
    s.r = si;
    s.c = sj;
    s.score = p[si][sj];
    s.forbidden_tile.assign(num_tiles, false);
    s.visited_square.assign(N, vector<bool>(N, false));
    s.visited_square[si][sj] = true;

    for (char move : path) {
        int move_idx = -1;
        if (move == 'U') move_idx = 0;
        else if (move == 'D') move_idx = 1;
        else if (move == 'L') move_idx = 2;
        else if (move == 'R') move_idx = 3;

        int prev_r = s.r;
        int prev_c = s.c;
        s.r += dr[move_idx];
        s.c += dc[move_idx];

        if (s.r < 0 || s.r >= N || s.c < 0 || s.c >= N) {
            s.score = -1; // Invalid path
            return s;
        }

        if (s.visited_square[s.r][s.c]) {
            s.score = -1; // Invalid path
            return s;
        }

        int prev_tid = t[prev_r][prev_c];
        int next_tid = t[s.r][s.c];

        if (prev_tid != next_tid) {
            if (s.forbidden_tile[next_tid]) {
                s.score = -1; // Invalid path
                return s;
            }
            s.forbidden_tile[prev_tid] = true;
        }

        s.visited_square[s.r][s.c] = true;
        s.score += p[s.r][s.c];
    }
    return s;
}


// Grows a path from a given state using a randomized greedy algorithm
pair<string, long long> grow_path_from_state(State start_state, mt19937& rng) {
    string path_suffix = "";
    long long score_suffix = 0;
    
    State current_state = start_state;

    for (int step = 0; step < 2 * N * N; ++step) {
        int current_tid = t[current_state.r][current_state.c];
        
        vector<tuple<double, int, int, int>> candidates;
        for (int i = 0; i < 4; ++i) {
            int nr = current_state.r + dr[i];
            int nc = current_state.c + dc[i];

            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (current_state.visited_square[nr][nc]) continue;

            int next_tid = t[nr][nc];
            double attractiveness = 0;

            if (current_tid == next_tid) {
                attractiveness = p[nr][nc] + 100.0; // Boost internal moves
            } else {
                if (current_state.forbidden_tile[next_tid]) continue;
                attractiveness = tile_scores[next_tid];
            }
            candidates.emplace_back(attractiveness, i, nr, nc);
        }

        if (candidates.empty()) {
            break;
        }

        sort(candidates.rbegin(), candidates.rend());
        
        int choice_idx = 0;
        if (candidates.size() > 1 && uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.1) {
            choice_idx = 1;
        }

        auto& chosen_move = candidates[choice_idx];
        int move_idx = get<1>(chosen_move);
        int nr = get<2>(chosen_move);
        int nc = get<3>(chosen_move);

        path_suffix += move_char[move_idx];
        score_suffix += p[nr][nc];
        
        int prev_tid = t[current_state.r][current_state.c];
        current_state.r = nr;
        current_state.c = nc;
        
        if (prev_tid != t[current_state.r][current_state.c]) {
            current_state.forbidden_tile[prev_tid] = true;
        }
        current_state.visited_square[current_state.r][current_state.c] = true;
    }

    return {path_suffix, score_suffix};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    cin >> si >> sj;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> t[i][j];
            num_tiles = max(num_tiles, t[i][j]);
        }
    }
    num_tiles++;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> p[i][j];
        }
    }

    tile_scores.resize(num_tiles, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            tile_scores[t[i][j]] += p[i][j];
        }
    }

    State initial_state;
    initial_state.r = si;
    initial_state.c = sj;
    initial_state.score = p[si][sj];
    initial_state.forbidden_tile.assign(num_tiles, false);
    initial_state.visited_square.assign(N, vector<bool>(N, false));
    initial_state.visited_square[si][sj] = true;
    
    auto initial_path_info = grow_path_from_state(initial_state, rng);
    string best_path = initial_path_info.first;
    long long best_score = initial_state.score + initial_path_info.second;
    
    string current_path = best_path;
    long long current_score = best_score;

    double time_limit = 1.95;
    double temp_start = 100;
    double temp_end = 0.1;

    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
        if (elapsed > time_limit) break;

        double temp = temp_start + (temp_end - temp_start) * elapsed / time_limit;
        
        string path_to_modify = current_path;
        if (path_to_modify.empty() && uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.5) {
            path_to_modify = best_path;
        }
        
        if (path_to_modify.empty()) {
             auto new_path_info = grow_path_from_state(initial_state, rng);
             current_path = new_path_info.first;
             current_score = initial_state.score + new_path_info.second;
             if (current_score > best_score) {
                 best_path = current_path;
                 best_score = current_score;
             }
             continue;
        }

        uniform_int_distribution<int> dist(0, path_to_modify.length());
        int truncate_len = dist(rng);

        string prefix_path = path_to_modify.substr(0, truncate_len);
        
        State prefix_state = evaluate_path(prefix_path);
        
        auto suffix_info = grow_path_from_state(prefix_state, rng);
        string new_path = prefix_path + suffix_info.first;
        long long new_score = prefix_state.score + suffix_info.second;

        if (new_score > current_score || 
            (temp > 0 && exp((double)(new_score - current_score) / temp) > uniform_real_distribution<double>(0.0, 1.0)(rng))) {
            current_path = new_path;
            current_score = new_score;
        }
        
        if (new_score > best_score) {
            best_path = new_path;
            best_score = new_score;
        }
    }

    cout << best_path << endl;

    return 0;
}