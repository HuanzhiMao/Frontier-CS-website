#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    int type; // 0 for mackerel, 1 for sardine
};

bool isInside(int px, int py, const vector<pair<int, int>>& polygon) {
    int n = polygon.size();
    bool inside = false;
    
    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = polygon[i].first, yi = polygon[i].second;
        int xj = polygon[j].first, yj = polygon[j].second;
        
        if (((yi > py) != (yj > py)) && (px < (xj - xi) * (py - yi) / (yj - yi) + xi)) {
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
    
    vector<Point> points(2 * N);
    
    for (int i = 0; i < 2 * N; i++) {
        cin >> points[i].x >> points[i].y;
        points[i].type = (i < N) ? 0 : 1; // 0-N-1: mackerel, N-2N-1: sardine
    }
    
    // Find bounding box of mackerels only
    int min_x = 100000, max_x = 0, min_y = 100000, max_y = 0;
    for (int i = 0; i < N; i++) {
        min_x = min(min_x, points[i].x);
        max_x = max(max_x, points[i].x);
        min_y = min(min_y, points[i].y);
        max_y = max(max_y, points[i].y);
    }
    
    // Add some margin
    min_x = max(0, min_x - 100);
    max_x = min(100000, max_x + 100);
    min_y = max(0, min_y - 100);
    max_y = min(100000, max_y + 100);
    
    int best_score = 0;
    vector<pair<int, int>> best_polygon;
    
    // Try different rectangles
    for (int margin = 0; margin <= 2000; margin += 100) {
        int x1 = max(0, min_x - margin);
        int x2 = min(100000, max_x + margin);
        int y1 = max(0, min_y - margin);
        int y2 = min(100000, max_y + margin);
        
        vector<pair<int, int>> polygon = {
            {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}
        };
        
        int mackerel_count = 0, sardine_count = 0;
        
        for (int i = 0; i < 2 * N; i++) {
            if (points[i].x >= x1 && points[i].x <= x2 && 
                points[i].y >= y1 && points[i].y <= y2) {
                if (points[i].type == 0) mackerel_count++;
                else sardine_count++;
            }
        }
        
        int score = max(0, mackerel_count - sardine_count + 1);
        if (score > best_score) {
            best_score = score;
            best_polygon = polygon;
        }
    }
    
    // Try some random rectangles around dense areas
    map<pair<int, int>, int> grid_count;
    int grid_size = 2000;
    
    for (int i = 0; i < N; i++) {
        int gx = points[i].x / grid_size;
        int gy = points[i].y / grid_size;
        grid_count[{gx, gy}]++;
    }
    
    vector<pair<int, pair<int, int>>> dense_areas;
    for (auto& p : grid_count) {
        if (p.second >= 5) {
            dense_areas.push_back({p.second, p.first});
        }
    }
    
    sort(dense_areas.rbegin(), dense_areas.rend());
    
    for (int i = 0; i < min(10, (int)dense_areas.size()); i++) {
        int gx = dense_areas[i].second.first;
        int gy = dense_areas[i].second.second;
        
        int cx = gx * grid_size + grid_size / 2;
        int cy = gy * grid_size + grid_size / 2;
        
        for (int size = 1000; size <= 5000; size += 500) {
            int x1 = max(0, cx - size);
            int x2 = min(100000, cx + size);
            int y1 = max(0, cy - size);
            int y2 = min(100000, cy + size);
            
            vector<pair<int, int>> polygon = {
                {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}
            };
            
            int mackerel_count = 0, sardine_count = 0;
            
            for (int j = 0; j < 2 * N; j++) {
                if (points[j].x >= x1 && points[j].x <= x2 && 
                    points[j].y >= y1 && points[j].y <= y2) {
                    if (points[j].type == 0) mackerel_count++;
                    else sardine_count++;
                }
            }
            
            int score = max(0, mackerel_count - sardine_count + 1);
            if (score > best_score) {
                best_score = score;
                best_polygon = polygon;
            }
        }
    }
    
    cout << best_polygon.size() << "\n";
    for (auto& p : best_polygon) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}