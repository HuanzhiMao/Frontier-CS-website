#include <bits/stdc++.h>
using namespace std;

struct Vehicle {
    int id;
    bool horizontal;
    int length;
    int row, col; // row for hor, col for vert
};

long long compute_key(const vector<int>& pos, int n, int base = 7) {
    long long key = 0;
    for (int i = 1; i <= n; ++i) {
        key = key * base + pos[i];
    }
    return key;
}

vector<int> get_pos(long long key, int n, int base = 7) {
    vector<int> p(n + 1);
    for (int i = n; i >= 1; --i) {
        p[i] = key % base;
        key /= base;
    }
    return p;
}

bool place_vehicles(const vector<int>& pos, const vector<Vehicle>& vehicles, int n, int occupied[6][6]) {
    memset(occupied, 0, sizeof(int) * 6 * 6);
    for (int i = 1; i <= n; ++i) {
        const auto& v = vehicles[i];
        int p = pos[i];
        if (v.horizontal) {
            for (int k = 0; k < v.length; ++k) {
                int c = p + k;
                if (c > 5) continue;
                if (occupied[v.row][c] != 0) return false;
                occupied[v.row][c] = i;
            }
        } else {
            for (int k = 0; k < v.length; ++k) {
                int r = p + k;
                if (r > 5) return false;
                if (occupied[r][v.col] != 0) return false;
                occupied[r][v.col] = i;
            }
        }
    }
    return true;
}

void enumerate_configs(int id, int n, const vector<Vehicle>& vehicles, vector<int>& cur_pos, long long cur_key, vector<long long>& all_valid, int base = 7) {
    if (id > n) {
        int occupied[6][6];
        if (place_vehicles(cur_pos, vehicles, n, occupied)) {
            all_valid.push_back(cur_key);
        }
        return;
    }
    int minp = 0;
    int maxp = (id == 1 ? 6 : 6 - vehicles[id].length);
    for (int p = minp; p <= maxp; ++p) {
        cur_pos[id] = p;
        long long new_key = cur_key * base + p;
        enumerate_configs(id + 1, n, vehicles, cur_pos, new_key, all_valid, base);
    }
}

