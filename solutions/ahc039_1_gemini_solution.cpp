#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>

using namespace std;

const int MAX_COORD = 100000;

struct Point {
    int x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
     bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

long long cross_product(const Point& a, const Point& b) {
    return (long long)a.x * b.y - (long long)a.y * b.x;
}

long long dist_sq(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

struct Result {
    long long score = 0;
    vector<Point> polygon;
};

bool is_collinear(const Point& p1, const Point& p2, const Point& p3) {
    return (p1.x == p2.x && p2.x == p3.x) || (p1.y == p2.y && p2.y == p3.y);
}

vector<Point> simplify_polygon(const vector<Point>& poly) {
    if (poly.size() <= 3) return poly;
    vector<Point> simplified;
    for (size_t i = 0; i < poly.size(); ++i) {
        Point p_prev = poly[(i + poly.size() - 1) % poly.size()];
        Point p_curr = poly[i];
        Point p_next = poly[(i + 1) % poly.size()];
        if (!is_collinear(p_prev, p_curr, p_next)) {
            simplified.push_back(p_curr);
        }
    }
    return simplified;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    vector<Point> mackerels(N), sardines(N);
    for (int i = 0; i < N; ++i) cin >> mackerels[i].x >> mackerels[i].y;
    for (int i = 0; i < N; ++i) cin >> sardines[i].x >> sardines[i].y;

    Result best_result;

    vector<int> cell_sizes = {250, 400, 500, 800, 1000};
    
    for (int C : cell_sizes) {
        for (int i = 0; i < 4; ++i) {
            int ox = (i % 2) * (C / 2);
            int oy = (i / 2) * (C / 2);

            map<pair<int, int>, int> grid_scores;
            for (const auto& p : mackerels) {
                grid_scores[{(p.x + ox) / C, (p.y + oy) / C}]++;
            }
            for (const auto& p : sardines) {
                grid_scores[{(p.x + ox) / C, (p.y + oy) / C}]--;
            }
            
            map<pair<int, int>, bool> visited;
            
            for(auto const& [cell, score] : grid_scores){
                if (score > 0 && !visited[cell]) {
                    long long current_component_score = 0;
                    set<pair<int, int>> component_cells;
                    queue<pair<int, int>> q;

                    q.push(cell);
                    visited[cell] = true;

                    while (!q.empty()) {
                        pair<int, int> curr = q.front();
                        q.pop();

                        component_cells.insert(curr);
                        current_component_score += grid_scores[curr];

                        int dx[] = {0, 0, 1, -1};
                        int dy[] = {1, -1, 0, 0};

                        for (int k = 0; k < 4; ++k) {
                            pair<int, int> next = {curr.first + dx[k], curr.second + dy[k]};
                            if (grid_scores.count(next) && grid_scores.at(next) > 0 && !visited[next]) {
                                visited[next] = true;
                                q.push(next);
                            }
                        }
                    }

                    if (current_component_score > best_result.score) {
                        set<pair<Point, Point>> boundary_edges;
                        
                        for (const auto& c : component_cells) {
                            int cx = c.first, cy = c.second;
                            Point p_bl = {cx * C, cy * C}, p_br = {(cx + 1) * C, cy * C};
                            Point p_tl = {cx * C, (cy + 1) * C}, p_tr = {(cx + 1) * C, (cy + 1) * C};

                            if (component_cells.find({cx, cy + 1}) == component_cells.end()) {
                                Point p1 = p_tl, p2 = p_tr; if (p2 < p1) swap(p1, p2); boundary_edges.insert({p1, p2});
                            }
                            if (component_cells.find({cx, cy - 1}) == component_cells.end()) {
                                Point p1 = p_bl, p2 = p_br; if (p2 < p1) swap(p1, p2); boundary_edges.insert({p1, p2});
                            }
                            if (component_cells.find({cx - 1, cy}) == component_cells.end()) {
                                Point p1 = p_bl, p2 = p_tl; if (p2 < p1) swap(p1, p2); boundary_edges.insert({p1, p2});
                            }
                            if (component_cells.find({cx + 1, cy}) == component_cells.end()) {
                                Point p1 = p_br, p2 = p_tr; if (p2 < p1) swap(p1, p2); boundary_edges.insert({p1, p2});
                            }
                        }

                        map<Point, vector<Point>> adj;
                        for (const auto& edge : boundary_edges) {
                            adj[edge.first].push_back(edge.second);
                            adj[edge.second].push_back(edge.first);
                        }

                        if (adj.empty()) continue;

                        set<Point> used_vertices;
                        vector<vector<Point>> loops;

                        for (auto const& [start_node, _] : adj) {
                            if (used_vertices.find(start_node) == used_vertices.end()) {
                                vector<Point> current_loop;
                                Point current_v = start_node;
                                Point prev_v = {start_node.x - 1, start_node.y}; 

                                do {
                                    used_vertices.insert(current_v);
                                    current_loop.push_back(current_v);
                                    
                                    Point next_v;
                                    long long best_cross_prod = -2e18; // Smallest possible
                                    Point v_in = current_v - prev_v;
                                    
                                    for(const auto& neighbor : adj[current_v]){
                                        if(neighbor == prev_v && adj[current_v].size() > 1) continue;
                                        Point v_out = neighbor - current_v;
                                        long long cp = cross_product(v_in, v_out);
                                        if (cp > best_cross_prod) {
                                            best_cross_prod = cp;
                                            next_v = neighbor;
                                        } else if (cp == best_cross_prod) {
                                            if (dist_sq(current_v, neighbor) > dist_sq(current_v, next_v)) {
                                                next_v = neighbor;
                                            }
                                        }
                                    }
                                    prev_v = current_v;
                                    current_v = next_v;
                                } while (current_v != start_node);
                                loops.push_back(current_loop);
                            }
                        }
                        
                        long long max_area = 0;
                        vector<Point> outer_loop;
                        for(auto& loop : loops) {
                            long long current_area = 0;
                            for(size_t j=0; j<loop.size(); ++j) {
                                current_area += cross_product(loop[j], loop[(j+1)%loop.size()]);
                            }
                            if(abs(current_area) > abs(max_area)) {
                                max_area = current_area;
                                outer_loop = loop;
                            }
                        }
                        
                        if (max_area < 0) {
                            reverse(outer_loop.begin(), outer_loop.end());
                        }

                        vector<Point> final_poly;
                        bool valid_coords = true;
                        
                        for(size_t j=0; j<outer_loop.size(); ++j){
                            Point p1_shifted = outer_loop[j];
                            Point p1 = {p1_shifted.x - ox, p1_shifted.y - oy};

                            if (p1.x < 0 || p1.x > MAX_COORD || p1.y < 0 || p1.y > MAX_COORD) {
                                valid_coords = false;
                                break;
                            }
                            final_poly.push_back(p1);
                        }

                        if (!valid_coords) continue;
                        
                        vector<Point> simplified = simplify_polygon(final_poly);

                        if (simplified.size() > 1000 || simplified.size() < 3) continue;
                        
                        long long perimeter = 0;
                         for(size_t j=0; j<simplified.size(); ++j){
                            Point p1 = simplified[j];
                            Point p2 = simplified[(j+1)%simplified.size()];
                            perimeter += abs(p1.x - p2.x) + abs(p1.y - p2.y);
                        }

                        if(perimeter > 400000) continue;
                        
                        best_result.score = current_component_score;
                        best_result.polygon = simplified;
                    }
                }
            }
        }
    }

    if (best_result.score == 0) {
        cout << 4 << endl;
        cout << "0 0" << endl;
        cout << "1 0" << endl;
        cout << "1 1" << endl;
        cout << "0 1" << endl;
    } else {
        cout << best_result.polygon.size() << endl;
        for (const auto& p : best_result.polygon) {
            cout << p.x << " " << p.y << endl;
        }
    }

    return 0;
}