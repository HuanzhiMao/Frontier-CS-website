#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;

const int N_fixed = 50;
const int M_fixed = 100;

int n, m;
vector<vector<int>> initial_grid;
vector<vector<int>> grid;
vector<vector<int>> best_grid;

set<pair<int, int>> required_adj;
set<int> outer_colors;

map<int, vector<pair<int, int>>> color_cells;
map<pair<int, int>, int> current_adj_counts;
map<int, int> boundary_cell_counts;
int non_zero_cells = 0;
long long current_score = 0;
long long best_score = -1;

mt19937 rng;

int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};

bool is_valid(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < n;
}

void get_adjacencies(const vector<vector<int>>& g, set<pair<int, int>>& adj, set<int>& outer) {
    adj.clear();
    outer.clear();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g[i][j] > 0 && (i == 0 || i == n - 1 || j == 0 || j == n - 1)) {
                outer.insert(g[i][j]);
            }
            for (int k = 0; k < 2; ++k) {
                int ni = i + dx[k];
                int nj = j + dy[k];
                if (is_valid(ni, nj) && g[i][j] != g[ni][nj]) {
                    int u = g[i][j], v = g[ni][nj];
                    if (u > v) swap(u, v);
                    adj.insert({u, v});
                }
            }
        }
    }
}

void update_adj_at(int r, int c, int old_color, int new_color) {
    if (old_color == new_color) return;
    for (int i = 0; i < 4; ++i) {
        int nr = r + dx[i];
        int nc = c + dy[i];
        if (is_valid(nr, nc)) {
            int neighbor_color = grid[nr][nc];
            if (old_color != neighbor_color) {
                int u = old_color, v = neighbor_color;
                if (u > v) swap(u, v);
                if (u != 0) current_adj_counts[{u, v}]--;
            }
            if (new_color != neighbor_color) {
                int u = new_color, v = neighbor_color;
                if (u > v) swap(u, v);
                if (u != 0) current_adj_counts[{u, v}]++;
            }
        }
    }
}

bool check_connectivity(int color, int r_rem, int c_rem) {
    const auto& cells = color_cells.at(color);
    if (cells.size() <= 1) {
        return true;
    }
    pair<int, int> start_node = cells[0];
    if (start_node.first == r_rem && start_node.second == c_rem) {
        start_node = cells[1];
    }
    
    set<pair<int, int>> visited;
    vector<pair<int, int>> q;
    q.push_back(start_node);
    visited.insert(start_node);
    int head = 0;
    while(head < q.size()){
        pair<int, int> curr = q[head++];
        for(int i=0; i<4; ++i){
            int nr = curr.first + dx[i];
            int nc = curr.second + dy[i];
            if(is_valid(nr, nc) && grid[nr][nc] == color && !(nr == r_rem && nc == c_rem)){
                if(visited.find({nr, nc}) == visited.end()){
                    visited.insert({nr, nc});
                    q.push_back({nr, nc});
                }
            }
        }
    }
    return visited.size() == cells.size() - 1;
}

