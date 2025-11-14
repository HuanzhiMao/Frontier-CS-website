#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <chrono>

using namespace std;

auto start_time = chrono::steady_clock::now();

struct Point {
    int r, c;
    bool operator<(const Point& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
    bool operator==(const Point& other) const {
        return r == other.r && c == other.c;
    }
};

struct State {
    int r, c, cost;
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

int N;
int start_r, start_c;
vector<string> grid;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_chars[] = {'U', 'D', 'L', 'R'};

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N && grid[r][c] != '#';
}

vector<vector<int>> h_seg_id, v_seg_id;
vector<vector<Point>> h_segs_pts, v_segs_pts;
set<Point> targets_set;
vector<Point> target_nodes;
map<Point, int> point_to_idx;

void find_segments_and_targets() {
    h_seg_id.assign(N, vector<int>(N, 0));
    v_seg_id.assign(N, vector<int>(N, 0));
    int h_seg_count = 0;
    int v_seg_count = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (is_valid(i, j) && h_seg_id[i][j] == 0) {
                h_seg_count++;
                vector<Point> current_seg_pts;
                int k = j;
                while (k < N && is_valid(i, k)) {
                    h_seg_id[i][k] = h_seg_count;
                    current_seg_pts.push_back({i, k});
                    k++;
                }
                h_segs_pts.push_back(current_seg_pts);
            }
        }
    }
    
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            if (is_valid(i, j) && v_seg_id[i][j] == 0) {
                v_seg_count++;
                vector<Point> current_seg_pts;
                int k = i;
                while (k < N && is_valid(k, j)) {
                    v_seg_id[k][j] = v_seg_count;
                    current_seg_pts.push_back({k, j});
                    k++;
                }
                v_segs_pts.push_back(current_seg_pts);
            }
        }
    }

    vector<bool> h_covered(h_seg_count + 1, false);
    vector<bool> v_covered(v_seg_count + 1, false);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!is_valid(i, j)) continue;
            bool is_on_h = (j > 0 && is_valid(i, j - 1)) || (j < N - 1 && is_valid(i, j + 1));
            bool is_on_v = (i > 0 && is_valid(i - 1, j)) || (i < N - 1 && is_valid(i + 1, j));
            if (is_on_h && is_on_v) {
                targets_set.insert({i, j});
                if(h_seg_id[i][j] > 0) h_covered[h_seg_id[i][j]] = true;
                if(v_seg_id[i][j] > 0) v_covered[v_seg_id[i][j]] = true;
            }
        }
    }
    
    for (int i = 0; i < h_seg_count; ++i) {
        if (!h_covered[i + 1] && !h_segs_pts[i].empty()) {
            targets_set.insert(h_segs_pts[i][h_segs_pts[i].size() / 2]);
        }
    }
    for (int i = 0; i < v_seg_count; ++i) {
        if (!v_covered[i + 1] && !v_segs_pts[i].empty()) {
            targets_set.insert(v_segs_pts[i][v_segs_pts[i].size() / 2]);
        }
    }
}

vector<vector<int>> dist_matrix;
vector<vector<vector<Point>>> all_parents;

void dijkstra(Point start_p, int start_idx) {
    vector<vector<int>> d(N, vector<int>(N, 1e9));
    vector<vector<Point>> parent(N, vector<Point>(N, {-1, -1}));
    priority_queue<State, vector<State>, greater<State>> pq;

    d[start_p.r][start_p.c] = 0;
    pq.push({start_p.r, start_p.c, 0});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.cost > d[current.r][current.c]) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = current.r + dr[i];
            int nc = current.c + dc[i];

            if (is_valid(nr, nc)) {
                int move_cost = grid[nr][nc] - '0';
                if (d[nr][nc] > d[current.r][current.c] + move_cost) {
                    d[nr][nc] = d[current.r][current.c] + move_cost;
                    parent[nr][nc] = {current.r, current.c};
                    pq.push({nr, nc, d[nr][nc]});
                }
            }
        }
    }
    
    all_parents[start_idx] = parent;
    for (size_t i = 0; i < target_nodes.size(); ++i) {
        Point end_p = target_nodes[i];
        dist_matrix[start_idx][i] = d[end_p.r][end_p.c];
    }
}

string get_path_str(const vector<Point>& path) {
    string res = "";
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int dr_move = path[i+1].r - path[i].r;
        int dc_move = path[i+1].c - path[i].c;
        if (dr_move == -1) res += 'U';
        else if (dr_move == 1) res += 'D';
        else if (dc_move == -1) res += 'L';
        else res += 'R';
    }
    return res;
}

string reconstruct_path(int start_idx, int end_idx) {
    vector<Point> path;
    Point start_p = target_nodes[start_idx];
    Point end_p = target_nodes[end_idx];
    Point curr = end_p;
    while(curr.r != -1) {
        path.push_back(curr);
        if(curr.r == start_p.r && curr.c == start_p.c) break;
        curr = all_parents[start_idx][curr.r][curr.c];
    }
    reverse(path.begin(), path.end());
    return get_path_str(path);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> start_r >> start_c;
    grid.resize(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];
    
    find_segments_and_targets();

    target_nodes.push_back({start_r, start_c});
    for(const auto& p : targets_set) {
        if(p.r == start_r && p.c == start_c) continue;
        target_nodes.push_back(p);
    }
    
    for(size_t i = 0; i < target_nodes.size(); ++i) {
        point_to_idx[target_nodes[i]] = i;
    }

    int num_nodes = target_nodes.size();
    dist_matrix.assign(num_nodes, vector<int>(num_nodes));
    all_parents.assign(num_nodes, vector<vector<Point>>(N, vector<Point>(N)));
    for (int i = 0; i < num_nodes; ++i) {
        dijkstra(target_nodes[i], i);
    }
    
    vector<int> tour(num_nodes);
    tour[0] = 0;
    vector<bool> visited(num_nodes, false);
    visited[0] = true;
    int current_node_idx = 0;
    
    for (int i = 1; i < num_nodes; ++i) {
        int next_node_idx = -1;
        int min_dist = 1e9;
        for (int j = 0; j < num_nodes; ++j) {
            if (!visited[j]) {
                if (dist_matrix[current_node_idx][j] < min_dist) {
                    min_dist = dist_matrix[current_node_idx][j];
                    next_node_idx = j;
                }
            }
        }
        tour[i] = next_node_idx;
        visited[next_node_idx] = true;
        current_node_idx = next_node_idx;
    }

    bool improved = true;
    while (improved) {
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count();
        if (elapsed > 2800) break;

        improved = false;
        for (int i = 1; i < num_nodes - 1; ++i) {
            for (int j = i + 1; j < num_nodes; ++j) {
                int u1 = tour[i - 1], v1 = tour[i];
                int u2 = tour[j], v2 = tour[(j + 1) % num_nodes];
                if(j + 1 == num_nodes) v2 = tour[0];

                long long current_dist = dist_matrix[u1][v1] + dist_matrix[u2][v2];
                long long new_dist = dist_matrix[u1][u2] + dist_matrix[v1][v2];

                if (new_dist < current_dist) {
                    reverse(tour.begin() + i, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    
    string final_path = "";
    for (size_t i = 0; i < tour.size(); ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % tour.size()];
        final_path += reconstruct_path(u, v);
    }

    cout << final_path << endl;

    return 0;
}