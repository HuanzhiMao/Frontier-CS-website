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
set<pair<int, int>> dots;
vector<Rectangle> rectangles;

double weight(int x, int y) {
    double c = (N - 1) / 2.0;
    return (x - c) * (x - c) + (y - c) * (y - c) + 1;
}

bool isValidPoint(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

bool hasDot(int x, int y) {
    return dots.count({x, y});
}

vector<Point> getPerimeter(Point p1, Point p2, Point p3, Point p4) {
    vector<Point> perimeter;
    
    auto addLine = [&](Point a, Point b) {
        int dx = b.x - a.x;
        int dy = b.y - a.y;
        int steps = max(abs(dx), abs(dy));
        if (steps == 0) return;
        
        for (int i = 0; i <= steps; i++) {
            int x = a.x + dx * i / steps;
            int y = a.y + dy * i / steps;
            perimeter.push_back(Point(x, y));
        }
    };
    
    addLine(p1, p2);
    addLine(p2, p3);
    addLine(p3, p4);
    addLine(p4, p1);
    
    return perimeter;
}

bool isValidRectangle(Point p1, Point p2, Point p3, Point p4) {
    if (!isValidPoint(p1.x, p1.y) || hasDot(p1.x, p1.y)) return false;
    if (!hasDot(p2.x, p2.y) || !hasDot(p3.x, p3.y) || !hasDot(p4.x, p4.y)) return false;
    
    vector<Point> perimeter = getPerimeter(p1, p2, p3, p4);
    
    // Check no other dots on perimeter
    for (Point p : perimeter) {
        if (p == p1 || p == p2 || p == p3 || p == p4) continue;
        if (hasDot(p.x, p.y)) return false;
    }
    
    return true;
}

vector<tuple<int, int, int, int, int, int, int, int>> operations;

void solve() {
    vector<pair<int, int>> candidates;
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (!hasDot(x, y)) {
                candidates.push_back({x, y});
            }
        }
    }
    
    sort(candidates.begin(), candidates.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return weight(a.first, a.second) > weight(b.first, b.second);
    });
    
    for (auto [x1, y1] : candidates) {
        vector<pair<int, int>> dotList;
        for (auto [dx, dy] : dots) {
            dotList.push_back({dx, dy});
        }
        
        for (int i = 0; i < dotList.size(); i++) {
            for (int j = i + 1; j < dotList.size(); j++) {
                for (int k = j + 1; k < dotList.size(); k++) {
                    Point p1(x1, y1);
                    Point p2(dotList[i].first, dotList[i].second);
                    Point p3(dotList[j].first, dotList[j].second);
                    Point p4(dotList[k].first, dotList[k].second);
                    
                    if (isValidRectangle(p1, p2, p3, p4)) {
                        dots.insert({x1, y1});
                        operations.push_back({x1, y1, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y});
                        rectangles.push_back(Rectangle(p1, p2, p3, p4));
                        goto next_candidate;
                    }
                    
                    if (isValidRectangle(p1, p2, p4, p3)) {
                        dots.insert({x1, y1});
                        operations.push_back({x1, y1, p2.x, p2.y, p4.x, p4.y, p3.x, p3.y});
                        rectangles.push_back(Rectangle(p1, p2, p4, p3));
                        goto next_candidate;
                    }
                    
                    if (isValidRectangle(p1, p3, p2, p4)) {
                        dots.insert({x1, y1});
                        operations.push_back({x1, y1, p3.x, p3.y, p2.x, p2.y, p4.x, p4.y});
                        rectangles.push_back(Rectangle(p1, p3, p2, p4));
                        goto next_candidate;
                    }
                }
            }
        }
        next_candidate:;
    }
}

int main() {
    cin >> N >> M;
    
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        dots.insert({x, y});
    }
    
    solve();
    
    cout << operations.size() << "\n";
    for (auto [x1, y1, x2, y2, x3, y3, x4, y4] : operations) {
        cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " " << x4 << " " << y4 << "\n";
    }
    
    return 0;
}