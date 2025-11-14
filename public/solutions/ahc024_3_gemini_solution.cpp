#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <tuple>
#include <cmath>

using namespace std;

const int N = 50;
const int M = 100;

int n_in, m_in;
vector<vector<int>> c_in;
vector<set<int>> adj;

vector<vector<int>> out_grid;
vector<pair<int, int>> core_pos;
vector<set<int>> current_adj;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void read_input() {
    cin >> n_in >> m_in;
    c_in.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> c_in[i][j];
        }
    }
}

void build_adj() {
    adj.assign(M + 1, set<int>());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int u = c_in[i][j];
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                adj[0].insert(u);
                adj[u].insert(0);
            }
            for (int k = 0; k < 4; ++k) {
                int ni = i + dr[k];
                int nj = j + dc[k];
                if (is_valid(ni, nj)) {
                    int v = c_in[ni][nj];
                    if (u != v) {
                        adj[u].insert(v);
                        adj[v].insert(u);
                    }
                }
            }
        }
    }
}

void place_cores() {
    out_grid.assign(N, vector<int>(N, 0));
    core_pos.assign(M + 1, {-1, -1});
    for (int k = 1; k <= M; ++k) {
        int r = 2 + 4 * ((k - 1) / 10);
        int c = 2 + 4 * ((k - 1) % 10);
        core_pos[k] = {r, c};
        out_grid[r][c] = k;
    }
}

bool is_safe(int r, int c, int u, int v) {
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid(nr, nc)) {
            int neighbor_color = out_grid[nr][nc];
            if (neighbor_color > 0 && neighbor_color != u && neighbor_color != v) {
                return false;
            }
        }
    }
    return true;
}

void connect(int u, int v, bool use_safe) {
    queue<pair<int, int>> q;
    vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));
    vector<vector<int>> dist(N, vector<int>(N, -1));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (out_grid[i][j] == u) {
                q.push({i, j});
                dist[i][j] = 0;
            }
        }
    }

    pair<int, int> end_node = {-1, -1};
    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!is_valid(nr, nc)) {
                if (v == 0) {
                    end_node = curr;
                    goto path_found;
                }
                continue;
            }
            
            if (v != 0 && out_grid[nr][nc] == v) {
                end_node = curr;
                goto path_found;
            }

            if (dist[nr][nc] == -1 && out_grid[nr][nc] == 0) {
                if (!use_safe || (v == 0 ? is_safe(nr, nc, u, u) : is_safe(nr, nc, u, v))) {
                    dist[nr][nc] = dist[curr.first][curr.second] + 1;
                    parent[nr][nc] = curr;
                    q.push({nr, nc});
                }
            }
        }
    }

path_found:
    if (end_node.first == -1) return;

    vector<pair<int, int>> path;
    pair<int, int> curr = end_node;
    while (dist[curr.first][curr.second] != 0) {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    reverse(path.begin(), path.end());

    if (v == 0) {
        for (auto const& p : path) {
            out_grid[p.first][p.second] = u;
        }
    } else {
        size_t mid = path.size() / 2;
        for (size_t i = 0; i < mid; ++i) {
            out_grid[path[i].first][path[i].second] = u;
        }
        for (size_t i = mid; i < path.size(); ++i) {
            out_grid[path[i].first][path[i].second] = v;
        }
    }
}

void update_current_adj() {
    current_adj.assign(M + 1, set<int>());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int u = out_grid[i][j];
            
            if (j + 1 < N) {
                int v = out_grid[i][j + 1];
                if (u != v) {
                    current_adj[u].insert(v);
                    current_adj[v].insert(u);
                }
            }
            if (i + 1 < N) {
                int v = out_grid[i + 1][j];
                if (u != v) {
                    current_adj[u].insert(v);
                    current_adj[v].insert(u);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read_input();
    build_adj();
    place_cores();
    
    vector<tuple<int, int, int>> conns;
    for (int u = 1; u <= M; ++u) {
        for (int v : adj[u]) {
            if (u < v && v != 0) {
                int dist = abs(core_pos[u].first - core_pos[v].first) + abs(core_pos[u].second - core_pos[v].second);
                conns.emplace_back(dist, u, v);
            }
        }
        if (adj[u].count(0)) {
            int dist = min({core_pos[u].first, N - 1 - core_pos[u].first, core_pos[u].second, N - 1 - core_pos[u].second});
            conns.emplace_back(dist, u, 0);
        }
    }

    sort(conns.begin(), conns.end());
    
    update_current_adj();

    for (auto const& t : conns) {
        int u = get<1>(t);
        int v = get<2>(t);

        if (current_adj[u].count(v)) {
            continue;
        }
        
        connect(u, v, true);
        update_current_adj();
        
        if (!current_adj[u].count(v)) {
            connect(u, v, false);
            update_current_adj();
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << out_grid[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}