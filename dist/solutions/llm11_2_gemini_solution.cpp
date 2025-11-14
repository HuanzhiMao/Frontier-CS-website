#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>
#include <set>

using namespace std;

int n, m;
vector<string> grid;
int sr_in, sc_in, er_in, ec_in;
int total_blank_cells = 0;

// U, D, L, R
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_chars[] = {'U', 'D', 'L', 'R'};

struct State {
    short r1, c1, r2, c2;
};

struct ParentInfo {
    short pr1, pc1, pr2, pc2;
    char move;
};

ParentInfo parent[31][31][31][31];
bool visited_bfs[31][31][31][31];

pair<int, int> get_next_pos(int r, int c, char move) {
    int move_idx = -1;
    if (move == 'U') move_idx = 0;
    else if (move == 'D') move_idx = 1;
    else if (move == 'L') move_idx = 2;
    else if (move == 'R') move_idx = 3;
    
    int nr = r + dr[move_idx];
    int nc = c + dc[move_idx];
    if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1') {
        return {nr, nc};
    }
    return {r, c};
}

bool check_all_visited(const string& s) {
    if (total_blank_cells == 0) return true;

    vector<vector<bool>> visited_cells(n, vector<bool>(m, false));
    int count = 0;
    
    int r = sr_in, c = sc_in;
    if (!visited_cells[r][c]) {
        visited_cells[r][c] = true;
        count++;
    }

    for (char move : s) {
        pair<int, int> next_pos = get_next_pos(r, c, move);
        r = next_pos.first;
        c = next_pos.second;
        if (!visited_cells[r][c]) {
            visited_cells[r][c] = true;
            count++;
        }
    }

    if (r != er_in || c != ec_in) {
        return false;
    }
    
    return count == total_blank_cells;
}

string reconstruct_prefix(int r1, int c1, int r2, int c2) {
    string prefix = "";
    int cr1 = r1, cc1 = c1, cr2 = r2, cc2 = c2;
    while (cr1 != sr_in || cc1 != sc_in || cr2 != er_in || cc2 != ec_in) {
        ParentInfo p = parent[cr1][cc1][cr2][cc2];
        prefix += p.move;
        int next_cr1 = p.pr1;
        int next_cc1 = p.pc1;
        int next_cr2 = p.pr2;
        int next_cc2 = p.pc2;
        cr1 = next_cr1;
        cc1 = next_cc1;
        cr2 = next_cr2;
        cc2 = next_cc2;
    }
    reverse(prefix.begin(), prefix.end());
    return prefix;
}


bool connectivity_check() {
    if (total_blank_cells == 0) return true;
    if (grid[sr_in][sc_in] == '0') return false;
    
    vector<vector<bool>> visited_grid(n, vector<bool>(m, false));
    queue<pair<int, int>> q;
    
    q.push({sr_in, sc_in});
    visited_grid[sr_in][sc_in] = true;
    int count = 1;

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1' && !visited_grid[nr][nc]) {
                visited_grid[nr][nc] = true;
                q.push({nr, nc});
                count++;
            }
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
    cin >> sr_in >> sc_in >> er_in >> ec_in;
    --sr_in; --sc_in; --er_in; --ec_in;

    if (!connectivity_check()) {
        cout << -1 << endl;
        return 0;
    }

    queue<State> q;
    q.push({(short)sr_in, (short)sc_in, (short)er_in, (short)ec_in});
    visited_bfs[sr_in][sc_in][er_in][ec_in] = true;

    while (!q.empty()) {
        int level_size = q.size();
        vector<State> current_level_states;
        current_level_states.reserve(level_size);
        for (int i = 0; i < level_size; ++i) {
            current_level_states.push_back(q.front());
            q.pop();
        }

        for (const auto& s : current_level_states) {
            if (s.r1 == s.r2 && s.c1 == s.c2) {
                string prefix = reconstruct_prefix(s.r1, s.c1, s.r2, s.c2);
                string suffix = prefix;
                reverse(suffix.begin(), suffix.end());
                string full_path = prefix + suffix;
                if (check_all_visited(full_path)) {
                    cout << full_path << endl;
                    return 0;
                }
            }
        }
        
        for (const auto& s : current_level_states) {
            for (char move : move_chars) {
                pair<int, int> next1 = get_next_pos(s.r1, s.c1, move);
                pair<int, int> next2 = get_next_pos(s.r2, s.c2, move);
                if (next1.first == next2.first && next1.second == next2.second) {
                    string prefix = reconstruct_prefix(s.r1, s.c1, s.r2, s.c2);
                    string suffix = prefix;
                    reverse(suffix.begin(), suffix.end());
                    string full_path = prefix + move + suffix;
                    if (check_all_visited(full_path)) {
                        cout << full_path << endl;
                        return 0;
                    }
                }
            }
        }

        for (const auto& s : current_level_states) {
            for (char move : move_chars) {
                pair<int, int> next1 = get_next_pos(s.r1, s.c1, move);
                pair<int, int> next2 = get_next_pos(s.r2, s.c2, move);
                int nr1 = next1.first, nc1 = next1.second;
                int nr2 = next2.first, nc2 = next2.second;

                if (!visited_bfs[nr1][nc1][nr2][nc2]) {
                    visited_bfs[nr1][nc1][nr2][nc2] = true;
                    parent[nr1][nc1][nr2][nc2] = {s.r1, s.c1, s.r2, s.c2, move};
                    q.push({(short)nr1, (short)nc1, (short)nr2, (short)nc2});
                }
            }
        }
    }

    cout << -1 << endl;

    return 0;
}