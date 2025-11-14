#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

struct Rectangle {
    Point p1, p2, p3, p4;
    Rectangle(Point a, Point b, Point c, Point d) : p1(a), p2(b), p3(c), p4(d) {}
};

int N, M;
vector<Point> initialDots;
set<pair<int, int>> dots;
vector<Rectangle> rectangles;

double getWeight(int x, int y) {
    double c = (N - 1) / 2.0;
    return (x - c) * (x - c) + (y - c) * (y - c) + 1;
}

bool isValidPoint(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

bool isRectangle(Point a, Point b, Point c, Point d) {
    vector<Point> points = {a, b, c, d};
    
    // Check if it forms an axis-aligned rectangle
    vector<int> xs = {a.x, b.x, c.x, d.x};
    vector<int> ys = {a.y, b.y, c.y, d.y};
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    
    if (xs[0] == xs[1] && xs[2] == xs[3] && ys[0] == ys[1] && ys[2] == ys[3]) {
        return true;
    }
    
    // Check if it forms a 45-degree rotated rectangle
    Point center = Point((a.x + c.x) / 2, (a.y + c.y) / 2);
    if ((a.x + c.x) % 2 != 0 || (a.y + c.y) % 2 != 0) return false;
    
    Point v1 = a - center;
    Point v2 = c - center;
    if (v1.x != -v2.x || v1.y != -v2.y) return false;
    
    Point v3 = b - center;
    Point v4 = d - center;
    if (v3.x != -v4.x || v3.y != -v4.y) return false;
    
    if (abs(v1.x) == abs(v1.y) && abs(v3.x) == abs(v3.y) && 
        v1.x * v3.x + v1.y * v3.y == 0) {
        return true;
    }
    
    return false;
}

vector<Point> getRectanglePerimeter(Point a, Point b, Point c, Point d) {
    vector<Point> perimeter;
    vector<Point> vertices = {a, b, c, d};
    
    for (int i = 0; i < 4; i++) {
        Point start = vertices[i];
        Point end = vertices[(i + 1) % 4];
        
        int dx = end.x - start.x;
        int dy = end.y - start.y;
        int steps = max(abs(dx), abs(dy));
        
        for (int j = 0; j <= steps; j++) {
            int x = start.x + j * dx / steps;
            int y = start.y + j * dy / steps;
            perimeter.push_back(Point(x, y));
        }
    }
    
    return perimeter;
}

bool hasOnlyThreeDots(Point a, Point b, Point c, Point d) {
    vector<Point> perimeter = getRectanglePerimeter(a, b, c, d);
    int dotCount = 0;
    
    for (Point p : perimeter) {
        if (dots.count({p.x, p.y})) {
            dotCount++;
        }
    }
    
    return dotCount == 3;
}

bool canPlaceRectangle(Point a, Point b, Point c, Point d) {
    if (!isValidPoint(a.x, a.y) || !isValidPoint(b.x, b.y) || 
        !isValidPoint(c.x, c.y) || !isValidPoint(d.x, d.y)) {
        return false;
    }
    
    if (!isRectangle(a, b, c, d)) return false;
    if (!hasOnlyThreeDots(a, b, c, d)) return false;
    
    return true;
}

void solve() {
    vector<tuple<int, int, int, int, int, int, int, int>> operations;
    
    vector<Point> candidates;
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (!dots.count({x, y})) {
                candidates.push_back(Point(x, y));
            }
        }
    }
    
    sort(candidates.begin(), candidates.end(), [](const Point& a, const Point& b) {
        return getWeight(a.x, a.y) > getWeight(b.x, b.y);
    });
    
    for (Point newPoint : candidates) {
        if (dots.count({newPoint.x, newPoint.y})) continue;
        
        vector<Point> existingDots;
        for (auto& dot : dots) {
            existingDots.push_back(Point(dot.first, dot.second));
        }
        
        bool found = false;
        for (int i = 0; i < existingDots.size() && !found; i++) {
            for (int j = i + 1; j < existingDots.size() && !found; j++) {
                for (int k = j + 1; k < existingDots.size() && !found; k++) {
                    Point p2 = existingDots[i];
                    Point p3 = existingDots[j];
                    Point p4 = existingDots[k];
                    
                    if (canPlaceRectangle(newPoint, p2, p3, p4)) {
                        operations.push_back(make_tuple(
                            newPoint.x, newPoint.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y
                        ));
                        dots.insert({newPoint.x, newPoint.y});
                        found = true;
                        break;
                    }
                }
            }
        }
    }
    
    cout << operations.size() << endl;
    for (auto& op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << " " << get<3>(op) << " "
             << get<4>(op) << " " << get<5>(op) << " " << get<6>(op) << " " << get<7>(op) << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> N >> M;
    
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        initialDots.push_back(Point(x, y));
        dots.insert({x, y});
    }
    
    solve();
    
    return 0;
}