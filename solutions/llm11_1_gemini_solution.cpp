#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

int n, m;
vector<string> grid;
int sr, sc, er, ec;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char moves[] = {'U', 'D', 'L', 'R'};

int pos_to_idx(int r, int c) {
    return r * m + c;
}

pair<int, int> idx_to_pos(int idx) {
    return {idx / m, idx % m};
}

bool is_valid(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < m && grid[r][c] == '1';
}

pair<int, int> do_move(pair<int, int> pos, char move_char) {
    int move_idx = -1;
    for (int i = 0; i < 4; ++i) {
        if (moves[i] == move_char) {
            move_idx = i;
            break;
        }
    }
    int nr = pos.first + dr[move_idx];
    int nc = pos.second + dc[move_idx];
    if (is_valid(nr, nc)) {
        return {nr, nc};
    }
    return pos;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    grid.resize(n);
    vector<pair<int, int>> blank_cells;
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '1') {
                blank_cells.push_back({i, j});
            }
        }
    }
    cin >> sr >> sc >> er >> ec;
    --sr; --sc; --er; --ec;

    if (!blank_cells.empty()) {
        vector<vector<bool>> visited_conn(n, vector<bool>(m, false));
        queue<pair<int, int>> q_conn;
        if (is_valid(sr, sc)) {
            q_conn.push({sr, sc});
            visited_conn[sr][sc] = true;
        }
        int count = 0;
        while (!q_conn.empty()) {
            pair<int, int> curr = q_conn.front();
            q_conn.pop();
            count++;
            for (int i = 0; i < 4; ++i) {
                int nr = curr.first + dr[i];
                int nc = curr.second + dc[i];
                if (is_valid(nr, nc) && !visited_conn[nr][nc]) {
                    visited_conn[nr][nc] = true;
                    q_conn.push({nr, nc});
                }
            }
        }
        if (count != blank_cells.size()) {
            cout << -1 << endl;
            return 0;
        }
    }

    int total_cells = n * m;
    vector<vector<int>> T(4, vector<int>(total_cells));
    for (int i = 0; i < 4; ++i) {
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                pair<int, int> next_pos = do_move({r, c}, moves[i]);
                T[i][pos_to_idx(r, c)] = pos_to_idx(next_pos.first, next_pos.second);
            }
        }
    }

    map<vector<int>, int> G_to_id;
    vector<vector<int>> id_to_G;
    vector<int> G_identity(total_cells);
    for (int i = 0; i < total_cells; ++i) G_identity[i] = i;
    G_to_id[G_identity] = 0;
    id_to_G.push_back(G_identity);
    int next_G_id = 1;

    queue<pair<pair<int, int>, int>> q;
    q.push({{pos_to_idx(sr, sc), 0}, 0});

    vector<map<int, int>> dist(total_cells);
    dist[pos_to_idx(sr, sc)][0] = 0;
    vector<map<int, pair<pair<int, int>, char>>> parent(total_cells);

    int last_t = -1;
    string best_sol = "-1";

    while (!q.empty()) {
        auto top = q.front();
        q.pop();

        int p_idx = top.first.first;
        int g_idx = top.first.second;
        int t = top.second;
        
        if (last_t != -1 && t > last_t) {
            cout << best_sol << endl;
            return 0;
        }

        const auto& G = id_to_G[g_idx];

        // Even length solution
        if (G[p_idx] == pos_to_idx(er, ec)) {
            string P = "";
            pair<int, int> curr_state = {p_idx, g_idx};
            while (dist[curr_state.first].count(curr_state.second) && dist[curr_state.first][curr_state.second] > 0) {
                auto prev = parent[curr_state.first][curr_state.second];
                P += prev.second;
                curr_state = prev.first;
            }
            reverse(P.begin(), P.end());
            string S = P;
            reverse(P.begin(), P.end());
            S += P;

            set<pair<int, int>> visited_cells;
            pair<int, int> current_pos = {sr, sc};
            visited_cells.insert(current_pos);
            for (char move_char : S) {
                current_pos = do_move(current_pos, move_char);
                visited_cells.insert(current_pos);
            }
            if (visited_cells.size() == blank_cells.size()) {
                if (last_t == -1) {
                    last_t = t;
                    best_sol = S;
                } else if (S.length() < best_sol.length()) {
                    best_sol = S;
                }
            }
        }

        // Odd length solution
        for (int i = 0; i < 4; ++i) {
            int mid_p_idx = T[i][p_idx];
            if (G[mid_p_idx] == pos_to_idx(er, ec)) {
                string P = "";
                pair<int, int> curr_state = {p_idx, g_idx};
                while (dist[curr_state.first].count(curr_state.second) && dist[curr_state.first][curr_state.second] > 0) {
                    auto prev = parent[curr_state.first][curr_state.second];
                    P += prev.second;
                    curr_state = prev.first;
                }
                reverse(P.begin(), P.end());
                string S = P;
                S += moves[i];
                reverse(P.begin(), P.end());
                S += P;

                set<pair<int, int>> visited_cells;
                pair<int, int> current_pos = {sr, sc};
                visited_cells.insert(current_pos);
                for (char move_char : S) {
                    current_pos = do_move(current_pos, move_char);
                    visited_cells.insert(current_pos);
                }

                if (visited_cells.size() == blank_cells.size()) {
                     if (last_t == -1) {
                        last_t = t; // Odd solution of len 2t+1 is found at level t.
                        best_sol = S;
                    } else if (S.length() < best_sol.length()) {
                        best_sol = S;
                    }
                }
            }
        }
        
        if (last_t != -1) continue;

        for (int i = 0; i < 4; ++i) {
            int new_p_idx = T[i][p_idx];
            vector<int> new_G(total_cells);
            for (int j = 0; j < total_cells; ++j) {
                new_G[j] = T[i][G[j]];
            }

            int new_g_idx;
            if (G_to_id.find(new_G) == G_to_id.end()) {
                new_g_idx = next_G_id++;
                G_to_id[new_G] = new_g_idx;
                id_to_G.push_back(new_G);
            } else {
                new_g_idx = G_to_id[new_G];
            }

            if (dist[new_p_idx].find(new_g_idx) == dist[new_p_idx].end()) {
                dist[new_p_idx][new_g_idx] = t + 1;
                parent[new_p_idx][new_g_idx] = {{p_idx, g_idx}, moves[i]};
                q.push({{new_p_idx, new_g_idx}, t + 1});
            }
        }
    }

    if(last_t != -1){
        cout << best_sol << endl;
    } else {
        cout << -1 << endl;
    }

    return 0;
}