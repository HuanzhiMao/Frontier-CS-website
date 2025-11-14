#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <array>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

// Constants
constexpr int N_CONST = 20;

// Utilities
struct Xorshift {
    unsigned int x, y, z, w;
    Xorshift() {
        x = 123456789;
        y = 362436069;
        z = 521288629;
        w = 88675123;
        // Use a time-based seed for submissions
        auto now = chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
        w = (unsigned int)millis;
    }
    unsigned int next_int() {
        unsigned int t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    int next_int(int min_val, int max_val) {
        if (min_val > max_val) return min_val;
        return min_val + next_int() % (max_val - min_val + 1);
    }
    double next_double() {
        return (double)next_int() / 0xFFFFFFFF;
    }
};

Xorshift rnd;

struct Placement {
    int r, c, dir; // 0 for horizontal, 1 for vertical
};

int char_to_int(char ch) {
    return ch - 'A';
}

int calculate_cell_conflicts(const array<int, 8>& counts) {
    int total = 0;
    int max_val = 0;
    for (int count : counts) {
        total += count;
        if (count > max_val) {
            max_val = count;
        }
    }
    return total - max_val;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    int N_val, M;
    cin >> N_val >> M;
    vector<string> S(M);
    for (int i = 0; i < M; ++i) {
        cin >> S[i];
    }

    vector<Placement> current_placements(M);
    vector<vector<array<int, 8>>> counts(N_CONST, vector<array<int, 8>>(N_CONST, {0}));
    
    for (int i = 0; i < M; ++i) {
        current_placements[i] = {rnd.next_int(0, N_CONST - 1), rnd.next_int(0, N_CONST - 1), rnd.next_int(0, 1)};
    }

    for (int i = 0; i < M; ++i) {
        const auto& p = current_placements[i];
        const string& s = S[i];
        if (p.dir == 0) {
            for (size_t k = 0; k < s.length(); ++k) {
                counts[p.r][(p.c + k) % N_CONST][char_to_int(s[k])]++;
            }
        } else {
            for (size_t k = 0; k < s.length(); ++k) {
                counts[(p.r + k) % N_CONST][p.c][char_to_int(s[k])]++;
            }
        }
    }

    long long current_conflicts = 0;
    for (int r = 0; r < N_CONST; ++r) {
        for (int c = 0; c < N_CONST; ++c) {
            current_conflicts += calculate_cell_conflicts(counts[r][c]);
        }
    }

    vector<Placement> best_placements = current_placements;
    long long best_conflicts = current_conflicts;

    double start_temp = 5.0;
    double end_temp = 0.01;
    double time_limit = 1.95;

    vector<pair<int, int>> affected_cells_vec;
    bool visited[N_CONST][N_CONST];
    for (int r = 0; r < N_CONST; ++r) for (int c = 0; c < N_CONST; ++c) visited[r][c] = false;

    int iter = 0;
    while (true) {
        iter++;
        if (iter % 256 == 0) {
            auto now = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration_cast<chrono::microseconds>(now - start_time).count() / 1e6;
            if (elapsed > time_limit) break;
            if (best_conflicts == 0) break;
        }

        auto now = chrono::high_resolution_clock::now();
        double progress = chrono::duration_cast<chrono::microseconds>(now - start_time).count() / (time_limit * 1e6);
        if (progress >= 1.0) break;
        double temp = start_temp * pow(end_temp / start_temp, progress);

        int str_idx = rnd.next_int(0, M - 1);
        Placement old_p = current_placements[str_idx];
        Placement new_p;

        if (rnd.next_double() < 0.15) {
             new_p = {rnd.next_int(0, N_CONST - 1), rnd.next_int(0, N_CONST - 1), rnd.next_int(0, 1)};
        } else {
            new_p = old_p;
            int move_type = rnd.next_int(0, 4);
            if (move_type == 0) new_p.r = (new_p.r + 1) % N_CONST;
            else if (move_type == 1) new_p.r = (new_p.r - 1 + N_CONST) % N_CONST;
            else if (move_type == 2) new_p.c = (new_p.c + 1) % N_CONST;
            else if (move_type == 3) new_p.c = (new_p.c - 1 + N_CONST) % N_CONST;
            else new_p.dir = 1 - new_p.dir;
        }

        if(old_p.r == new_p.r && old_p.c == new_p.c && old_p.dir == new_p.dir) continue;

        const string& s = S[str_idx];
        
        affected_cells_vec.clear();
        
        if (old_p.dir == 0) { for (size_t k = 0; k < s.length(); ++k) { int r = old_p.r, c = (old_p.c + k) % N_CONST; if (!visited[r][c]) { visited[r][c] = true; affected_cells_vec.push_back({r, c}); }}}
        else { for (size_t k = 0; k < s.length(); ++k) { int r = (old_p.r + k) % N_CONST, c = old_p.c; if (!visited[r][c]) { visited[r][c] = true; affected_cells_vec.push_back({r, c}); }}}
        if (new_p.dir == 0) { for (size_t k = 0; k < s.length(); ++k) { int r = new_p.r, c = (new_p.c + k) % N_CONST; if (!visited[r][c]) { visited[r][c] = true; affected_cells_vec.push_back({r, c}); }}}
        else { for (size_t k = 0; k < s.length(); ++k) { int r = (new_p.r + k) % N_CONST, c = new_p.c; if (!visited[r][c]) { visited[r][c] = true; affected_cells_vec.push_back({r, c}); }}}
        
        long long old_local_conflicts = 0;
        for (const auto& cell : affected_cells_vec) old_local_conflicts += calculate_cell_conflicts(counts[cell.first][cell.second]);

        if (old_p.dir == 0) { for (size_t k=0; k<s.length(); ++k) counts[old_p.r][(old_p.c+k)%N_CONST][char_to_int(s[k])]--; } 
        else { for (size_t k=0; k<s.length(); ++k) counts[(old_p.r+k)%N_CONST][old_p.c][char_to_int(s[k])]--; }
        if (new_p.dir == 0) { for (size_t k=0; k<s.length(); ++k) counts[new_p.r][(new_p.c+k)%N_CONST][char_to_int(s[k])]++; } 
        else { for (size_t k=0; k<s.length(); ++k) counts[(new_p.r+k)%N_CONST][new_p.c][char_to_int(s[k])]++; }

        long long new_local_conflicts = 0;
        for (const auto& cell : affected_cells_vec) new_local_conflicts += calculate_cell_conflicts(counts[cell.first][cell.second]);
        for (const auto& cell : affected_cells_vec) visited[cell.first][cell.second] = false;

        long long conflicts_delta = new_local_conflicts - old_local_conflicts;

        if (conflicts_delta < 0 || rnd.next_double() < exp(-conflicts_delta / temp)) {
            current_placements[str_idx] = new_p;
            current_conflicts += conflicts_delta;
            if (current_conflicts < best_conflicts) {
                best_conflicts = current_conflicts;
                best_placements = current_placements;
            }
        } else {
            if (new_p.dir == 0) { for (size_t k=0; k<s.length(); ++k) counts[new_p.r][(new_p.c+k)%N_CONST][char_to_int(s[k])]--; }
            else { for (size_t k=0; k<s.length(); ++k) counts[(new_p.r+k)%N_CONST][new_p.c][char_to_int(s[k])]--; }
            if (old_p.dir == 0) { for (size_t k=0; k<s.length(); ++k) counts[old_p.r][(old_p.c+k)%N_CONST][char_to_int(s[k])]++; }
            else { for (size_t k=0; k<s.length(); ++k) counts[(old_p.r+k)%N_CONST][old_p.c][char_to_int(s[k])]++; }
        }
    }

    vector<bool> is_valid(M, true);
    if (best_conflicts > 0) {
        vector<vector<int>> adj(M);
        vector<int> degree(M, 0);
        vector<vector<pair<int, int>>> cell_users(N_CONST, vector<pair<int, int>>(N_CONST));
        for (int i = 0; i < M; ++i) {
            const auto& p = best_placements[i]; const string& s = S[i];
            if (p.dir == 0) { for (size_t k=0; k<s.length(); ++k) cell_users[p.r][(p.c+k)%N_CONST].push_back({i, char_to_int(s[k])}); }
            else { for (size_t k=0; k<s.length(); ++k) cell_users[(p.r+k)%N_CONST][p.c].push_back({i, char_to_int(s[k])}); }
        }
        for (int r = 0; r < N_CONST; ++r) {
            for (int c = 0; c < N_CONST; ++c) {
                if (cell_users[r][c].size() <= 1) continue;
                for (size_t i = 0; i < cell_users[r][c].size(); ++i) {
                    for (size_t j = i + 1; j < cell_users[r][c].size(); ++j) {
                        auto& u1 = cell_users[r][c][i]; auto& u2 = cell_users[r][c][j];
                        if (u1.second != u2.second) { adj[u1.first].push_back(u2.first); adj[u2.first].push_back(u1.first); }
                    }
                }
            }
        }
        set<pair<int, int>> pq;
        for (int i=0; i<M; ++i) { sort(adj[i].begin(), adj[i].end()); adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end()); degree[i] = adj[i].size(); if (degree[i]>0) pq.insert({-degree[i], i}); }
        
        vector<bool> removed(M, false);
        while (!pq.empty()) {
            int u = pq.begin()->second; pq.erase(pq.begin());
            if (removed[u]) continue;
            removed[u] = true; is_valid[u] = false;
            for (int v : adj[u]) {
                if (!removed[v]) {
                    auto it = pq.find({-degree[v], v});
                    if (it != pq.end()) { pq.erase(it); degree[v]--; if (degree[v]>0) pq.insert({-degree[v], v}); }
                }
            }
        }
    }

    vector<string> grid(N_CONST, string(N_CONST, '.'));
    for (int i = 0; i < M; ++i) {
        if (!is_valid[i]) continue;
        const auto& p = best_placements[i]; const string& s = S[i];
        if (p.dir == 0) { for (size_t k=0; k<s.length(); ++k) grid[p.r][(p.c+k)%N_CONST] = s[k]; }
        else { for (size_t k=0; k<s.length(); ++k) grid[(p.r+k)%N_CONST][p.c] = s[k]; }
    }
    for (int i = 0; i < N_CONST; ++i) cout << grid[i] << endl;

    return 0;
}