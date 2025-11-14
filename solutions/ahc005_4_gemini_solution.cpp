#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>
#include <map>

// A large constant for infinity
const int INF = 1e9;
const long long L_INF = 1e18;

// Grid dimensions and start position
int N;
int si, sj;

// Map representation
std::vector<std::string> C;
std::vector<std::vector<int>> cost;
std::vector<std::vector<bool>> is_road;

// Movement directions (Up, Down, Left, Right)
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// State for Dijkstra's algorithm: {distance, row, column}
struct State {
    int d, r, c;
    bool operator>(const State& other) const {
        return d > other.d;
    }
};

// Dijkstra's algorithm to find shortest paths from a source square
auto dijkstra(int sr, int sc) {
    std::vector<std::vector<int>> dist(N, std::vector<int>(N, INF));
    std::vector<std::vector<std::pair<int, int>>> parent(N, std::vector<std::pair<int, int>>(N, {-1, -1}));
    
    dist[sr][sc] = 0;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, sr, sc});

    while (!pq.empty()) {
        auto [d, r, c] = pq.top();
        pq.pop();

        if (d > dist[r][c]) continue;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < N && nc >= 0 && nc < N && is_road[nr][nc]) {
                if (dist[r][c] + cost[nr][nc] < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + cost[nr][nc];
                    parent[nr][nc] = {r, c};
                    pq.push({dist[nr][nc], nr, nc});
                }
            }
        }
    }
    return std::make_pair(dist, parent);
}

