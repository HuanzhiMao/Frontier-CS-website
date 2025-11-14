#include <bits/stdc++.h>
using namespace std;

struct Company {
    int x, y, r;
    int id;
};

struct Rectangle {
    int x1, y1, x2, y2;
    Rectangle(int a = 0, int b = 0, int c = 0, int d = 0) : x1(a), y1(b), x2(c), y2(d) {}
    int area() const { return (x2 - x1) * (y2 - y1); }
    bool contains(int x, int y) const {
        return x >= x1 && x < x2 && y >= y1 && y < y2;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Company> companies(n);
    for (int i = 0; i < n; i++) {
        cin >> companies[i].x >> companies[i].y >> companies[i].r;
        companies[i].id = i;
    }
    
    vector<Rectangle> rectangles(n);
    vector<vector<bool>> used(10000, vector<bool>(10000, false));
    
    // Sort companies by desired area in descending order
    sort(companies.begin(), companies.end(), [](const Company& a, const Company& b) {
        return a.r > b.r;
    });
    
    for (const auto& company : companies) {
        int x = company.x;
        int y = company.y;
        int r = company.r;
        int id = company.id;
        
        int best_score = -1;
        Rectangle best_rect;
        
        // Try different rectangle sizes around the desired point
        for (int w = 1; w <= 10000 && w * w <= r * 4; w++) {
            int h = (r + w - 1) / w; // ceiling division
            if (h > 10000) continue;
            
            // Try different positions for this w x h rectangle
            for (int dx = -w; dx <= w; dx++) {
                for (int dy = -h; dy <= h; dy++) {
                    int x1 = max(0, min(x + dx, 10000 - w));
                    int y1 = max(0, min(y + dy, 10000 - h));
                    int x2 = x1 + w;
                    int y2 = y1 + h;
                    
                    if (x2 > 10000 || y2 > 10000) continue;
                    if (x < x1 || x >= x2 || y < y1 || y >= y2) continue;
                    
                    // Check if this position is available
                    bool valid = true;
                    for (int i = x1; i < x2 && valid; i++) {
                        for (int j = y1; j < y2 && valid; j++) {
                            if (used[i][j]) valid = false;
                        }
                    }
                    
                    if (valid) {
                        int area = w * h;
                        int score = area;
                        if (score > best_score) {
                            best_score = score;
                            best_rect = Rectangle(x1, y1, x2, y2);
                        }
                    }
                }
            }
        }
        
        // If no valid rectangle found, try to place a small 1x1 rectangle
        if (best_score == -1) {
            for (int dx = -50; dx <= 50; dx++) {
                for (int dy = -50; dy <= 50; dy++) {
                    int x1 = max(0, min(x + dx, 9999));
                    int y1 = max(0, min(y + dy, 9999));
                    int x2 = x1 + 1;
                    int y2 = y1 + 1;
                    
                    if (!used[x1][y1]) {
                        best_rect = Rectangle(x1, y1, x2, y2);
                        best_score = 1;
                        break;
                    }
                }
                if (best_score > 0) break;
            }
        }
        
        rectangles[id] = best_rect;
        
        // Mark used cells
        for (int i = best_rect.x1; i < best_rect.x2; i++) {
            for (int j = best_rect.y1; j < best_rect.y2; j++) {
                used[i][j] = true;
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        cout << rectangles[i].x1 << " " << rectangles[i].y1 << " " 
             << rectangles[i].x2 << " " << rectangles[i].y2 << "\n";
    }
    
    return 0;
}