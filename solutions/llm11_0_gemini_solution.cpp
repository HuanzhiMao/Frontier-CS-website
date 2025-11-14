#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

int n, m;
vector<string> grid;
int sr, sc, er, ec;
int total_blank_cells = 0;

// L, R, U, D
int dr[] = {0, 0, -1, 1};
int dc[] = {-1, 1, 0, 0};
char moves[] = {'L', 'R', 'U', 'D'};
int inv_move[] = {1, 0, 3, 2};

pair<int, int> next_pos(int r, int c, int move_idx) {
    int nr = r + dr[move_idx];
    int nc = c + dc[move_idx];
    if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1') {
        return {nr, nc};
    }
    return {r, c};
}

int dist[900][900];
pair<int, int> par_state[900][900];
char par_move[900][900];

bool check_all_visited(const string& full_s) {
    if (total_blank_cells > full_s.length() + 1 && total_blank_cells > 1) return false;

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    int count = 0;
    
    int curr_r = sr, curr_c = sc;
    if (!visited[curr_r][curr_c]) {
        visited[curr_r][curr_c] = true;
        count++;
    }

    for (char move_char : full_s) {
        int move_idx = -1;
        if (move_char == 'L') move_idx = 0;
        else if (move_char == 'R') move_idx = 1;
        else if (move_char == 'U') move_idx = 2;
        else if (move_char == 'D') move_idx = 3;
        
        tie(curr_r, curr_c) = next_pos(curr_r, curr_c, move_idx);
        if (!visited[curr_r][curr_c]) {
            visited[curr_r][curr_c] = true;
            count++;
        }
    }

    return count == total_blank_cells;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    grid.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '1') {
                total_blank_cells++;
            }
        }
    }
    cin >> sr >> sc >> er >> ec;
    --sr; --sc; --er; --ec;

    if (sr == er && sc == ec && total_blank_cells == 1) {
        cout << "" << endl;
        return 0;
    }
    
    vector<vector<bool>> visited_conn(n, vector<bool>(m, false));
    queue<pair<int, int>> q_conn;
    if(grid[sr][sc] == '1') {
        q_conn.push({sr, sc});
        visited_conn[sr][sc] = true;
    }
    int count_conn = 0;
    if(grid[sr][sc] == '1') count_conn = 1;

    while(!q_conn.empty()){
        auto curr = q_conn.front();
        q_conn.pop();
        for(int i=0; i<4; ++i){
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1' && !visited_conn[nr][nc]){
                visited_conn[nr][nc] = true;
                count_conn++;
                q_conn.push({nr, nc});
            }
        }
    }
    if(count_conn != total_blank_cells){
        cout << -1 << endl;
        return 0;
    }

    for (int i = 0; i < n * m; ++i) {
        for (int j = 0; j < n * m; ++j) {
            dist[i][j] = -1;
        }
    }

    queue<pair<int, int>> q;
    int start_a_id = sr * m + sc;
    int start_b_id = er * m + ec;

    dist[start_a_id][start_b_id] = 0;
    q.push({start_a_id, start_b_id});
    
    int max_states_per_p = n*m < 31 ? 10000 : 810001;
    vector<vector<pair<int,int>>> states_at_p(n*m + 2);
    if(n*m >=0) {
       states_at_p[0].reserve(1);
       states_at_p[0].push_back({start_a_id, start_b_id});
    }

    int max_p = 0;

    while(!q.empty()){
        pair<int,int> curr = q.front();
        q.pop();
        int u_a_id = curr.first;
        int u_b_id = curr.second;
        int p = dist[u_a_id][u_b_id];
        max_p = max(max_p, p);
        if (p + 1 < states_at_p.size() && states_at_p[p+1].size() < max_states_per_p) {
            states_at_p[p+1].reserve(states_at_p[p].size() * 2);
        }

        int uar = u_a_id / m, uac = u_a_id % m;
        int ubr = u_b_id / m, ubc = u_b_id % m;

        for (int i = 0; i < 4; ++i) {
            auto [nar, nac] = next_pos(uar, uac, i);
            auto [nbr, nbc] = next_pos(ubr, ubc, inv_move[i]);
            int v_a_id = nar * m + nac;
            int v_b_id = nbr * m + nbc;

            if (dist[v_a_id][v_b_id] == -1) {
                dist[v_a_id][v_b_id] = p + 1;
                par_state[v_a_id][v_b_id] = {u_a_id, u_b_id};
                par_move[v_a_id][v_b_id] = moves[i];
                q.push({v_a_id, v_b_id});
                if (p + 1 < states_at_p.size() && states_at_p[p+1].size() < max_states_per_p) {
                    states_at_p[p+1].push_back({v_a_id, v_b_id});
                }
            }
        }
    }

    for (int p = 0; p <= max_p; ++p) {
        // Even length solutions k=2p
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                if (grid[r][c] == '0') continue;
                int meet_id = r * m + c;
                if (dist[meet_id][meet_id] == p) {
                    string s_f_rev = "";
                    int cur_a = meet_id, cur_b = meet_id;
                    while (cur_a != start_a_id || cur_b != start_b_id) {
                        s_f_rev += par_move[cur_a][cur_b];
                        tie(cur_a, cur_b) = par_state[cur_a][cur_b];
                    }
                    string s_f = s_f_rev;
                    reverse(s_f.begin(), s_f.end());
                    
                    string full_s = s_f + s_f_rev;
                    if (check_all_visited(full_s)) {
                        cout << full_s << endl;
                        return 0;
                    }
                }
            }
        }

        // Odd length solutions k=2p+1
        if (p < states_at_p.size()){
            for (auto state : states_at_p[p]) {
                int ar = state.first / m, ac = state.first % m;
                int br_id = state.second;

                for (int i = 0; i < 4; ++i) {
                    auto [nar, nac] = next_pos(ar, ac, i);
                    if (nar * m + nac == br_id) {
                        string s_f_rev = "";
                        int cur_a = state.first, cur_b = state.second;
                        while (cur_a != start_a_id || cur_b != start_b_id) {
                            s_f_rev += par_move[cur_a][cur_b];
                            tie(cur_a, cur_b) = par_state[cur_a][cur_b];
                        }
                        string s_f = s_f_rev;
                        reverse(s_f.begin(), s_f.end());
                        
                        string full_s = s_f + moves[i] + s_f_rev;
                        if(check_all_visited(full_s)){
                           cout << full_s << endl;
                           return 0;
                        }
                    }
                }
            }
        }
    }

    cout << -1 << endl;

    return 0;
}