#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <cmath>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

const int MAX_COORD_VAL = 100000;
const int GRID_SIZE = 250;
const int CELL_SIZE = (MAX_COORD_VAL + 1 + GRID_SIZE - 1) / GRID_SIZE;

struct Point {
    int x, y;
};

bool operator<(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}
bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

// Check if a point is inside or on the boundary of a rectilinear polygon
bool is_inside(const Point& p, const std::vector<Point>& polygon) {
    int n = polygon.size();
    if (n < 3) return false;

    bool on_boundary = false;
    for (int i = 0; i < n; ++i) {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        if (p1.x == p2.x && p.x == p1.x && p.y >= std::min(p1.y, p2.y) && p.y <= std::max(p1.y, p2.y)) {
            on_boundary = true;
            break;
        }
        if (p1.y == p2.y && p.y == p1.y && p.x >= std::min(p1.x, p2.x) && p.x <= std::max(p1.x, p2.x)) {
            on_boundary = true;
            break;
        }
    }
    if (on_boundary) return true;

    bool inside = false;
    for (int i = 0; i < n; ++i) {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        if (((p1.y > p.y) != (p2.y > p.y)) &&
            (p.x < p1.x + (double)(p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y))) {
            inside = !inside;
        }
    }
    return inside;
}


int main() {
    fast_io();

    int N;
    std::cin >> N;
    std::vector<Point> mackerels(N), sardines(N);
    for (int i = 0; i < N; ++i) std::cin >> mackerels[i].x >> mackerels[i].y;
    for (int i = 0; i < N; ++i) std::cin >> sardines[i].x >> sardines[i].y;

    auto grid_scores_master = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    for (const auto& p : mackerels) {
        grid_scores_master[p.y / CELL_SIZE][p.x / CELL_SIZE]++;
    }
    for (const auto& p : sardines) {
        grid_scores_master[p.y / CELL_SIZE][p.x / CELL_SIZE]--;
    }

    long long best_total_score = -1e18;
    std::vector<Point> best_polygon;

    int K_RECTS = 5;
    for (int k = 0; k < K_RECTS; ++k) {
        auto grid_scores = grid_scores_master;
        
        long long max_rect_score = -1e18;
        int best_r1 = -1, best_c1 = -1, best_r2 = -1, best_c2 = -1;

        for (int c1 = 0; c1 < GRID_SIZE; ++c1) {
            std::vector<long long> col_sum(GRID_SIZE, 0);
            for (int c2 = c1; c2 < GRID_SIZE; ++c2) {
                for (int r = 0; r < GRID_SIZE; ++r) {
                    col_sum[r] += grid_scores[r][c2];
                }
                
                long long current_max = 0;
                long long overall_max = -1e18;
                int current_r1 = 0, best_r1_kadane = -1, best_r2_kadane = -1;

                for (int r = 0; r < GRID_SIZE; ++r) {
                    if (current_r1 > r) current_r1 = r; // Kadane start pos fix
                    current_max += col_sum[r];
                    if (current_max > overall_max) {
                        overall_max = current_max;
                        best_r1_kadane = current_r1;
                        best_r2_kadane = r;
                    }
                    if (current_max < 0) {
                        current_max = 0;
                        current_r1 = r + 1;
                    }
                }
                
                if (overall_max > max_rect_score) {
                    max_rect_score = overall_max;
                    best_r1 = best_r1_kadane;
                    best_r2 = best_r2_kadane;
                    best_c1 = c1;
                    best_c2 = c2;
                }
            }
        }
        
        if (best_r1 == -1) break;
        
        if (k < K_RECTS - 1) {
             for(int r = best_r1; r <= best_r2; ++r) {
                for (int c = best_c1; c <= best_c2; ++c) {
                    grid_scores_master[r][c] = -1e9;
                }
            }
        }

        std::vector<std::vector<bool>> in_C(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        for (int r = best_r1; r <= best_r2; ++r) {
            for (int c = best_c1; c <= best_c2; ++c) {
                in_C[r][c] = true;
            }
        }
        
        using State = std::pair<int, std::pair<int, int>>;
        std::priority_queue<State> pq;
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int r = best_r1; r <= best_r2; ++r) {
            for (int c = best_c1; c <= best_c2; ++c) {
                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE && !in_C[nr][nc]) {
                         pq.push({grid_scores[nr][nc], {nr, nc}});
                    }
                }
            }
        }

        while(!pq.empty()){
            auto top = pq.top();
            pq.pop();
            int score = top.first;
            int r = top.second.first;
            int c = top.second.second;

            if (score <= 0) break;
            if (in_C[r][c]) continue;

            in_C[r][c] = true;

            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE && !in_C[nr][nc]) {
                    pq.push({grid_scores[nr][nc], {nr, nc}});
                }
            }
        }

        std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        std::queue<std::pair<int,int>> q;
        for(int r=0; r<GRID_SIZE; ++r){
            if(!in_C[r][0] && !visited[r][0]) { q.push({r,0}); visited[r][0] = true; }
            if(!in_C[r][GRID_SIZE-1] && !visited[r][GRID_SIZE-1]) { q.push({r,GRID_SIZE-1}); visited[r][GRID_SIZE-1] = true; }
        }
        for(int c=0; c<GRID_SIZE; ++c){
            if(!in_C[0][c] && !visited[0][c]) { q.push({0,c}); visited[0][c] = true; }
            if(!in_C[GRID_SIZE-1][c] && !visited[GRID_SIZE-1][c]) { q.push({GRID_SIZE-1,c}); visited[GRID_SIZE-1][c] = true; }
        }

        while(!q.empty()){
            auto [r,c] = q.front(); q.pop();
            for(int i=0; i<4; ++i){
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE && !in_C[nr][nc] && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    q.push({nr,nc});
                }
            }
        }

        for(int r=0; r<GRID_SIZE; ++r) {
            for(int c=0; c<GRID_SIZE; ++c) {
                if(!in_C[r][c] && !visited[r][c]) {
                    in_C[r][c] = true;
                }
            }
        }
        
        std::map<Point, std::vector<Point>> adj;
        Point start_node = {-1, -1};

        for (int r = 0; r < GRID_SIZE; ++r) {
            for (int c = 0; c < GRID_SIZE; ++c) {
                if (!in_C[r][c]) continue;
                
                auto add_edge = [&](int r1, int c1, int r2, int c2) {
                    Point p1 = {std::min(MAX_COORD_VAL, c1 * CELL_SIZE), std::min(MAX_COORD_VAL, r1 * CELL_SIZE)};
                    Point p2 = {std::min(MAX_COORD_VAL, c2 * CELL_SIZE), std::min(MAX_COORD_VAL, r2 * CELL_SIZE)};
                    if (!(p1 == p2)) {
                        adj[p1].push_back(p2); adj[p2].push_back(p1);
                        if (start_node.x == -1) start_node = p1;
                    }
                };

                if (r == 0 || !in_C[r - 1][c]) add_edge(r, c, r, c + 1);
                if (r == GRID_SIZE - 1 || !in_C[r + 1][c]) add_edge(r + 1, c, r + 1, c + 1);
                if (c == 0 || !in_C[r][c - 1]) add_edge(r, c, r + 1, c);
                if (c == GRID_SIZE - 1 || !in_C[r][c + 1]) add_edge(r, c + 1, r + 1, c + 1);
            }
        }
        
        if (start_node.x == -1) continue;

        std::vector<Point> polygon;
        std::set<Point> visited_poly_nodes;
        Point curr = start_node;
        Point prev = {-1, -1};

        while(visited_poly_nodes.find(curr) == visited_poly_nodes.end()) {
            polygon.push_back(curr);
            visited_poly_nodes.insert(curr);
            
            bool found_next = false;
            for (Point& next : adj[curr]) {
                if (!(next == prev)) {
                    prev = curr;
                    curr = next;
                    found_next = true;
                    break;
                }
            }
            if (!found_next) break;
        }
        
        std::vector<Point> simplified_polygon;
        if(polygon.size() > 0) {
            simplified_polygon.push_back(polygon[0]);
            for (size_t i = 1; i < polygon.size(); ++i) {
                Point& p1 = simplified_polygon.back();
                Point& p2 = polygon[i];
                if(simplified_polygon.size() > 1){
                    Point& p0 = simplified_polygon[simplified_polygon.size()-2];
                    if((p0.x == p1.x && p1.x == p2.x) || (p0.y == p1.y && p1.y == p2.y)) {
                        simplified_polygon.back() = p2;
                    } else {
                        simplified_polygon.push_back(p2);
                    }
                } else {
                    simplified_polygon.push_back(p2);
                }
            }
            if (simplified_polygon.size() > 2) {
                Point p_last = simplified_polygon.back();
                Point p_first = simplified_polygon.front();
                Point p_sec_last = simplified_polygon[simplified_polygon.size()-2];
                if ((p_sec_last.x == p_last.x && p_last.x == p_first.x) || (p_sec_last.y == p_last.y && p_last.y == p_first.y)) {
                    simplified_polygon.pop_back();
                }
            }
             if (simplified_polygon.size() > 2) {
                Point p_last = simplified_polygon.back();
                Point p_first = simplified_polygon.front();
                Point p_second = simplified_polygon[1];
                 if ((p_last.x == p_first.x && p_first.x == p_second.x) || (p_last.y == p_first.y && p_first.y == p_second.y)) {
                    simplified_polygon.front() = p_last;
                    simplified_polygon.pop_back();
                }
            }
        }
        
        long long perimeter = 0;
        if(simplified_polygon.size() > 1) {
            for (size_t i = 0; i < simplified_polygon.size(); ++i) {
                Point p1 = simplified_polygon[i];
                Point p2 = simplified_polygon[(i + 1) % simplified_polygon.size()];
                perimeter += std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
            }
        }
        
        if (simplified_polygon.size() >= 4 && simplified_polygon.size() <= 1000 && perimeter <= 400000) {
            long long current_score = 0;
            for (const auto& p : mackerels) {
                if (is_inside(p, simplified_polygon)) current_score++;
            }
            for (const auto& p : sardines) {
                if (is_inside(p, simplified_polygon)) current_score--;
            }

            if (current_score > best_total_score) {
                best_total_score = current_score;
                best_polygon = simplified_polygon;
            }
        }
    }
    
    if (best_polygon.empty()){
      best_polygon.push_back({0,0});
      best_polygon.push_back({1,0});
      best_polygon.push_back({1,1});
      best_polygon.push_back({0,1});
    }

    std::cout << best_polygon.size() << "\n";
    for (const auto& p : best_polygon) {
        std::cout << p.x << " " << p.y << "\n";
    }

    return 0;
}