#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <tuple>

using namespace std;

const int N = 50;
const int M = 100;

int n_in, m_in;
vector<vector<int>> initial_grid(N, vector<int>(N));
vector<vector<int>> res(N, vector<int>(N, 0));
vector<set<int>> adj(M + 1);

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

bool is_valid_cell(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void build_adj() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int c1 = initial_grid[i][j];
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                if (adj[c1].find(0) == adj[c1].end()) {
                    adj[c1].insert(0);
                    adj[0].insert(c1);
                }
            }
            for (int k = 0; k < 4; ++k) {
                int ni = i + dr[k];
                int nj = j + dc[k];
                if (is_valid_cell(ni, nj)) {
                    int c2 = initial_grid[ni][nj];
                    if (c1 != c2) {
                        if (adj[c1].find(c2) == adj[c1].end()) {
                            adj[c1].insert(c2);
                            adj[c2].insert(c1);
                        }
                    }
                }
            }
        }
    }
}

bool check_validity(int r, int c, int color, int k, int l) {
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid_cell(nr, nc)) {
            int neighbor_color = res[nr][nc];
            if (neighbor_color > 0 && neighbor_color != k && neighbor_color != l) {
                if (adj[color].find(neighbor_color) == adj[color].end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

void color_path(pair<int, int> end_pos, int color, const vector<vector<pair<int, int>>>& parent, vector<vector<bool>>& current_adj) {
    pair<int, int> curr = end_pos;
    while (curr.first != -1) {
        if (res[curr.first][curr.second] == 0) {
            res[curr.first][curr.second] = color;
            for (int i = 0; i < 4; ++i) {
                int nr = curr.first + dr[i];
                int nc = curr.second + dc[i];
                if (is_valid_cell(nr, nc)) {
                    int neighbor_color = res[nr][nc];
                    if (neighbor_color > 0 && neighbor_color != color) {
                        current_adj[color][neighbor_color] = current_adj[neighbor_color][color] = true;
                    }
                }
            }
        }
        curr = parent[curr.first][curr.second];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n_in >> m_in;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> initial_grid[i][j];
        }
    }

    build_adj();

    vector<pair<int, int>> initial_pos(M + 1);
    for (int k = 1; k <= M; ++k) {
        int r = 2 + 5 * ((k - 1) / 10);
        int c = 2 + 5 * ((k - 1) % 10);
        res[r][c] = k;
        initial_pos[k] = {r, c};
    }
    
    vector<vector<bool>> current_adj(M + 1, vector<bool>(M + 1, false));

    for (int k = 1; k <= M; ++k) {
        if (adj[k].count(0)) {
            queue<pair<int, int>> q_bfs;
            q_bfs.push(initial_pos[k]);
            vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));
            vector<vector<bool>> visited(N, vector<bool>(N, false));
            visited[initial_pos[k].first][initial_pos[k].second] = true;
            
            pair<int,int> boundary_cell = {-1, -1};

            while(!q_bfs.empty()){
                pair<int,int> curr = q_bfs.front();
                q_bfs.pop();

                if(curr.first == 0 || curr.first == N-1 || curr.second == 0 || curr.second == N-1){
                    boundary_cell = curr;
                    break;
                }

                for(int i=0; i<4; ++i){
                    int nr = curr.first + dr[i];
                    int nc = curr.second + dc[i];
                    if(is_valid_cell(nr, nc) && !visited[nr][nc] && res[nr][nc] == 0){
                         if (check_validity(nr, nc, k, k, k)) {
                            visited[nr][nc] = true;
                            parent[nr][nc] = curr;
                            q_bfs.push({nr,nc});
                        }
                    }
                }
            }

            if(boundary_cell.first != -1){
                color_path(boundary_cell, k, parent, current_adj);
            }
        }
    }

    vector<tuple<double, int, int>> edges;
    for (int k = 1; k <= M; ++k) {
        for (int l = k + 1; l <= M; ++l) {
            if (adj[k].count(l)) {
                double dist = pow(initial_pos[k].first - initial_pos[l].first, 2) + pow(initial_pos[k].second - initial_pos[l].second, 2);
                edges.emplace_back(dist, k, l);
            }
        }
    }
    sort(edges.begin(), edges.end());

    for (const auto& edge : edges) {
        int k = get<1>(edge);
        int l = get<2>(edge);

        if (current_adj[k][l]) continue;

        queue<pair<int, int>> q;
        vector<vector<int>> visited_by(N, vector<int>(N, 0));
        vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));

        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (res[r][c] == k) {
                    q.push({r, c});
                    visited_by[r][c] = 1;
                } else if (res[r][c] == l) {
                    q.push({r, c});
                    visited_by[r][c] = 2;
                }
            }
        }
        
        pair<int, int> meet_point1 = {-1, -1}, meet_point2 = {-1, -1};
        bool path_found = false;

        while (!q.empty()) {
            pair<int, int> curr = q.front();
            q.pop();
            int c_visited_by = visited_by[curr.first][curr.second];
            int c_color = (c_visited_by == 1) ? k : l;

            for (int i = 0; i < 4; ++i) {
                int nr = curr.first + dr[i];
                int nc = curr.second + dc[i];

                if (is_valid_cell(nr, nc)) {
                    int neighbor_visited_by = visited_by[nr][nc];
                    if (res[nr][nc] == 0 && neighbor_visited_by == 0) {
                        if (check_validity(nr, nc, c_color, k, l)) {
                            visited_by[nr][nc] = c_visited_by;
                            parent[nr][nc] = curr;
                            q.push({nr, nc});
                        }
                    } else if (neighbor_visited_by != 0 && neighbor_visited_by != c_visited_by) {
                         meet_point1 = curr;
                         meet_point2 = {nr, nc};
                         path_found = true;
                         break;
                    }
                }
            }
            if(path_found) break;
        }
        
        if (path_found) {
            color_path(meet_point1, (visited_by[meet_point1.first][meet_point1.second] == 1 ? k : l), parent, current_adj);
            color_path(meet_point2, (visited_by[meet_point2.first][meet_point2.second] == 1 ? k : l), parent, current_adj);
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << res[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}