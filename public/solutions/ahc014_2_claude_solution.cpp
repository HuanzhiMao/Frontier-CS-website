#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    set<pair<int, int>> dots;
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        dots.insert({x, y});
    }
    
    vector<tuple<int, int, int, int, int, int, int, int>> operations;
    set<vector<pair<int, int>>> used_rectangles;
    
    double c = (N - 1) / 2.0;
    
    auto weight = [&](int x, int y) {
        return (x - c) * (x - c) + (y - c) * (y - c) + 1;
    };
    
    auto is_valid_rectangle = [](int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
        vector<pair<int, int>> points = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
        
        // Check if it forms a rectangle (axis-aligned or 45-degree)
        vector<int> dx = {x2-x1, x3-x2, x4-x3, x1-x4};
        vector<int> dy = {y2-y1, y3-y2, y4-y3, y1-y4};
        
        // Check if opposite sides are equal and adjacent sides are perpendicular
        if (dx[0] != -dx[2] || dy[0] != -dy[2] || dx[1] != -dx[3] || dy[1] != -dy[3]) return false;
        if (dx[0]*dx[1] + dy[0]*dy[1] != 0) return false;
        
        return true;
    };
    
    auto get_perimeter_points = [](int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
        set<pair<int, int>> perimeter;
        
        auto add_line = [&](int xa, int ya, int xb, int yb) {
            int dx = xb - xa, dy = yb - ya;
            int g = __gcd(abs(dx), abs(dy));
            if (g == 0) g = 1;
            dx /= g; dy /= g;
            
            int x = xa, y = ya;
            while (x != xb || y != yb) {
                perimeter.insert({x, y});
                x += dx; y += dy;
            }
            perimeter.insert({xb, yb});
        };
        
        add_line(x1, y1, x2, y2);
        add_line(x2, y2, x3, y3);
        add_line(x3, y3, x4, y4);
        add_line(x4, y4, x1, y1);
        
        return perimeter;
    };
    
    while (true) {
        bool found = false;
        double best_score = -1;
        tuple<int, int, int, int, int, int, int, int> best_op;
        
        for (int x1 = 0; x1 < N && !found; x1++) {
            for (int y1 = 0; y1 < N && !found; y1++) {
                if (dots.count({x1, y1})) continue;
                
                vector<pair<int, int>> dot_list(dots.begin(), dots.end());
                
                for (int i = 0; i < dot_list.size() && !found; i++) {
                    for (int j = i + 1; j < dot_list.size() && !found; j++) {
                        for (int k = j + 1; k < dot_list.size(); k++) {
                            int x2 = dot_list[i].first, y2 = dot_list[i].second;
                            int x3 = dot_list[j].first, y3 = dot_list[j].second;
                            int x4 = dot_list[k].first, y4 = dot_list[k].second;
                            
                            if (!is_valid_rectangle(x1, y1, x2, y2, x3, y3, x4, y4)) continue;
                            
                            auto perimeter = get_perimeter_points(x1, y1, x2, y2, x3, y3, x4, y4);
                            
                            // Check condition 2: no other dots on perimeter
                            bool valid = true;
                            for (auto [px, py] : perimeter) {
                                if (dots.count({px, py}) && make_pair(px, py) != make_pair(x2, y2) && 
                                    make_pair(px, py) != make_pair(x3, y3) && make_pair(px, py) != make_pair(x4, y4)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (!valid) continue;
                            
                            // Check condition 3: no overlap with existing rectangles
                            vector<pair<int, int>> rect_points = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
                            sort(rect_points.begin(), rect_points.end());
                            if (used_rectangles.count(rect_points)) continue;
                            
                            double score = weight(x1, y1);
                            if (score > best_score) {
                                best_score = score;
                                best_op = {x1, y1, x2, y2, x3, y3, x4, y4};
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        if (!found) break;
        
        auto [x1, y1, x2, y2, x3, y3, x4, y4] = best_op;
        operations.push_back(best_op);
        dots.insert({x1, y1});
        
        vector<pair<int, int>> rect_points = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
        sort(rect_points.begin(), rect_points.end());
        used_rectangles.insert(rect_points);
    }
    
    cout << operations.size() << '\n';
    for (auto [x1, y1, x2, y2, x3, y3, x4, y4] : operations) {
        cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << ' ' << x3 << ' ' << y3 << ' ' << x4 << ' ' << y4 << '\n';
    }
    
    return 0;
}