#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <tuple>

using namespace std;

const int N = 50;
const int M = 100;

int n_in, m_in;
vector<set<int>> adj;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

bool is_inside(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void read_input() {
    cin >> n_in >> m_in;
    vector<vector<int>> initial_grid(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> initial_grid[i][j];
        }
    }
    adj.assign(M + 1, set<int>());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int c1 = initial_grid[i][j];
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                adj[c1].insert(0);
                adj[0].insert(c1);
            }
            for (int k = 0; k < 4; ++k) {
                int ni = i + dr[k];
                int nj = j + dc[k];
                if (is_inside(ni, nj)) {
                    int c2 = initial_grid[ni][nj];
                    if (c1 != c2) {
                        adj[c1].insert(c2);
                        adj[c2].insert(c1);
                    }
                }
            }
        }
    }
}

bool is_safe(int r, int c, int color, const vector<vector<int>>& grid) {
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_inside(nr, nc)) {
            int neighbor_color = grid[nr][nc];
            if (neighbor_color != 0 && neighbor_color != color) {
                if (adj[color].find(neighbor_color) == adj[color].end()) {
                    return false;
                }
            }
        } else {
             if (adj[color].find(0) == adj[color].end()) {
                 return false;
             }
        }
    }
    return true;
}

vector<pair<int, int>> find_path(int c1, int c2, const vector<vector<int>>& grid) {
    if (c1 == 0 || (c1 == c2)) return {};

    vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    queue<pair<int, int>> q;
    pair<int, int> end_node = {-1, -1};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == c1) {
                for (int k = 0; k < 4; ++k) {
                    int nr = i + dr[k];
                    int nc = j + dc[k];
                    
                    if (!is_inside(nr, nc)) {
                        if (c2 == 0) return {}; 
                    } else if (grid[nr][nc] == c2) {
                        return {};
                    }

                    if (is_inside(nr, nc) && grid[nr][nc] == 0 && !visited[nr][nc]) {
                        if (is_safe(nr, nc, c1, grid)) {
                            q.push({nr, nc});
                            visited[nr][nc] = true;
                            parent[nr][nc] = {i, j};
                        }
                    }
                }
            }
        }
    }

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!is_inside(nr, nc)) {
                 if (c2 == 0) {
                    end_node = curr;
                    goto end_bfs;
                 }
                 continue;
            }

            if (grid[nr][nc] == c2) {
                end_node = curr;
                goto end_bfs;
            }

            if (grid[nr][nc] == 0 && !visited[nr][nc]) {
                 if (is_safe(nr, nc, c1, grid)) {
                    visited[nr][nc] = true;
                    parent[nr][nc] = curr;
                    q.push({nr, nc});
                }
            }
        }
    }

end_bfs:
    if (end_node.first == -1) {
        return {};
    }

    vector<pair<int, int>> path;
    pair<int, int> curr = end_node;
    while (grid[curr.first][curr.second] != c1) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());
    return path;
}

int calculate_score(const vector<vector<int>>& grid) {
    int zero_count = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == 0) {
                zero_count++;
            }
        }
    }
    return zero_count + 1;
}

void print_grid(const vector<vector<int>>& grid) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << grid[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    auto start_time = chrono::high_resolution_clock::now();

    read_input();

    vector<pair<int, int>> reqs;
    for (int i = 0; i <= M; ++i) {
        for (int neighbor : adj[i]) {
            if (i < neighbor) {
                reqs.push_back({i, neighbor});
            }
        }
    }
    
    vector<vector<int>> best_grid(N, vector<int>(N, 1));
    int max_score = -1;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    while(true) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 2900) {
            break;
        }

        vector<vector<int>> current_grid(N, vector<int>(N, 0));
        
        vector<pair<int, int>> seed_pos;
        for (int i = 1; i < N-1; i += 4) {
             for (int j = 1; j < N-1; j += 4) {
                 seed_pos.push_back({i, j});
             }
        }
        if (seed_pos.size() < M) {
             for (int i = 3; i < N-1; i += 4) {
                for (int j = 3; j < N-1; j += 4) {
                    if(seed_pos.size() < M) seed_pos.push_back({i, j});
                }
            }
        }
        shuffle(seed_pos.begin(), seed_pos.end(), rng);
        
        for (int i = 1; i <= M; ++i) {
            current_grid[seed_pos[i-1].first][seed_pos[i-1].second] = i;
        }

        vector<pair<int, int>> current_reqs = reqs;
        shuffle(current_reqs.begin(), current_reqs.end(), rng);
        
        for (int iter = 0; iter < 3; ++iter) {
            if (current_reqs.empty()) break;
            bool changed_this_iter = false;
            vector<pair<int, int>> remaining_reqs;
            for (const auto& p : current_reqs) {
                int c1 = p.first;
                int c2 = p.second;
                
                bool connected = false;
                if (c1 != 0) {
                    vector<pair<int, int>> path = find_path(c1, c2, current_grid);
                    if (!path.empty()) {
                        for (auto cell : path) current_grid[cell.first][cell.second] = c1;
                        connected = true;
                    }
                }
                if (!connected && c2 != 0) {
                    vector<pair<int, int>> path = find_path(c2, c1, current_grid);
                    if (!path.empty()) {
                        for (auto cell : path) current_grid[cell.first][cell.second] = c2;
                        connected = true;
                    }
                }

                if (connected) {
                    changed_this_iter = true;
                } else {
                    remaining_reqs.push_back(p);
                }
            }
            current_reqs = remaining_reqs;
            if (!changed_this_iter && !current_reqs.empty()) break;
        }

        if (!current_reqs.empty()) continue;

        int current_score = calculate_score(current_grid);
        if (current_score > max_score) {
            max_score = current_score;
            best_grid = current_grid;
        }
    }

    print_grid(best_grid);

    return 0;
}