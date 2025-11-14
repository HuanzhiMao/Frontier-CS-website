#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<pair<int, int>> mackerel(N), sardine(N);
    
    for (int i = 0; i < N; i++) {
        cin >> mackerel[i].first >> mackerel[i].second;
    }
    
    for (int i = 0; i < N; i++) {
        cin >> sardine[i].first >> sardine[i].second;
    }
    
    int best_score = 0;
    vector<pair<int, int>> best_polygon;
    
    // Try different rectangle sizes and positions
    for (int step = 0; step < 10000; step++) {
        int min_x = rand() % 100000;
        int min_y = rand() % 100000;
        int max_x = min(100000, min_x + rand() % (100000 - min_x + 1));
        int max_y = min(100000, min_y + rand() % (100000 - min_y + 1));
        
        if (max_x <= min_x || max_y <= min_y) continue;
        
        int perimeter = 2 * (max_x - min_x) + 2 * (max_y - min_y);
        if (perimeter > 400000) continue;
        
        int m_count = 0, s_count = 0;
        
        for (int i = 0; i < N; i++) {
            if (mackerel[i].first >= min_x && mackerel[i].first <= max_x && 
                mackerel[i].second >= min_y && mackerel[i].second <= max_y) {
                m_count++;
            }
        }
        
        for (int i = 0; i < N; i++) {
            if (sardine[i].first >= min_x && sardine[i].first <= max_x && 
                sardine[i].second >= min_y && sardine[i].second <= max_y) {
                s_count++;
            }
        }
        
        int score = max(0, m_count - s_count + 1);
        
        if (score > best_score) {
            best_score = score;
            best_polygon.clear();
            best_polygon.push_back({min_x, min_y});
            best_polygon.push_back({max_x, min_y});
            best_polygon.push_back({max_x, max_y});
            best_polygon.push_back({min_x, max_y});
        }
    }
    
    // If no good rectangle found, use a simple one
    if (best_polygon.empty()) {
        best_polygon.push_back({0, 0});
        best_polygon.push_back({1000, 0});
        best_polygon.push_back({1000, 1000});
        best_polygon.push_back({0, 1000});
    }
    
    cout << best_polygon.size() << endl;
    for (auto& p : best_polygon) {
        cout << p.first << " " << p.second << endl;
    }
    
    return 0;
}