// Reconstructs the path from a start to an end square using parent pointers
std::string reconstruct_path(int sr, int sc, int er, int ec, const std::vector<std::vector<std::pair<int, int>>>& parent) {
    if (sr == er && sc == ec) return "";
    std::string path = "";
    int cr = er, cc = ec;
    while (cr != sr || cc != sc) {
        auto [pr, pc] = parent[cr][cc];
        char move;
        if (cr == pr + 1) move = 'D';
        else if (cr == pr - 1) move = 'U';
        else if (cc == pc + 1) move = 'R';
        else move = 'L';
        path += move;
        cr = pr;
        cc = pc;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Read input
    std::cin >> N >> si >> sj;
    C.resize(N);
    cost.assign(N, std::vector<int>(N, 0));
    is_road.assign(N, std::vector<bool>(N, false));

    for (int i = 0; i < N; ++i) {
        std::cin >> C[i];
        for (int j = 0; j < N; ++j) {
            if (C[i][j] != '#') {
                is_road[i][j] = true;
                cost[i][j] = C[i][j] - '0';
            }
        }
    }
    
    // Identify maximal horizontal and vertical road segments
    std::vector<std::vector<int>> horz_seg_id(N, std::vector<int>(N, -1));
    std::vector<std::vector<int>> vert_seg_id(N, std::vector<int>(N, -1));
    int h_seg_count = 0;
    int v_seg_count = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (is_road[i][j] && horz_seg_id[i][j] == -1) {
                int k = j;
                while (k < N && is_road[i][k]) {
                    horz_seg_id[i][k] = h_seg_count;
                    k++;
                }
                h_seg_count++;
            }
            if (is_road[i][j] && vert_seg_id[i][j] == -1) {
                int k = i;
                while (k < N && is_road[k][j]) {
                    vert_seg_id[k][j] = v_seg_count;
                    k++;
                }
                v_seg_count++;
            }
        }
    }

    // Greedily select target squares to visit to ensure full visibility
    std::vector<std::pair<int, int>> targets;
    std::vector<bool> h_covered(h_seg_count, false);
    std::vector<bool> v_covered(v_seg_count, false);

    targets.push_back({si, sj});
    if(horz_seg_id[si][sj] != -1) h_covered[horz_seg_id[si][sj]] = true;
    if(vert_seg_id[si][sj] != -1) v_covered[vert_seg_id[si][sj]] = true;

    while (true) {
        int uncovered_count = 0;
        for(bool b : h_covered) if(!b) uncovered_count++;
        for(bool b : v_covered) if(!b) uncovered_count++;
        if (uncovered_count == 0) break;

        int best_score = -1;
        std::pair<int, int> best_target = {-1, -1};
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!is_road[i][j]) continue;
                
                int score = 0;
                int h_id = horz_seg_id[i][j];
                int v_id = vert_seg_id[i][j];
                
                if (h_id != -1 && !h_covered[h_id]) score++;
                if (v_id != -1 && !v_covered[v_id]) score++;
                
                if (score > best_score) {
                    best_score = score;
                    best_target = {i, j};
                }
            }
        }
        
        if (best_score <= 0) break;

        targets.push_back(best_target);
        if(horz_seg_id[best_target.first][best_target.second] != -1) h_covered[horz_seg_id[best_target.first][best_target.second]] = true;
        if(vert_seg_id[best_target.first][best_target.second] != -1) v_covered[vert_seg_id[best_target.first][best_target.second]] = true;
    }

    // Remove duplicate targets
    std::sort(targets.begin(), targets.end());
    targets.erase(std::unique(targets.begin(), targets.end()), targets.end());

    int K = targets.size();
    if (K <= 1) {
        std::cout << "" << std::endl;
        return 0;
    }

    // Compute all-pairs shortest paths between targets
    std::vector<std::vector<int>> dist_matrix(K, std::vector<int>(K));
    std::vector<std::vector<std::vector<std::pair<int,int>>>> parent_maps(K);

    for (int i = 0; i < K; ++i) {
        auto [dist, parent] = dijkstra(targets[i].first, targets[i].second);
        parent_maps[i] = parent;
        for (int j = 0; j < K; ++j) {
            dist_matrix[i][j] = dist[targets[j].first][targets[j].second];
        }
    }
    
    // Find the index of the starting square in the targets list
    int start_node_idx = -1;
    for(int i=0; i<K; ++i) {
        if (targets[i].first == si && targets[i].second == sj) {
            start_node_idx = i;
            break;
        }
    }

    // Solve the Traveling Salesperson Problem on the targets using heuristics
    // 1. Build initial tour with Nearest Insertion heuristic
    std::vector<int> tour;
    std::vector<bool> in_tour(K, false);

    tour.push_back(start_node_idx);
    in_tour[start_node_idx] = true;

    while (tour.size() < (size_t)K) {
        int best_u = -1;
        int best_pos = -1;
        long long min_increase = L_INF;

        for (int u = 0; u < K; ++u) {
            if (in_tour[u]) continue;
            
            for (size_t i = 0; i < tour.size(); ++i) {
                int v_idx = tour[i];
                int w_idx = tour[(i + 1) % tour.size()];

                long long increase = (long long)dist_matrix[v_idx][u] + dist_matrix[u][w_idx] - dist_matrix[v_idx][w_idx];
                if (increase < min_increase) {
                    min_increase = increase;
                    best_u = u;
                    best_pos = i + 1;
                }
            }
        }
        tour.insert(tour.begin() + best_pos, best_u);
        in_tour[best_u] = true;
    }

    // Rotate tour to begin at the starting square
    auto it = std::find(tour.begin(), tour.end(), start_node_idx);
    std::rotate(tour.begin(), it, tour.end());

    // 2. Improve the tour using 2-opt heuristic
    bool improved = true;
    while(improved) {
        improved = false;
        for (int i = 0; i < K - 1; ++i) {
            for (int j = i + 2; j < K; ++j) {
                int u1 = tour[i];
                int u2 = tour[i+1];
                int v1 = tour[j];
                int v2 = tour[(j + 1) % K];
                
                long long current_dist = (long long)dist_matrix[u1][u2] + dist_matrix[v1][v2];
                long long new_dist = (long long)dist_matrix[u1][v1] + dist_matrix[u2][v2];
                
                if (new_dist < current_dist) {
                    std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    
    // Generate the final path string by reconstructing paths between consecutive targets in the tour
    std::string final_path = "";
    for (size_t i = 0; i < tour.size(); ++i) {
        int u_idx = tour[i];
        int v_idx = tour[(i + 1) % tour.size()];
        final_path += reconstruct_path(targets[u_idx].first, targets[u_idx].second, 
                                       targets[v_idx].first, targets[v_idx].second,
                                       parent_maps[u_idx]);
    }
    
    std::cout << final_path << std::endl;

    return 0;
}