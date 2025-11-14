#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    int d;
    Point(int x = 0, int y = 0, int d = 0) : x(x), y(y), d(d) {}
};

struct Line {
    int x1, y1, x2, y2;
    Line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

long long cross(long long ax, long long ay, long long bx, long long by) {
    return ax * by - ay * bx;
}

int orientation(long long px, long long py, long long qx, long long qy, long long rx, long long ry) {
    long long val = cross(qx - px, qy - py, rx - px, ry - py);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool onSegment(long long px, long long py, long long qx, long long qy, long long rx, long long ry) {
    return (qx <= max(px, rx) && qx >= min(px, rx) &&
            qy <= max(py, ry) && qy >= min(py, ry));
}

bool doIntersect(const Line& l1, const Line& l2) {
    int o1 = orientation(l1.x1, l1.y1, l1.x2, l1.y2, l2.x1, l2.y1);
    int o2 = orientation(l1.x1, l1.y1, l1.x2, l1.y2, l2.x2, l2.y2);
    int o3 = orientation(l2.x1, l2.y1, l2.x2, l2.y2, l1.x1, l1.y1);
    int o4 = orientation(l2.x1, l2.y1, l2.x2, l2.y2, l1.x2, l1.y2);
    
    if (o1 != o2 && o3 != o4) return true;
    
    if (o1 == 0 && onSegment(l1.x1, l1.y1, l2.x1, l2.y1, l1.x2, l1.y2)) return true;
    if (o2 == 0 && onSegment(l1.x1, l1.y1, l2.x2, l2.y2, l1.x2, l1.y2)) return true;
    if (o3 == 0 && onSegment(l2.x1, l2.y1, l1.x1, l1.y1, l2.x2, l2.y2)) return true;
    if (o4 == 0 && onSegment(l2.x1, l2.y1, l1.x2, l1.y2, l2.x2, l2.y2)) return true;
    
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    vector<int> a(11);
    for (int i = 1; i <= 10; i++) {
        cin >> a[i];
    }
    
    vector<Point> strawberries;
    int idx = 0;
    for (int d = 1; d <= 10; d++) {
        for (int cnt = 0; cnt < d * a[d]; cnt++) {
            int x, y;
            cin >> x >> y;
            strawberries.push_back(Point(x, y, d));
            idx++;
        }
    }
    
    vector<Line> cuts;
    
    // Simple greedy approach: try to separate strawberries by their d values
    for (int iter = 0; iter < min(K, 50); iter++) {
        vector<int> count(11, 0);
        
        // Count current distribution
        // This is a simplified version - in practice we'd need to determine
        // which region each strawberry is in after all cuts
        for (const auto& s : strawberries) {
            count[s.d]++;
        }
        
        // Find the d value that's most over-represented
        int best_d = 1;
        double worst_ratio = 0;
        for (int d = 1; d <= 10; d++) {
            if (a[d] > 0 && count[d] > a[d]) {
                double ratio = (double)count[d] / a[d];
                if (ratio > worst_ratio) {
                    worst_ratio = ratio;
                    best_d = d;
                }
            }
        }
        
        // Try to create a cut that separates some strawberries of best_d
        if (worst_ratio > 1.1) {
            // Find two strawberries of different d values
            Point p1, p2;
            bool found = false;
            for (const auto& s1 : strawberries) {
                if (found) break;
                for (const auto& s2 : strawberries) {
                    if (s1.d != s2.d && abs(s1.x - s2.x) + abs(s1.y - s2.y) > 100) {
                        p1 = s1;
                        p2 = s2;
                        found = true;
                        break;
                    }
                }
            }
            
            if (found) {
                // Create a perpendicular bisector-like line
                int mx = (p1.x + p2.x) / 2;
                int my = (p1.y + p2.y) / 2;
                int dx = p2.y - p1.y;
                int dy = p1.x - p2.x;
                
                if (dx == 0 && dy == 0) {
                    dx = 1;
                }
                
                // Extend the line
                const int SCALE = 10000;
                int x1 = mx - dx * SCALE;
                int y1 = my - dy * SCALE;
                int x2 = mx + dx * SCALE;
                int y2 = my + dy * SCALE;
                
                // Clamp to bounds
                x1 = max(-1000000000, min(1000000000, x1));
                y1 = max(-1000000000, min(1000000000, y1));
                x2 = max(-1000000000, min(1000000000, x2));
                y2 = max(-1000000000, min(1000000000, y2));
                
                cuts.push_back(Line(x1, y1, x2, y2));
            }
        }
    }
    
    // Add some random cuts if we have budget left
    while (cuts.size() < min(K, 30)) {
        int x1 = rand() % 20000 - 10000;
        int y1 = rand() % 20000 - 10000;
        int x2 = rand() % 20000 - 10000;
        int y2 = rand() % 20000 - 10000;
        
        if (x1 != x2 || y1 != y2) {
            cuts.push_back(Line(x1, y1, x2, y2));
        }
    }
    
    cout << cuts.size() << "\n";
    for (const auto& cut : cuts) {
        cout << cut.x1 << " " << cut.y1 << " " << cut.x2 << " " << cut.y2 << "\n";
    }
    
    return 0;
}