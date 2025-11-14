#include <bits/stdc++.h>
using namespace std;

struct Company {
    int x, y, r;
    int id;
};

struct Rectangle {
    int a, b, c, d;
    Rectangle() : a(0), b(0), c(0), d(0) {}
    Rectangle(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {}
    int area() const { return (c - a) * (d - b); }
};

bool overlaps(const Rectangle& r1, const Rectangle& r2) {
    return !(r1.c <= r2.a || r2.c <= r1.a || r1.d <= r2.b || r2.d <= r1.b);
}

bool contains_point(const Rectangle& r, int x, int y) {
    return r.a <= x && x < r.c && r.b <= y && y < r.d;
}

double satisfaction(int desired_area, int actual_area) {
    if (actual_area == 0) return 0.0;
    int min_area = min(desired_area, actual_area);
    int max_area = max(desired_area, actual_area);
    double ratio = (double)min_area / max_area;
    return 1.0 - (1.0 - ratio) * (1.0 - ratio);
}

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
    
    // Simple greedy approach: place rectangles one by one
    for (int i = 0; i < n; i++) {
        int x = companies[i].x;
        int y = companies[i].y;
        int area = companies[i].r;
        
        // Try to create a square-ish rectangle around the desired point
        int side = sqrt(area);
        int width = side;
        int height = (area + width - 1) / width;
        
        // Try different positions around the desired point
        vector<pair<int, int>> offsets = {
            {0, 0}, {-1, 0}, {0, -1}, {-1, -1},
            {-width/2, -height/2}, {-width/3, -height/3}
        };
        
        bool placed = false;
        for (auto& offset : offsets) {
            int a = max(0, min(10000 - width, x + offset.first));
            int b = max(0, min(10000 - height, y + offset.second));
            int c = min(10000, a + width);
            int d = min(10000, b + height);
            
            if (c <= a || d <= b) continue;
            if (!contains_point(Rectangle(a, b, c, d), x, y)) continue;
            
            Rectangle candidate(a, b, c, d);
            bool valid = true;
            
            for (int j = 0; j < i; j++) {
                if (overlaps(candidate, rectangles[j])) {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                rectangles[i] = candidate;
                placed = true;
                break;
            }
        }
        
        if (!placed) {
            // Try smaller rectangles
            for (int w = 1; w <= min(100, area); w++) {
                int h = (area + w - 1) / w;
                if (h > 10000) continue;
                
                for (int dx = -10; dx <= 10; dx++) {
                    for (int dy = -10; dy <= 10; dy++) {
                        int a = max(0, min(10000 - w, x + dx));
                        int b = max(0, min(10000 - h, y + dy));
                        int c = min(10000, a + w);
                        int d = min(10000, b + h);
                        
                        if (c <= a || d <= b) continue;
                        if (!contains_point(Rectangle(a, b, c, d), x, y)) continue;
                        
                        Rectangle candidate(a, b, c, d);
                        bool valid = true;
                        
                        for (int j = 0; j < i; j++) {
                            if (overlaps(candidate, rectangles[j])) {
                                valid = false;
                                break;
                            }
                        }
                        
                        if (valid) {
                            rectangles[i] = candidate;
                            placed = true;
                            goto next_company;
                        }
                    }
                }
            }
            next_company:;
        }
        
        if (!placed) {
            rectangles[i] = Rectangle(x, y, min(10000, x + 1), min(10000, y + 1));
        }
    }
    
    for (int i = 0; i < n; i++) {
        cout << rectangles[i].a << " " << rectangles[i].b << " " 
             << rectangles[i].c << " " << rectangles[i].d << "\n";
    }
    
    return 0;
}