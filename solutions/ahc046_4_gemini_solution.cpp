#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <map>
#include <algorithm>
#include <tuple>

using namespace std;

const int N = 20;
const int M = 40;

struct Pos {
    int r, c;
    bool operator==(const Pos& other) const {
        return r == other.r && c == other.c;
    }
    bool operator<(const Pos& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
};

int manhattan_dist(const Pos& a, const Pos& b) {
    return abs(a.r - b.r) + abs(a.c - b.c);
}

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
const char dir_char[] = {'U', 'D', 'L', 'R'};

struct State {
    int cost;
    int g_cost;
    Pos pos;

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

struct ParentInfo {
    Pos pos;
    char action;
    char dir;
};

vector<pair<char, char>> find_path_astar(Pos start, Pos end, const vector<vector<bool>>& grid) {
    if (start == end) {
        return {};
    }

    priority_queue<State, vector<State>, greater<State>> pq;
    map<Pos, int> g_costs;
    map<Pos, ParentInfo> parent;

    g_costs[start] = 0;
    pq.push({manhattan_dist(start, end), 0, start});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int cost = current.cost;
        int g_cost = current.g_cost;
        Pos pos = current.pos;

        if (g_costs.count(pos) && g_cost > g_costs[pos]) {
            continue;
        }

        if (pos == end) {
            break;
        }

        // Moves
        for (int i = 0; i < 4; ++i) {
            int nr = pos.r + dr[i];
            int nc = pos.c + dc[i];
            if (is_valid(nr, nc) && !grid[nr][nc]) {
                Pos next_pos = {nr, nc};
                if (g_costs.find(next_pos) == g_costs.end() || g_cost + 1 < g_costs[next_pos]) {
                    g_costs[next_pos] = g_cost + 1;
                    parent[next_pos] = {pos, 'M', dir_char[i]};
                    pq.push({g_cost + 1 + manhattan_dist(next_pos, end), g_cost + 1, next_pos});
                }
            }
        }

        // Slides
        for (int i = 0; i < 4; ++i) {
            int r = pos.r;
            int c = pos.c;
            while (true) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (!is_valid(nr, nc) || grid[nr][nc]) {
                    break;
                }
                r = nr;
                c = nc;
            }
            Pos next_pos = {r, c};
            if (next_pos == pos) continue;
            
            if (g_costs.find(next_pos) == g_costs.end() || g_cost + 1 < g_costs[next_pos]) {
                g_costs[next_pos] = g_cost + 1;
                parent[next_pos] = {pos, 'S', dir_char[i]};
                pq.push({g_cost + 1 + manhattan_dist(next_pos, end), g_cost + 1, next_pos});
            }
        }
    }

    vector<pair<char, char>> path;
    Pos current = end;
    while (!(current == start)) {
        ParentInfo p_info = parent[current];
        path.push_back({p_info.action, p_info.dir});
        current = p_info.pos;
    }
    reverse(path.begin(), path.end());
    return path;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy, m_dummy;
    cin >> n_dummy >> m_dummy;

    vector<Pos> targets(M);
    vector<vector<bool>> is_target(N, vector<bool>(N, false));
    for (int i = 0; i < M; ++i) {
        cin >> targets[i].r >> targets[i].c;
        is_target[targets[i].r][targets[i].c] = true;
    }

    vector<vector<int>> usefulness(N, vector<int>(N, 0));
    for (int k = 0; k < M - 1; ++k) {
        Pos s = targets[k];
        Pos t = targets[k+1];

        int sgn_r = (t.r > s.r) ? 1 : -1;
        int sgn_c = (t.c > s.c) ? 1 : -1;
        
        if (s.r != t.r) {
            // B_v1 for S -> P2=(t.r, s.c) (V-slide)
            Pos b_v1 = {t.r + sgn_r, s.c};
            if (is_valid(b_v1.r, b_v1.c)) usefulness[b_v1.r][b_v1.c]++;
            
            // B_v2 for P1=(s.r, t.c) -> T (V-slide)
            Pos b_v2 = {t.r + sgn_r, t.c};
            if (is_valid(b_v2.r, b_v2.c)) usefulness[b_v2.r][b_v2.c]++;
        }
        if (s.c != t.c) {
            // B_h1 for S -> P1=(s.r, t.c) (H-slide)
            Pos b_h1 = {s.r, t.c + sgn_c};
            if (is_valid(b_h1.r, b_h1.c)) usefulness[b_h1.r][b_h1.c]++;

            // B_h2 for P2=(t.r, s.c) -> T (H-slide)
            Pos b_h2 = {t.r, t.c + sgn_c};
            if (is_valid(b_h2.r, b_h2.c)) usefulness[b_h2.r][b_h2.c]++;
        }
    }

    vector<pair<int, Pos>> useful_blocks;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (usefulness[i][j] > 0 && !is_target[i][j]) {
                useful_blocks.push_back({usefulness[i][j], {i, j}});
            }
        }
    }
    sort(useful_blocks.rbegin(), useful_blocks.rend());

    int K = 20;
    vector<Pos> blocks_to_place;
    for (int i = 0; i < min((int)useful_blocks.size(), K); ++i) {
        blocks_to_place.push_back(useful_blocks[i].second);
    }

    vector<vector<bool>> grid(N, vector<bool>(N, false));
    Pos current_pos = targets[0];
    vector<pair<char, char>> actions;

    for (const auto& block_pos : blocks_to_place) {
        Pos best_neighbor;
        int min_d = 1e9;
        char best_dir_char = ' ';

        for (int i = 0; i < 4; ++i) {
            int nr = block_pos.r - dr[i];
            int nc = block_pos.c - dc[i];
            if (is_valid(nr, nc)) {
                int d = manhattan_dist(current_pos, {nr, nc});
                if (d < min_d) {
                    min_d = d;
                    best_neighbor = {nr, nc};
                    best_dir_char = dir_char[i];
                }
            }
        }
        
        if(min_d == 1e9) continue; // no valid neighbor

        vector<pair<char, char>> path = find_path_astar(current_pos, best_neighbor, grid);
        actions.insert(actions.end(), path.begin(), path.end());
        current_pos = best_neighbor;

        actions.push_back({'A', best_dir_char});
        grid[block_pos.r][block_pos.c] = true;
    }

    for (int i = 1; i < M; ++i) {
        vector<pair<char, char>> path = find_path_astar(current_pos, targets[i], grid);
        actions.insert(actions.end(), path.begin(), path.end());
        current_pos = targets[i];
    }

    for (const auto& action : actions) {
        cout << action.first << " " << action.second << "\n";
    }

    return 0;
}