int main() {
    int board[6][6];
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            cin >> board[r][c];
        }
    }
    set<int> id_set;
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            if (board[r][c] > 0) id_set.insert(board[r][c]);
        }
    }
    int n = *id_set.rbegin();
    vector<Vehicle> vehicles(n + 1);
    for (int id = 1; id <= n; ++id) {
        vector<pair<int, int>> positions;
        for (int r = 0; r < 6; ++r) {
            for (int c = 0; c < 6; ++c) {
                if (board[r][c] == id) positions.emplace_back(r, c);
            }
        }
        int sz = positions.size();
        vehicles[id].id = id;
        vehicles[id].length = sz;
        bool same_row = true, same_col = true;
        int ref_r = positions[0].first;
        int ref_c = positions[0].second;
        for (auto& pr : positions) {
            if (pr.first != ref_r) same_row = false;
            if (pr.second != ref_c) same_col = false;
        }
        vehicles[id].horizontal = same_row;
        if (same_row) {
            vehicles[id].row = ref_r;
            sort(positions.begin(), positions.end(), [](auto a, auto b) { return a.second < b.second; });
            int min_c = positions[0].second;
        } else {
            vehicles[id].col = ref_c;
            sort(positions.begin(), positions.end(), [](auto a, auto b) { return a.first < b.first; });
            int min_r = positions[0].first;
        }
    }
    vector<int> initial_pos(n + 1);
    for (int id = 1; id <= n; ++id) {
        const auto& v = vehicles[id];
        if (v.horizontal) {
            int min_c = 6, max_c = -1;
            for (int c = 0; c < 6; ++c) {
                if (board[v.row][c] == id) {
                    min_c = min(min_c, c);
                    max_c = max(max_c, c);
                }
            }
            initial_pos[id] = min_c;
        } else {
            int min_r = 6, max_r = -1;
            for (int r = 0; r < 6; ++r) {
                if (board[r][v.col] == id) {
                    min_r = min(min_r, r);
                    max_r = max(max_r, r);
                }
            }
            initial_pos[id] = min_r;
        }
    }
    long long initial_key = compute_key(initial_pos, n);
    vector<long long> all_valid;
    vector<int> cur_pos(n + 1, 0);
    enumerate_configs(1, n, vehicles, cur_pos, 0LL, all_valid);
    unordered_map<long long, int> solve_dist;
    queue<long long> q;
    for (auto k : all_valid) {
        auto p = get_pos(k, n);
        if (p[1] >= 6) {
            if (solve_dist.find(k) == solve_dist.end()) {
                solve_dist[k] = 0;
                q.push(k);
            }
        }
    }
    int base = 7;
    while (!q.empty()) {
        long long k = q.front();
        q.pop();
        int d = solve_dist[k];
        auto curr_pos = get_pos(k, n);
        int occupied[6][6];
        place_vehicles(curr_pos, vehicles, n, occupied);
        for (int i = 1; i <= n; ++i) {
            const auto& v = vehicles[i];
            int cur_p = curr_pos[i];
            bool is_red = (i == 1);
            bool hor = v.horizontal;
            if (hor) {
                // left L
                if (cur_p > 0) {
                    int new_c = cur_p - 1;
                    if (occupied[v.row][new_c] == 0) {
                        int new_p = cur_p - 1;
                        vector<int> new_pos = curr_pos;
                        new_pos[i] = new_p;
                        long long new_k = compute_key(new_pos, n);
                        if (solve_dist.find(new_k) == solve_dist.end()) {
                            solve_dist[new_k] = d + 1;
                            q.push(new_k);
                        }
                    }
                }
                // right R
                int front_c = cur_p + v.length;
                bool can = false;
                if (front_c <= 5) {
                    if (occupied[v.row][front_c] == 0) can = true;
                } else if (is_red && v.row == 2) {
                    can = true;
                }
                if (can) {
                    int new_p = cur_p + 1;
                    int maxp = (is_red ? 6 : 6 - v.length);
                    if (new_p <= maxp) {
                        vector<int> new_pos = curr_pos;
                        new_pos[i] = new_p;
                        long long new_k = compute_key(new_pos, n);
                        if (solve_dist.find(new_k) == solve_dist.end()) {
                            solve_dist[new_k] = d + 1;
                            q.push(new_k);
                        }
                    }
                }
            } else {
                // up U
                if (cur_p > 0) {
                    int new_r = cur_p - 1;
                    if (occupied[new_r][v.col] == 0) {
                        int new_p = cur_p - 1;
                        vector<int> new_pos = curr_pos;
                        new_pos[i] = new_p;
                        long long new_k = compute_key(new_pos, n);
                        if (solve_dist.find(new_k) == solve_dist.end()) {
                            solve_dist[new_k] = d + 1;
                            q.push(new_k);
                        }
                    }
                }
                // down D
                int front_r = cur_p + v.length;
                if (front_r <= 5 && occupied[front_r][v.col] == 0) {
                    int new_p = cur_p + 1;
                    int maxp = 6 - v.length;
                    if (new_p <= maxp) {
                        vector<int> new_pos = curr_pos;
                        new_pos[i] = new_p;
                        long long new_k = compute_key(new_pos, n);
                        if (solve_dist.find(new_k) == solve_dist.end()) {
                            solve_dist[new_k] = d + 1;
                            q.push(new_k);
                        }
                    }
                }
            }
        }
    }
    unordered_map<long long, int> init_dist;
    unordered_map<long long, pair<long long, pair<int, char>>> prev_move;
    queue<long long> q_init;
    init_dist[initial_key] = 0;
    q_init.push(initial_key);
    prev_move[initial_key] = {-1LL, {-1, ' '}};
    while (!q_init.empty()) {
        long long k = q_init.front();
        q_init.pop();
        int d = init_dist[k];
        auto curr_pos = get_pos(k, n);
        int occupied[6][6];
        place_vehicles(curr_pos, vehicles, n, occupied);
        for (int i = 1; i <= n; ++i) {
            const auto& v = vehicles[i];
            int cur_p = curr_pos[i];
            bool is_red = (i == 1);
            bool hor = v.horizontal;
            char dir_left = hor ? 'L' : 'U';
            char dir_right = hor ? 'R' : 'D';
            // left/up: decrease p
            if (cur_p > 0) {
                bool can = false;
                if (hor) {
                    int new_c = cur_p - 1;
                    can = (occupied[v.row][new_c] == 0);
                } else {
                    int new_r = cur_p - 1;
                    can = (occupied[new_r][v.col] == 0);
                }
                if (can) {
                    int new_p = cur_p - 1;
                    vector<int> new_pos = curr_pos;
                    new_pos[i] = new_p;
                    long long new_k = compute_key(new_pos, n);
                    if (init_dist.find(new_k) == init_dist.end()) {
                        init_dist[new_k] = d + 1;
                        q_init.push(new_k);
                        prev_move[new_k] = {k, {i, dir_left}};
                    }
                }
            }
            // right/down: increase p
            bool can = false;
            int front = cur_p + v.length;
            if (hor) {
                if (front <= 5) {
                    can = (occupied[v.row][front] == 0);
                } else if (is_red && v.row == 2) {
                    can = true;
                }
            } else {
                if (front <= 5) {
                    can = (occupied[front][v.col] == 0);
                }
            }
            if (can) {
                int new_p = cur_p + 1;
                int maxp = (is_red ? 6 : 6 - v.length);
                if (new_p <= maxp && !(is_red && new_p >= 6)) {
                    vector<int> new_pos = curr_pos;
                    new_pos[i] = new_p;
                    long long new_k = compute_key(new_pos, n);
                    if (init_dist.find(new_k) == init_dist.end()) {
                        init_dist[new_k] = d + 1;
                        q_init.push(new_k);
                        prev_move[new_k] = {k, {i, dir_right}};
                    }
                }
            }
        }
    }
    int max_sd = -1;
    long long chosen = -1;
    int min_form = INT_MAX;
    for (auto& pr : init_dist) {
        long long k = pr.first;
        auto it = solve_dist.find(k);
        if (it == solve_dist.end()) continue; // unsolvable, skip
        int sd = it->second;
        int form = pr.second;
        if (sd > max_sd || (sd == max_sd && form < min_form)) {
            max_sd = sd;
            min_form = form;
            chosen = k;
        }
    }
    if (chosen == -1) {
        // no valid, perhaps output 0 0 or something, but assume exists
        cout << 0 << " " << 0 << endl;
        return 0;
    }
    vector<pair<int, char>> moves;
    long long cur = chosen;
    while (prev_move[cur].first != -1LL) {
        auto [prev_k, move] = prev_move[cur];
        moves.push_back(move);
        cur = prev_k;
    }
    reverse(moves.begin(), moves.end());
    cout << max_sd << " " << moves.size() << endl;
    for (auto& m : moves) {
        cout << m.first << " " << m.second << endl;
    }
    return 0;
}