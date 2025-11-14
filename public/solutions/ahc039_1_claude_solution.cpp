#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<pair<int, int>> mackerel(N), sardine(N);
    
    for (int i = 0; i < N; i++) {
        cin >> mackerel[i].first >> mackerel[i].second;
    }
    
    for (int i = 0; i < N; i++) {
        cin >> sardine[i].first >> sardine[i].second;
    }
    
    // Simple greedy approach: find a rectangle that maximizes mackerel - sardine
    int best_score = 0;
    vector<pair<int, int>> best_polygon;
    
    // Get all unique x and y coordinates
    set<int> x_coords, y_coords;
    for (int i = 0; i < N; i++) {
        x_coords.insert(mackerel[i].first);
        y_coords.insert(mackerel[i].second);
        x_coords.insert(sardine[i].first);
        y_coords.insert(sardine[i].second);
    }
    
    vector<int> xs(x_coords.begin(), x_coords.end());
    vector<int> ys(y_coords.begin(), y_coords.end());
    
    // Try different rectangles
    for (int i = 0; i < min(50, (int)xs.size()); i++) {
        for (int j = i + 1; j < min(50, (int)xs.size()); j++) {
            for (int k = 0; k < min(50, (int)ys.size()); k++) {
                for (int l = k + 1; l < min(50, (int)ys.size()); l++) {
                    int x1 = xs[i], x2 = xs[j];
                    int y1 = ys[k], y2 = ys[l];
                    
                    if (x1 > x2) swap(x1, x2);
                    if (y1 > y2) swap(y1, y2);
                    
                    // Check perimeter constraint
                    int perimeter = 2 * (x2 - x1) + 2 * (y2 - y1);
                    if (perimeter > 400000) continue;
                    
                    int m_count = 0, s_count = 0;
                    
                    // Count mackerels inside rectangle
                    for (int p = 0; p < N; p++) {
                        if (mackerel[p].first >= x1 && mackerel[p].first <= x2 &&
                            mackerel[p].second >= y1 && mackerel[p].second <= y2) {
                            m_count++;
                        }
                    }
                    
                    // Count sardines inside rectangle
                    for (int p = 0; p < N; p++) {
                        if (sardine[p].first >= x1 && sardine[p].first <= x2 &&
                            sardine[p].second >= y1 && sardine[p].second <= y2) {
                            s_count++;
                        }
                    }
                    
                    int score = max(0, m_count - s_count + 1);
                    if (score > best_score) {
                        best_score = score;
                        best_polygon = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
                    }
                }
            }
        }
    }
    
    // If no good rectangle found, create a simple one
    if (best_polygon.empty()) {
        best_polygon = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};
    }
    
    cout << best_polygon.size() << "\n";
    for (auto& p : best_polygon) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}