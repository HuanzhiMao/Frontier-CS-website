#include <bits/stdc++.h>
using namespace std;

struct Company {
    int x, y, r;
    int id;
};

struct Rectangle {
    int x1, y1, x2, y2;
    Rectangle() : x1(0), y1(0), x2(0), y2(0) {}
    Rectangle(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {}
    int area() const { return (x2 - x1) * (y2 - y1); }
    bool contains(int x, int y) const {
        return x1 <= x && x < x2 && y1 <= y && y < y2;
    }
    bool overlaps(const Rectangle& other) const {
        return !(x2 <= other.x1 || other.x2 <= x1 || y2 <= other.y1 || other.y2 <= y1);
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
    
    // Simple greedy approach: place rectangles as squares around desired points
    for (int i = 0; i < n; i++) {
        int area = companies[i].r;
        int side = (int)sqrt(area);
        
        // Try to make it close to square
        int w = side;
        int h = (area + w - 1) / w;
        
        if (w * h < area) {
            if (w * (h + 1) >= area) h++;
            else w++;
        }
        
        // Center around the desired point
        int cx = companies[i].x;
        int cy = companies[i].y;
        
        int x1 = max(0, cx - w / 2);
        int y1 = max(0, cy - h / 2);
        int x2 = min(10000, x1 + w);
        int y2 = min(10000, y1 + h);
        
        // Adjust if out of bounds
        if (x2 - x1 < w && x1 > 0) x1 = max(0, x2 - w);
        if (y2 - y1 < h && y1 > 0) y1 = max(0, y2 - h);
        
        rectangles[i] = Rectangle(x1, y1, x2, y2);
    }
    
    // Try to resolve overlaps with simple adjustments
    for (int iter = 0; iter < 10; iter++) {
        bool changed = false;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (rectangles[i].overlaps(rectangles[j])) {
                    // Simple resolution: move one rectangle
                    if (companies[i].r <= companies[j].r) {
                        // Move rectangle i
                        int w = rectangles[i].x2 - rectangles[i].x1;
                        int h = rectangles[i].y2 - rectangles[i].y1;
                        
                        // Try different positions
                        for (int dx = -50; dx <= 50; dx += 10) {
                            for (int dy = -50; dy <= 50; dy += 10) {
                                int nx1 = max(0, min(10000 - w, companies[i].x + dx));
                                int ny1 = max(0, min(10000 - h, companies[i].y + dy));
                                int nx2 = nx1 + w;
                                int ny2 = ny1 + h;
                                
                                Rectangle new_rect(nx1, ny1, nx2, ny2);
                                
                                if (new_rect.contains(companies[i].x, companies[i].y)) {
                                    bool valid = true;
                                    for (int k = 0; k < n; k++) {
                                        if (k != i && new_rect.overlaps(rectangles[k])) {
                                            valid = false;
                                            break;
                                        }
                                    }
                                    if (valid) {
                                        rectangles[i] = new_rect;
                                        changed = true;
                                        goto next_pair;
                                    }
                                }
                            }
                        }
                    }
                }
                next_pair:;
            }
        }
        if (!changed) break;
    }
    
    // Output results
    for (int i = 0; i < n; i++) {
        cout << rectangles[i].x1 << " " << rectangles[i].y1 << " " 
             << rectangles[i].x2 << " " << rectangles[i].y2 << "\n";
    }
    
    return 0;
}