#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <chrono>

// Globals
const int INF = 1e9;
int N;
int start_r, start_c;
std::vector<std::string> C;
int costs[70][70];
bool is_road[70][70];
int total_road_squares = 0;

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

struct State {
    int time;
    int r, c;
    bool operator>(const State& other) const { return time > other.time; }
};

struct Pos {
    int r, c;
};

// Segment endpoint storage
Pos h_seg_ends[70][70];
Pos v_seg_ends[70][70];

// Prefix sums for unseen squares
int row_unseen_prefix_sum[70][71];
int col_unseen_prefix_sum[70][71];

// Function declarations
void dijkstra(int s_r, int s_c, std::vector<std::vector<int>>& dist, std::vector<std::vector<Pos>>& parent);
void update_visibility(int r, int c, std::vector<std::vector<bool>>& visible, int& num_visible);
std::string get_path(int s_r, int s_c, int e_r, int e_c, const std::vector<std::vector<Pos>>& parent);
void precompute_segments();
void update_prefix_sums(const std::vector<std::vector<bool>>& visible);
int calculate_gain_fast(int r, int c, const std::vector<std::vector<bool>>& visible);


void dijkstra(int s_r, int s_c, std::vector<std::vector<int>>& dist, std::vector<std::vector<Pos>>& parent) {
    dist.assign(N, std::vector<int>(N, INF));
    parent.assign(N, std::vector<Pos>(N, {-1, -1}));
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[s_r][s_c] = 0;
    pq.push({0, s_r, s_c});
    while (!pq.empty()) {
        auto [t, r, c] = pq.top();
        pq.pop();
        if (t > dist[r][c]) continue;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N && is_road[nr][nc]) {
                int new_time = t + costs[nr][nc];
                if (new_time < dist[nr][nc]) {
                    dist[nr][nc] = new_time;
                    parent[nr][nc] = {r, c};
                    pq.push({new_time, nr, nc});
                }
            }
        }
    }
}

void update_visibility(int r, int c, std::vector<std::vector<bool>>& visible, int& num_visible) {
    for (int j = c; j >= 0 && is_road[r][j]; --j) if (!visible[r][j]) { visible[r][j] = true; num_visible++; }
    for (int j = c + 1; j < N && is_road[r][j]; ++j) if (!visible[r][j]) { visible[r][j] = true; num_visible++; }
    for (int i = r - 1; i >= 0 && is_road[i][c]; --i) if (!visible[i][c]) { visible[i][c] = true; num_visible++; }
    for (int i = r + 1; i < N && is_road[i][c]; ++i) if (!visible[i][c]) { visible[i][c] = true; num_visible++; }
}

std::string get_path(int s_r, int s_c, int e_r, int e_c, const std::vector<std::vector<Pos>>& parent) {
    if (s_r == e_r && s_c == e_c) return "";
    std::string path = "";
    int cur_r = e_r, cur_c = e_c;
    while (cur_r != s_r || cur_c != s_c) {
        Pos p = parent[cur_r][cur_c];
        int pr = p.r, pc = p.c;
        if (pr == -1) break;
        if (pr == cur_r - 1) path += 'D';
        else if (pr == cur_r + 1) path += 'U';
        else if (pc == cur_c - 1) path += 'R';
        else if (pc == cur_c + 1) path += 'L';
        cur_r = pr; cur_c = pc;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void precompute_segments() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (is_road[i][j]) {
                int j_start = j; while (j_start > 0 && is_road[i][j_start - 1]) j_start--;
                int j_end = j; while (j_end < N - 1 && is_road[i][j_end + 1]) j_end++;
                h_seg_ends[i][j] = {j_start, j_end};
                int i_start = i; while (i_start > 0 && is_road[i_start - 1][j]) i_start--;
                int i_end = i; while (i_end < N - 1 && is_road[i_end + 1][j]) i_end++;
                v_seg_ends[i][j] = {i_start, i_end};
            }
        }
    }
}

void update_prefix_sums(const std::vector<std::vector<bool>>& visible) {
    for (int i = 0; i < N; ++i) {
        row_unseen_prefix_sum[i][0] = 0;
        for (int j = 0; j < N; ++j) {
            row_unseen_prefix_sum[i][j+1] = row_unseen_prefix_sum[i][j] + (is_road[i][j] && !visible[i][j]);
        }
    }
    for (int j = 0; j < N; ++j) {
        col_unseen_prefix_sum[j][0] = 0;
        for (int i = 0; i < N; ++i) {
            col_unseen_prefix_sum[j][i+1] = col_unseen_prefix_sum[j][i] + (is_road[i][j] && !visible[i][j]);
        }
    }
}

int calculate_gain_fast(int r, int c, const std::vector<std::vector<bool>>& visible) {
    Pos h_ends = h_seg_ends[r][c];
    Pos v_ends = v_seg_ends[r][c];
    int h_gain = row_unseen_prefix_sum[r][h_ends.c + 1] - row_unseen_prefix_sum[r][h_ends.r];
    int v_gain = col_unseen_prefix_sum[c][v_ends.c + 1] - col_unseen_prefix_sum[c][v_ends.r];
    int gain = h_gain + v_gain;
    if (is_road[r][c] && !visible[r][c]) {
        gain--;
    }
    return gain;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    auto start_time = std::chrono::high_resolution_clock::now();

    std::cin >> N >> start_r >> start_c;
    C.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> C[i];
        for (int j = 0; j < N; ++j) {
            if (C[i][j] == '#') {
                is_road[i][j] = false;
            } else {
                is_road[i][j] = true;
                costs[i][j] = C[i][j] - '0';
                total_road_squares++;
            }
        }
    }

    precompute_segments();

    int cur_r = start_r, cur_c = start_c;
    std::string total_path = "";
    std::vector<std::vector<bool>> visible(N, std::vector<bool>(N, false));
    int num_visible = 0;

    update_visibility(cur_r, cur_c, visible, num_visible);

    while (num_visible < total_road_squares) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed_ms > 1950) {
            break;
        }

        std::vector<std::vector<int>> dist;
        std::vector<std::vector<Pos>> parent;
        dijkstra(cur_r, cur_c, dist, parent);

        update_prefix_sums(visible);

        double best_score = -1.0;
        int best_r = -1, best_c = -1;
        int best_cost = INF;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!is_road[i][j] || dist[i][j] == INF) continue;
                
                int gain = calculate_gain_fast(i, j, visible);
                if (gain > 0) {
                    double cost = dist[i][j];
                    if (cost == 0) continue; 
                    
                    double score = (double)gain / cost;
                    
                    if (score > best_score) {
                        best_score = score;
                        best_r = i;
                        best_c = j;
                        best_cost = dist[i][j];
                    } else if (score == best_score) {
                        if (dist[i][j] < best_cost) {
                            best_r = i;
                            best_c = j;
                            best_cost = dist[i][j];
                        }
                    }
                }
            }
        }

        if (best_r == -1) break;

        total_path += get_path(cur_r, cur_c, best_r, best_c, parent);
        cur_r = best_r;
        cur_c = best_c;
        update_visibility(cur_r, cur_c, visible, num_visible);
    }
    
    std::vector<std::vector<int>> dist;
    std::vector<std::vector<Pos>> parent;
    dijkstra(cur_r, cur_c, dist, parent);
    total_path += get_path(cur_r, cur_c, start_r, start_c, parent);

    std::cout << total_path << std::endl;

    return 0;
}