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
    
    vector<Rectangle> result(n);
    vector<vector<bool>> occupied(10000, vector<bool>(10000, false));
    
    // Sort companies by area in descending order to place larger ones first
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return companies[i].r > companies[j].r;
    });
    
    for (int idx : order) {
        int x = companies[idx].x;
        int y = companies[idx].y;
        int target_area = companies[idx].r;
        
        // Try to find a rectangle containing (x,y) with area close to target_area
        Rectangle best_rect;
        bool found = false;
        
        // Try different aspect ratios
        for (int w = 1; w * w <= target_area && w <= 10000; w++) {
            if (target_area % w == 0) {
                int h = target_area / w;
                if (h > 10000) continue;
                
                // Try different positions where the rectangle contains (x,y)
                for (int dx = 0; dx < w; dx++) {
                    for (int dy = 0; dy < h; dy++) {
                        int x1 = x - dx;
                        int y1 = y - dy;
                        int x2 = x1 + w;
                        int y2 = y1 + h;
                        
                        if (x1 < 0 || y1 < 0 || x2 > 10000 || y2 > 10000) continue;
                        
                        // Check if this rectangle is free
                        bool can_place = true;
                        for (int i = x1; i < x2 && can_place; i++) {
                            for (int j = y1; j < y2 && can_place; j++) {
                                if (occupied[i][j]) {
                                    can_place = false;
                                }
                            }
                        }
                        
                        if (can_place) {
                            best_rect = Rectangle(x1, y1, x2, y2);
                            found = true;
                            goto place_rectangle;
                        }
                    }
                }
            }
        }
        
        place_rectangle:
        if (!found) {
            // Find any free 1x1 rectangle containing the point
            if (x >= 0 && x < 10000 && y >= 0 && y < 10000 && !occupied[x][y]) {
                best_rect = Rectangle(x, y, x + 1, y + 1);
                found = true;
            }
        }
        
        if (found) {
            result[idx] = best_rect;
            // Mark as occupied
            for (int i = best_rect.x1; i < best_rect.x2; i++) {
                for (int j = best_rect.y1; j < best_rect.y2; j++) {
                    occupied[i][j] = true;
                }
            }
        } else {
            // Fallback: place a 1x1 rectangle anywhere
            result[idx] = Rectangle(0, 0, 1, 1);
        }
    }
    
    for (int i = 0; i < n; i++) {
        cout << result[i].x1 << " " << result[i].y1 << " " << result[i].x2 << " " << result[i].y2 << "\n";
    }
    
    return 0;
}