long long calculate_score_full() {
    long long missing_adj = 0;
    for (const auto& p : required_adj) {
        if (current_adj_counts.find(p) == current_adj_counts.end() || current_adj_counts[p] == 0) {
            missing_adj++;
        }
    }

    long long spurious_adj = 0;
    for (const auto& kv : current_adj_counts) {
        if (kv.second > 0 && required_adj.find(kv.first) == required_adj.end()) {
            spurious_adj++;
        }
    }

    long long boundary_fails = 0;
    for (int c = 1; c <= m; ++c) {
        bool needs_outer = outer_colors.count(c);
        bool is_outer = boundary_cell_counts.count(c) && boundary_cell_counts[c] > 0;
        if (needs_outer != is_outer) {
            boundary_fails++;
        }
    }
    return missing_adj * 1000 + spurious_adj * 1000 + boundary_fails * 1000 + non_zero_cells;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    auto start_time = chrono::high_resolution_clock::now();
    rng.seed(start_time.time_since_epoch().count());

    cin >> n >> m;
    initial_grid.assign(N_fixed, vector<int>(N_fixed));
    grid.assign(N_fixed, vector<int>(N_fixed));
    best_grid.assign(N_fixed, vector<int>(N_fixed));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> initial_grid[i][j];
        }
    }

    set<pair<int, int>> req_adj_with_0;
    get_adjacencies(initial_grid, req_adj_with_0, outer_colors);
    for(const auto& p : req_adj_with_0){
        if(p.first != 0){
            required_adj.insert(p);
        }
    }
    
    int current_color = 1;
    for (int i = 0; i < 10 && current_color <= m; ++i) {
        for (int j = 0; j < 10 && current_color <= m; ++j) {
            grid[i * 5][j * 5] = current_color;
            current_color++;
        }
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] > 0) {
                color_cells[grid[i][j]].push_back({i,j});
                non_zero_cells++;
                if (i == 0 || i == n-1 || j == 0 || j == n-1) {
                    boundary_cell_counts[grid[i][j]]++;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < 4; ++k) {
                int ni = i + dx[k], nj = j + dy[k];
                if (is_valid(ni, nj) && grid[i][j] < grid[ni][nj]) {
                    if (grid[i][j] != 0) current_adj_counts[{grid[i][j], grid[ni][nj]}]++;
                }
            }
        }
    }

    current_score = calculate_score_full();
    best_score = current_score;
    best_grid = grid;

    double start_temp = 100, end_temp = 0.1, time_limit = 2.8; 
    uniform_int_distribution<> dis_coord(0, n - 1), dis_dir(0, 3);
    uniform_real_distribution<> dis_prob(0.0, 1.0);

    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
        if (elapsed > time_limit) break;

        double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);

        int r = dis_coord(rng), c = dis_coord(rng);
        int old_color = grid[r][c];
        if (old_color == 0) continue;
        if (color_cells.at(old_color).size() == 1) continue;

        int k = dis_dir(rng);
        int nr = r + dx[k], nc = c + dy[k];
        if (!is_valid(nr, nc)) continue;
        int new_color = grid[nr][nc];
        if (old_color == new_color) continue;
        
        if (!check_connectivity(old_color, r, c)) continue;
        
        long long score_delta = 0;
        if (new_color == 0) score_delta--;
        
        for(int i=0; i<4; ++i) {
            int cr = r + dx[i], cc = c + dy[i];
            if (!is_valid(cr, cc)) continue;
            int D = grid[cr][cc];
            if (D == 0) continue;
            if (old_color != D) {
                int u=old_color, v=D; if(u>v) swap(u,v);
                if (required_adj.count({u,v})) { if (current_adj_counts[{u,v}] == 1) score_delta += 1000; }
                else { if (current_adj_counts[{u,v}] == 1) score_delta -= 1000; }
            }
            if (new_color != D) {
                int u=new_color, v=D; if(u>v) swap(u,v);
                if (required_adj.count({u,v})) { if (current_adj_counts[{u,v}] == 0) score_delta -= 1000; }
                else { if (current_adj_counts[{u,v}] == 0) score_delta += 1000; }
            }
        }
        
        bool on_boundary = (r==0 || r==n-1 || c==0 || c==n-1);
        if (on_boundary) {
            if (outer_colors.count(old_color)) { if (boundary_cell_counts[old_color] == 1) score_delta += 1000; } 
            else { if (boundary_cell_counts.count(old_color) && boundary_cell_counts[old_color] == 1) score_delta -= 1000; }
            
            if (new_color != 0) {
                if (outer_colors.count(new_color)) { if (!boundary_cell_counts.count(new_color) || boundary_cell_counts[new_color] == 0) score_delta -= 1000; }
                else { if (!boundary_cell_counts.count(new_color) || boundary_cell_counts[new_color] == 0) score_delta += 1000; }
            }
        }

        if (score_delta < 0 || dis_prob(rng) < exp(-score_delta / temp)) {
            update_adj_at(r, c, old_color, new_color);
            grid[r][c] = new_color;
            
            auto& old_v = color_cells.at(old_color);
            old_v.erase(remove(old_v.begin(), old_v.end(), make_pair(r, c)), old_v.end());
            if (new_color != 0) color_cells[new_color].push_back({r, c});
            else non_zero_cells--;
            
            if(on_boundary) {
                boundary_cell_counts[old_color]--;
                if (new_color != 0) boundary_cell_counts[new_color]++;
            }
            current_score += score_delta;
            if (current_score < best_score) {
                best_score = current_score;
                best_grid = grid;
            }
        }
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << best_grid[i][j] << (j == n - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}