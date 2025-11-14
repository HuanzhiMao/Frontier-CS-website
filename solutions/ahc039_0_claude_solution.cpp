#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

bool isInsidePolygon(const vector<Point>& polygon, const Point& p) {
    int n = polygon.size();
    bool inside = false;
    
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    
    return inside;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<Point> mackerels(N), sardines(N);
    
    for (int i = 0; i < N; i++) {
        cin >> mackerels[i].x >> mackerels[i].y;
    }
    
    for (int i = 0; i < N; i++) {
        cin >> sardines[i].x >> sardines[i].y;
    }
    
    // Find bounding box of all mackerels
    int min_x = 100000, max_x = 0, min_y = 100000, max_y = 0;
    for (const auto& m : mackerels) {
        min_x = min(min_x, m.x);
        max_x = max(max_x, m.x);
        min_y = min(min_y, m.y);
        max_y = max(max_y, m.y);
    }
    
    int best_score = 0;
    vector<Point> best_polygon;
    
    // Try different rectangular regions around mackerel clusters
    for (int iterations = 0; iterations < 1000; iterations++) {
        // Select random subset of mackerels to focus on
        vector<int> indices(N);
        iota(indices.begin(), indices.end(), 0);
        random_shuffle(indices.begin(), indices.end());
        
        int cluster_size = min(N, 200 + rand() % 500);
        
        int c_min_x = 100000, c_max_x = 0, c_min_y = 100000, c_max_y = 0;
        for (int i = 0; i < cluster_size; i++) {
            const auto& m = mackerels[indices[i]];
            c_min_x = min(c_min_x, m.x);
            c_max_x = max(c_max_x, m.x);
            c_min_y = min(c_min_y, m.y);
            c_max_y = max(c_max_y, m.y);
        }
        
        // Expand the bounding box slightly
        int expand = 500 + rand() % 2000;
        c_min_x = max(0, c_min_x - expand);
        c_max_x = min(100000, c_max_x + expand);
        c_min_y = max(0, c_min_y - expand);
        c_max_y = min(100000, c_max_y + expand);
        
        vector<Point> polygon = {
            Point(c_min_x, c_min_y),
            Point(c_max_x, c_min_y),
            Point(c_max_x, c_max_y),
            Point(c_min_x, c_max_y)
        };
        
        int mackerel_count = 0, sardine_count = 0;
        
        for (const auto& m : mackerels) {
            if (m.x >= c_min_x && m.x <= c_max_x && m.y >= c_min_y && m.y <= c_max_y) {
                mackerel_count++;
            }
        }
        
        for (const auto& s : sardines) {
            if (s.x >= c_min_x && s.x <= c_max_x && s.y >= c_min_y && s.y <= c_max_y) {
                sardine_count++;
            }
        }
        
        int score = max(0, mackerel_count - sardine_count + 1);
        
        if (score > best_score) {
            best_score = score;
            best_polygon = polygon;
        }
    }
    
    // If no good polygon found, use a simple rectangle around all mackerels
    if (best_polygon.empty()) {
        best_polygon = {
            Point(max(0, min_x - 100), max(0, min_y - 100)),
            Point(min(100000, max_x + 100), max(0, min_y - 100)),
            Point(min(100000, max_x + 100), min(100000, max_y + 100)),
            Point(max(0, min_x - 100), min(100000, max_y + 100))
        };
    }
    
    cout << best_polygon.size() << "\n";
    for (const auto& p : best_polygon) {
        cout << p.x << " " << p.y << "\n";
    }
    
    return 0;
}