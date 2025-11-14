#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <map>

using namespace std;

int n, m;
vector<string> grid;
int sr, sc, er, ec;
int total_blank_cells = 0;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_char[] = {'U', 'D', 'L', 'R'};
char op_move_char[] = {'D', 'U', 'R', 'L'};

struct State {
    int r1, c1, r2, c2;
};

struct ParentInfo {
    int r1, c1, r2, c2;
    char move;
};

int dist[30][30][30][30];
ParentInfo parent[30][30][30][30];

pair<int, int> do_move(int r, int c, char move) {
    int nr = r, nc = c;
    if (move == 'U') nr--;
    else if (move == 'D') nr++;
    else if (move == 'L') nc--;
    else if (move == 'R') nc++;

    if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '1') {
        return {nr, nc};
    }
    return {r, c};
}

char get_op_move(char move) {
    if (move == 'U') return 'D';
    if (move == 'D') return 'U';
    if (move == 'L') return 'R';
    if (move == 'R') return 'L';
    return ' ';
}

string reconstruct_path(int r1, int c1, int r2, int c2) {
    string path = "";
    int cr1 = r1, cc1 = c1, cr2 = r2, cc2 = c2;
    while (dist[cr1][cc1][cr2][cc2] > 0) {
        ParentInfo p = parent[cr1][cc1][cr2][cc2];
        path += p.move;
        cr1 = p.r1;
        cc1 = p.c1;
        cr2 = p.r2;
        cc2 = p.c2;
    }
    reverse(path.begin(), path.end());
    return path;
}

bool check_solution(const string& s) {
    set<pair<int, int>> visited;
    int r = sr - 1, c = sc - 1;
    visited.insert({r, c});

    for (char move : s) {
        pair<int, int> next_pos = do_move(r, c, move);
        r = next_pos.first;
        c = next_pos.second;
        visited.insert({r, c});
    }

    return visited.size() == total_blank_cells && r == er - 1 && c == ec - 1;
}

void dfs_check_connectivity(int r, int c, vector<vector<bool>>& visited, int& count) {
    if (r < 0 || r >= n || c < 0 || c >= m || visited[r][c] || grid[r][c] == '0') {
        return;
    }
    visited[r][c] = true;
    count++;
    for (int i = 0; i < 4; ++i) {
        dfs_check_connectivity(r + dr[i], c + dc[i], visited, count);
    }
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

    vector<vector<bool>> visited_conn(n, vector<bool>(m, false));
    int component_size = 0;
    if (grid[sr-1][sc-1] == '1') {
        dfs_check_connectivity(sr - 1, sc - 1, visited_conn, component_size);
    }
    if (component_size != total_blank_cells) {
        cout << -1 << endl;
        return 0;
    }
    
    if (check_solution("")) {
        cout << "" << endl;
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < n; ++k) {
                for (int l = 0; l < m; ++l) {
                    dist[i][j][k][l] = -1;
                }
            }
        }
    }

    queue<State> q;
    q.push({sr - 1, sc - 1, er - 1, ec - 1});
    dist[sr - 1][sc - 1][er - 1][ec - 1] = 0;

    int current_dist = 0;

    while (!q.empty()) {
        int level_size = q.size();
        vector<State> level_states;
        while(level_size-- > 0) {
            level_states.push_back(q.front());
            q.pop();
        }

        for (const auto& s : level_states) {
            for (char m_mid : move_char) {
                pair<int, int> p1 = do_move(s.r1, s.c1, m_mid);
                pair<int, int> p2 = do_move(s.r2, s.c2, get_op_move(m_mid));
                if (p1.first == p2.first && p1.second == p2.second) {
                    string p = reconstruct_path(s.r1, s.c1, s.r2, s.c2);
                    string p_rev = p;
                    reverse(p_rev.begin(), p_rev.end());
                    string full_path = p + m_mid + p_rev;
                    if (check_solution(full_path)) {
                        cout << full_path << endl;
                        return 0;
                    }
                }
            }
        }

        for (const auto& s : level_states) {
            for (int i = 0; i < 4; ++i) {
                char move = move_char[i];
                char op_move = op_move_char[i];
                pair<int, int> next1 = do_move(s.r1, s.c1, move);
                pair<int, int> next2 = do_move(s.r2, s.c2, op_move);

                if (dist[next1.first][next1.second][next2.first][next2.second] == -1) {
                    dist[next1.first][next1.second][next2.first][next2.second] = current_dist + 1;
                    parent[next1.first][next1.second][next2.first][next2.second] = {s.r1, s.c1, s.r2, s.c2, move};
                    q.push({next1.first, next1.second, next2.first, next2.second});

                    if (next1.first == next2.first && next1.second == next2.second) {
                        string p = reconstruct_path(next1.first, next1.second, next2.first, next2.second);
                        string p_rev = p;
                        reverse(p_rev.begin(), p_rev.end());
                        string full_path = p + p_rev;
                        if (check_solution(full_path)) {
                            cout << full_path << endl;
                            return 0;
                        }
                    }
                }
            }
        }
        current_dist++;
    }

    cout << -1 << endl;

    return 0;
}