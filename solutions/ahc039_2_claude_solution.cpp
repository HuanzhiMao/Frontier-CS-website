#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<pair<int, int>> mackerels(N), sardines(N);
    
    for (int i = 0; i < N; i++) {
        cin >> mackerels[i].first >> mackerels[i].second;
    }
    
    for (int i = 0; i < N; i++) {
        cin >> sardines[i].first >> sardines[i].second;
    }
    
    // Find the bounding box of mackerels
    int min_x = 100000, max_x = 0, min_y = 100000, max_y = 0;
    for (auto& m : mackerels) {
        min_x = min(min_x, m.first);
        max_x = max(max_x, m.first);
        min_y = min(min_y, m.second);
        max_y = max(max_y, m.second);
    }
    
    // Try different rectangle sizes and positions to maximize score
    int best_score = 0;
    vector<pair<int, int>> best_polygon;
    
    // Try rectangles of different sizes
    for (int w = 1000; w <= 50000; w += 1000) {
        for (int h = 1000; h <= 50000; h += 1000) {
            if (2 * (w + h) > 400000) continue;
            
            // Try different positions
            for (int x = max(0, min_x - 5000); x <= min(100000 - w, max_x + 5000); x += 2000) {
                for (int y = max(0, min_y - 5000); y <= min(100000 - h, max_y + 5000); y += 2000) {
                    
                    int mackerel_count = 0, sardine_count = 0;
                    
                    // Count mackerels in rectangle
                    for (auto& m : mackerels) {
                        if (m.first >= x && m.first <= x + w && 
                            m.second >= y && m.second <= y + h) {
                            mackerel_count++;
                        }
                    }
                    
                    // Count sardines in rectangle
                    for (auto& s : sardines) {
                        if (s.first >= x && s.first <= x + w && 
                            s.second >= y && s.second <= y + h) {
                            sardine_count++;
                        }
                    }
                    
                    int score = max(0, mackerel_count - sardine_count + 1);
                    
                    if (score > best_score) {
                        best_score = score;
                        best_polygon = {
                            {x, y},
                            {x + w, y},
                            {x + w, y + h},
                            {x, y + h}
                        };
                    }
                }
            }
        }
    }
    
    // If no good rectangle found, use a simple one
    if (best_polygon.empty()) {
        best_polygon = {
            {0, 0},
            {10000, 0},
            {10000, 10000},
            {0, 10000}
        };
    }
    
    cout << best_polygon.size() << "\n";
    for (auto& p : best_polygon) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}