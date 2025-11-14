#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <chrono>
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
};

struct Polygon {
    vector<Point> vertices;
};

bool is_inside(const Point& p, const Polygon& poly) {
    if (poly.vertices.empty()) return false;

    for (size_t i = 0; i < poly.vertices.size(); ++i) {
        Point p1 = poly.vertices[i];
        Point p2 = poly.vertices[(i + 1) % poly.vertices.size()];
        if (p1.x == p2.x && p.x == p1.x) {
            if (min(p1.y, p2.y) <= p.y && p.y <= max(p1.y, p2.y)) return true;
        }
        if (p1.y == p2.y && p.y == p1.y) {
            if (min(p1.x, p2.x) <= p.x && p.x <= max(p1.x, p2.x)) return true;
        }
    }

    int wn = 0;
    for (size_t i = 0; i < poly.vertices.size(); ++i) {
        Point p1 = poly.vertices[i];
        Point p2 = poly.vertices[(i + 1) % poly.vertices.size()];
        if (p1.y <= p.y) {
            if (p2.y > p.y && (long long)(p2.x - p1.x) * (p.y - p1.y) - (long long)(p2.y - p1.y) * (p.x - p1.x) > 0) {
                wn++;
            }
        } else {
            if (p2.y <= p.y && (long long)(p2.x - p1.x) * (p.y - p1.y) - (long long)(p2.y - p1.y) * (p.x - p1.x) < 0) {
                wn--;
            }
        }
    }
    return wn != 0;
}

int calculate_a_b_score(const Polygon& poly, const vector<Point>& mackerels, const vector<Point>& sardines) {
    int a = 0, b = 0;
    for (const auto& fish : mackerels) {
        if (is_inside(fish, poly)) {
            a++;
        }
    }
    for (const auto& fish : sardines) {
        if (is_inside(fish, poly)) {
            b++;
        }
    }
    return a - b;
}

chrono::high_resolution_clock::time_point start_time;
bool time_limit_exceeded() {
    auto now = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    return duration > 2800;
}

