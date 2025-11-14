#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <tuple>
#include <algorithm>
#include <cmath>

using namespace std;

int n, m;
vector<string> grid;
int sr, sc, er, ec;

int dr[] = {-1, 1, 0, 0}; // U, D, L, R
int dc[] = {0, 0, -1, 1};
char moves[] = {'U', 'D', 'L', 'R'};
int op_move_idx[] = {1, 0, 3, 2};

bool is_valid(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < m && grid[r][c] == '1';
}

pair<int, int> do_move(int r, int c, int move_idx) {
    int nr = r + dr[move_idx];
    int nc = c + dc[move_idx];
    if (is_valid(nr, nc)) {
        return {nr, nc};
    }
    return {r, c};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    grid.resize(n);
    vector<pair<int, int>> blank_cells;
    map<pair<int, int>, int> pos_to_idx;
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '1') {
                pos_to_idx[{i, j}] = blank_cells.size();
                blank_cells.push_back({i, j});
            }
        }
    }
    cin >> sr >> sc >> er >> ec;
    --sr; --sc; --er; --ec;

    int B = blank_cells.size();

    if (B == 1) {
        if (sr == er && sc == ec) {
            cout << "" << endl;
        } else {
            cout << -1 << endl;
        }
        return 0;
    }

    // Check connectivity
    vector<bool> visited_conn(B, false);
    queue<int> q_conn;
    int start_node_idx = pos_to_idx.at({sr,sc});
    q_conn.push(start_node_idx);
    visited_conn[start_node_idx] = true;
    int count_conn = 1;
    while (!q_conn.empty()) {
        int u_idx = q_conn.front();
        q_conn.pop();
        pair<int, int> u_pos = blank_cells[u_idx];

        for (int i = 0; i < 4; ++i) {
            int vr = u_pos.first + dr[i];
            int vc = u_pos.second + dc[i];
            if (is_valid(vr, vc)) {
                int v_idx = pos_to_idx.at({vr, vc});
                if (!visited_conn[v_idx]) {
                    visited_conn[v_idx] = true;
                    q_conn.push(v_idx);
                    count_conn++;
                }
            }
        }
    }
    if (count_conn != B) {
        cout << -1 << endl;
        return 0;
    }
    
    if (B > 17) {
        cout << -1 << endl;
        return 0;
    }
    
    int sr_idx = pos_to_idx.at({sr, sc});
    int er_idx = pos_to_idx.at({er, ec});
    
    queue<tuple<int, int, int>> q;
    
    vector<vector<vector<int>>> dist(B, vector<vector<int>>(B, vector<int>(1 << B, -1)));
    vector<vector<vector<char>>> parent_move(B, vector<vector<char>>(B, vector<char>(1 << B, 0)));

    int start_mask = (1 << sr_idx);
    dist[sr_idx][er_idx][start_mask] = 0;
    q.push({sr_idx, er_idx, start_mask});

    int final_p1 = -1, final_p2 = -1, final_mask = -1;

    while (!q.empty()) {
        auto [p1_idx, p2_idx, mask] = q.front();
        q.pop();

        if (p1_idx == er_idx && p2_idx == sr_idx && mask == (1 << B) - 1) {
            final_p1 = p1_idx;
            final_p2 = p2_idx;
            final_mask = mask;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            pair<int, int> p1_pos = blank_cells[p1_idx];
            pair<int, int> p2_pos = blank_cells[p2_idx];

            pair<int, int> np1_pos = do_move(p1_pos.first, p1_pos.second, i);
            pair<int, int> np2_pos = do_move(p2_pos.first, p2_pos.second, op_move_idx[i]);

            int np1_idx = pos_to_idx.at(np1_pos);
            int np2_idx = pos_to_idx.at(np2_pos);
            int nmask = mask | (1 << np1_idx);

            if (dist[np1_idx][np2_idx][nmask] == -1) {
                dist[np1_idx][np2_idx][nmask] = dist[p1_idx][p2_idx][mask] + 1;
                parent_move[np1_idx][np2_idx][nmask] = moves[i];
                q.push({np1_idx, np2_idx, nmask});
            }
        }
    }

    if (final_p1 != -1) {
        string result = "";
        int cur_p1_idx = final_p1;
        int cur_p2_idx = final_p2;
        int cur_mask = final_mask;

        while (dist[cur_p1_idx][cur_p2_idx][cur_mask] > 0) {
            char move_char = parent_move[cur_p1_idx][cur_p2_idx][cur_mask];
            result += move_char;
            
            int move_idx = -1;
            for(int i=0; i<4; ++i) if(moves[i] == move_char) move_idx = i;
            int op_m_idx = op_move_idx[move_idx];

            pair<int, int> cur_p1_pos = blank_cells[cur_p1_idx];
            pair<int, int> cur_p2_pos = blank_cells[cur_p2_idx];

            vector<pair<int,int>> p1_cands, p2_cands;
            p1_cands.push_back(cur_p1_pos);
            p1_cands.push_back({cur_p1_pos.first - dr[move_idx], cur_p1_pos.second - dc[move_idx]});
            p2_cands.push_back(cur_p2_pos);
            p2_cands.push_back({cur_p2_pos.first - dr[op_m_idx], cur_p2_pos.second - dc[op_m_idx]});
            
            bool found_prev = false;
            for(auto p1_cand_pos : p1_cands) {
                if(found_prev) break;
                if(!pos_to_idx.count(p1_cand_pos)) continue;
                if(do_move(p1_cand_pos.first, p1_cand_pos.second, move_idx) != cur_p1_pos) continue;

                for(auto p2_cand_pos : p2_cands) {
                    if(found_prev) break;
                    if(!pos_to_idx.count(p2_cand_pos)) continue;
                    if(do_move(p2_cand_pos.first, p2_cand_pos.second, op_m_idx) != cur_p2_pos) continue;
                    
                    int p1_cand_idx = pos_to_idx.at(p1_cand_pos);
                    int p2_cand_idx = pos_to_idx.at(p2_cand_pos);

                    int prev_mask_cand1 = cur_mask;
                    if (dist[p1_cand_idx][p2_cand_idx][prev_mask_cand1] == dist[cur_p1_idx][cur_p2_idx][cur_mask] - 1) {
                        cur_p1_idx = p1_cand_idx; cur_p2_idx = p2_cand_idx; cur_mask = prev_mask_cand1;
                        found_prev = true;
                    } else {
                        int prev_mask_cand2 = cur_mask & ~(1 << cur_p1_idx);
                        if (dist[p1_cand_idx][p2_cand_idx][prev_mask_cand2] == dist[cur_p1_idx][cur_p2_idx][cur_mask] - 1) {
                            cur_p1_idx = p1_cand_idx; cur_p2_idx = p2_cand_idx; cur_mask = prev_mask_cand2;
                            found_prev = true;
                        }
                    }
                }
            }
        }
        reverse(result.begin(), result.end());
        cout << result << endl;
    } else {
        cout << -1 << endl;
    }

    return 0;
}