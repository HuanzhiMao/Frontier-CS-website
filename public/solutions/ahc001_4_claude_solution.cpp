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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Company> companies(n);
    for (int i = 0; i < n; i++) {
        cin >> companies[i].x >> companies[i].y >> companies[i].r;
        companies[i].id = i;
    }
    
    vector<Rectangle> rects(n);
    
    // Simple greedy approach: place rectangles as squares around desired points
    vector<vector<bool>> used(10000, vector<bool>(10000, false));
    
    for (int i = 0; i < n; i++) {
        int x = companies[i].x;
        int y = companies[i].y;
        int r = companies[i].r;
        
        // Try to make a square with area close to r
        int side = max(1, (int)sqrt(r));
        
        // Find best position for rectangle containing (x, y)
        int best_x1 = -1, best_y1 = -1, best_x2 = -1, best_y2 = -1;
        int best_area = 0;
        
        for (int dx = -side; dx <= 0; dx++) {
            for (int dy = -side; dy <= 0; dy++) {
                int x1 = max(0, x + dx);
                int y1 = max(0, y + dy);
                
                // Try different rectangle sizes
                for (int w = 1; w <= min(100, 10000 - x1); w++) {
                    for (int h = 1; h <= min(100, 10000 - y1); h++) {
                        int x2 = x1 + w;
                        int y2 = y1 + h;
                        
                        if (x2 > 10000 || y2 > 10000) continue;
                        if (x < x1 || x >= x2 || y < y1 || y >= y2) continue;
                        
                        // Check if area is free
                        bool free = true;
                        for (int px = x1; px < x2 && free; px++) {
                            for (int py = y1; py < y2 && free; py++) {
                                if (used[px][py]) free = false;
                            }
                        }
                        
                        if (free && w * h > best_area) {
                            best_area = w * h;
                            best_x1 = x1;
                            best_y1 = y1;
                            best_x2 = x2;
                            best_y2 = y2;
                        }
                    }
                }
            }
        }
        
        if (best_area > 0) {
            rects[i] = Rectangle(best_x1, best_y1, best_x2, best_y2);
            for (int px = best_x1; px < best_x2; px++) {
                for (int py = best_y1; py < best_y2; py++) {
                    used[px][py] = true;
                }
            }
        } else {
            // Fallback: find any free 1x1 space
            bool found = false;
            for (int px = 0; px < 9999 && !found; px++) {
                for (int py = 0; py < 9999 && !found; py++) {
                    if (!used[px][py]) {
                        rects[i] = Rectangle(px, py, px + 1, py + 1);
                        used[px][py] = true;
                        found = true;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        cout << rects[i].x1 << " " << rects[i].y1 << " " 
             << rects[i].x2 << " " << rects[i].y2 << "\n";
    }
    
    return 0;
}