Polygon solve(const vector<Point>& mackerels, const vector<Point>& sardines) {
    Polygon best_poly;
    int best_score = -1e9;

    vector<int> grid_sizes = {300, 400, 500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000};
    vector<double> offset_factors = {0.0, 0.25, 0.5, 0.75};

    int min_x = MAX_COORD, max_x = 0, min_y = MAX_COORD, max_y = 0;
    for (const auto& p : mackerels) {
        min_x = min(min_x, p.x); max_x = max(max_x, p.x);
        min_y = min(min_y, p.y); max_y = max(max_y, p.y);
    }
    for (const auto& p : sardines) {
        min_x = min(min_x, p.x); max_x = max(max_x, p.x);
        min_y = min(min_y, p.y); max_y = max(max_y, p.y);
    }
    
    int margin = 2000;
    min_x = max(0, min_x - margin);
    max_x = min(MAX_COORD, max_x + margin);
    min_y = max(0, min_y - margin);
    max_y = min(MAX_COORD, max_y + margin);

    for (int S : grid_sizes) {
        if (time_limit_exceeded()) break;
        for (double ox_f : offset_factors) {
            if (time_limit_exceeded()) break;
            for (double oy_f : offset_factors) {
                if (time_limit_exceeded()) break;
                int ox = S * ox_f;
                int oy = S * oy_f;

                map<pair<int, int>, int> grid_scores;
                for (const auto& p : mackerels) {
                    grid_scores[{(p.y - min_y - oy) / S, (p.x - min_x - ox) / S}]++;
                }
                for (const auto& p : sardines) {
                    grid_scores[{(p.y - min_y - oy) / S, (p.x - min_x - ox) / S}]--;
                }

                map<pair<int, int>, bool> visited;
                vector<pair<int, set<pair<int, int>>>> components;

                for (auto const& [pos, score] : grid_scores) {
                    if (score > 0 && !visited[pos]) {
                        set<pair<int, int>> current_component;
                        queue<pair<int, int>> q;
                        int component_score = 0;
                        q.push(pos);
                        visited[pos] = true;
                        
                        while (!q.empty()) {
                            pair<int, int> curr = q.front();
                            q.pop();
                            current_component.insert(curr);
                            component_score += grid_scores.count(curr) ? grid_scores[curr] : 0;
                            
                            int dr[] = {-1, 1, 0, 0};
                            int dc[] = {0, 0, -1, 1};
                            for (int i = 0; i < 4; ++i) {
                                pair<int, int> next = {curr.first + dr[i], curr.second + dc[i]};
                                if (grid_scores.count(next) && grid_scores[next] > 0 && !visited[next]) {
                                    visited[next] = true;
                                    q.push(next);
                                }
                            }
                        }
                        if (component_score > 0) {
                            components.push_back({component_score, current_component});
                        }
                    }
                }
                
                sort(components.rbegin(), components.rend());

                for(size_t i = 0; i < min((size_t)5, components.size()); ++i) {
                    if (time_limit_exceeded()) break;
                    auto const& component_cells = components[i].second;

                    map<Point, vector<Point>> adj;
                    int max_r= -1e9, max_c= -1e9, min_r=1e9, min_c=1e9;
                    for(auto cell : component_cells){
                        min_r = min(min_r, cell.first); max_r = max(max_r, cell.first);
                        min_c = min(min_c, cell.second); max_c = max(max_c, cell.second);
                    }

                    for(int r = min_r - 1; r <= max_r + 1; ++r){
                        for(int c = min_c - 1; c <= max_c + 1; ++c){
                            if((component_cells.count({r-1, c}) > 0) != (component_cells.count({r, c}) > 0)){
                                Point p1 = {c * S + ox + min_x, r * S + oy + min_y};
                                Point p2 = {(c + 1) * S + ox + min_x, r * S + oy + min_y};
                                adj[p1].push_back(p2); adj[p2].push_back(p1);
                            }
                            if((component_cells.count({r, c-1}) > 0) != (component_cells.count({r, c}) > 0)){
                                Point p1 = {c * S + ox + min_x, r * S + oy + min_y};
                                Point p2 = {c * S + ox + min_x, (r + 1) * S + oy + min_y};
                                adj[p1].push_back(p2); adj[p2].push_back(p1);
                            }
                        }
                    }
                    if (adj.empty()) continue;

                    set<Point> visited_nodes;
                    vector<Polygon> loops;
                    for(auto const& [start_node, _] : adj){
                        if(visited_nodes.find(start_node) == visited_nodes.end()){
                            Polygon loop;
                            Point curr = start_node;
                            Point prev = {-1,-1};
                            while(visited_nodes.find(curr) == visited_nodes.end()){
                                loop.vertices.push_back(curr);
                                visited_nodes.insert(curr);
                                bool found_next = false;
                                for(Point next : adj[curr]){
                                    if(!(next == prev)){
                                        prev = curr;
                                        curr = next;
                                        found_next = true;
                                        break;
                                    }
                                }
                                if (!found_next) break; 
                            }
                            if (loop.vertices.size() >= 3) loops.push_back(loop);
                        }
                    }
                    
                    Polygon best_loop;
                    long long max_area = -1;
                    for(auto& loop : loops){
                        long long area = 0;
                        for(size_t j=0; j < loop.vertices.size(); ++j){
                            Point p1 = loop.vertices[j];
                            Point p2 = loop.vertices[(j+1)%loop.vertices.size()];
                            area += (long long)p1.x * p2.y - (long long)p2.x * p1.y;
                        }
                        if (abs(area) > max_area) {
                           max_area = abs(area);
                           if (area < 0) reverse(loop.vertices.begin(), loop.vertices.end());
                           best_loop = loop;
                        }
                    }
                    if (best_loop.vertices.empty()) continue;
                    
                    Polygon simplified_poly;
                    if(best_loop.vertices.size() > 1) {
                        simplified_poly.vertices.push_back(best_loop.vertices[0]);
                        for(size_t j = 1; j < best_loop.vertices.size(); ++j){
                            if(simplified_poly.vertices.size() < 2){
                                simplified_poly.vertices.push_back(best_loop.vertices[j]);
                                continue;
                            }
                            Point p_prev = simplified_poly.vertices[simplified_poly.vertices.size()-2];
                            Point p_mid = simplified_poly.vertices.back();
                            Point p_curr = best_loop.vertices[j];
                            if((p_prev.x == p_mid.x && p_mid.x == p_curr.x) || (p_prev.y == p_mid.y && p_mid.y == p_curr.y)){
                                simplified_poly.vertices.back() = p_curr;
                            } else {
                                simplified_poly.vertices.push_back(p_curr);
                            }
                        }
                        if (simplified_poly.vertices.size() >= 3) {
                             Point p_last = simplified_poly.vertices.back();
                             Point p_first = simplified_poly.vertices[0];
                             Point p_second = simplified_poly.vertices[1];
                             if((p_last.x == p_first.x && p_first.x == p_second.x) || (p_last.y == p_first.y && p_first.y == p_second.y)){
                                simplified_poly.vertices[0] = simplified_poly.vertices.back();
                                simplified_poly.vertices.pop_back();
                            }
                        }
                    }
                    
                    if (simplified_poly.vertices.size() < 4 || simplified_poly.vertices.size() > 1000) continue;
                    
                    long long perimeter = 0;
                    for(size_t j=0; j < simplified_poly.vertices.size(); ++j){
                        Point p1 = simplified_poly.vertices[j];
                        Point p2 = simplified_poly.vertices[(j+1)%simplified_poly.vertices.size()];
                        perimeter += abs(p1.x - p2.x) + abs(p1.y - p2.y);
                    }
                    if(perimeter > 400000) continue;

                    int current_score = calculate_a_b_score(simplified_poly, mackerels, sardines);
                    if (current_score > best_score) {
                        best_score = current_score;
                        best_poly = simplified_poly;
                    }
                }
            }
        }
    }

    if (best_score <= 0) {
        int best_mackerel_score = -1e9;
        for(const auto& m : mackerels){
            Polygon box;
            box.vertices.push_back({m.x - 1, m.y - 1});
            box.vertices.push_back({m.x + 1, m.y - 1});
            box.vertices.push_back({m.x + 1, m.y + 1});
            box.vertices.push_back({m.x - 1, m.y + 1});
            for (auto& p : box.vertices) {
                p.x = max(0, min(MAX_COORD, p.x));
                p.y = max(0, min(MAX_COORD, p.y));
            }
            int score = calculate_a_b_score(box, mackerels, sardines);
            if(score > best_mackerel_score){
                best_mackerel_score = score;
                best_poly = box;
                best_score = score;
            }
        }
    }
    
    return best_poly;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    start_time = chrono::high_resolution_clock::now();
    int N_val;
    cin >> N_val;
    vector<Point> mackerels(N_val), sardines(N_val);
    for (int i = 0; i < N_val; ++i) cin >> mackerels[i].x >> mackerels[i].y;
    for (int i = 0; i < N_val; ++i) cin >> sardines[i].x >> sardines[i].y;
    Polygon result_poly = solve(mackerels, sardines);
    cout << result_poly.vertices.size() << endl;
    for (const auto& p : result_poly.vertices) {
        cout << p.x << " " << p.y << endl;
    }
    return 0;
}