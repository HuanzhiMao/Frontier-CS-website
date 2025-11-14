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

bool isAxisAligned(Point p1, Point p2, Point p3, Point p4) {
    return (p1.x == p2.x && p2.y == p3.y && p3.x == p4.x && p4.y == p1.y) ||
           (p1.y == p2.y && p2.x == p3.x && p3.y == p4.y && p4.x == p1.x);
}

bool is45Degree(Point p1, Point p2, Point p3, Point p4) {
    Point v1 = p2 - p1, v2 = p3 - p2, v3 = p4 - p3, v4 = p1 - p4;
    return abs(abs(v1.x) - abs(v1.y)) == 0 && abs(abs(v2.x) - abs(v2.y)) == 0 &&
           abs(abs(v3.x) - abs(v3.y)) == 0 && abs(abs(v4.x) - abs(v4.y)) == 0;
}

vector<Point> getPerimeterPoints(Point p1, Point p2, Point p3, Point p4) {
    vector<Point> points;
    
    auto addLine = [&](Point a, Point b) {
        int dx = (b.x > a.x) ? 1 : (b.x < a.x) ? -1 : 0;
        int dy = (b.y > a.y) ? 1 : (b.y < a.y) ? -1 : 0;
        Point curr = a;
        while (!(curr == b)) {
            points.push_back(curr);
            curr.x += dx;
            curr.y += dy;
        }
    };
    
    addLine(p1, p2);
    addLine(p2, p3);
    addLine(p3, p4);
    addLine(p4, p1);
    
    return points;
}

bool isValidRectangle(Point p1, Point p2, Point p3, Point p4) {
    if (dots.count({p1.x, p1.y}) || !dots.count({p2.x, p2.y}) || 
        !dots.count({p3.x, p3.y}) || !dots.count({p4.x, p4.y})) {
        return false;
    }
    
    if (p1.x < 0 || p1.x >= N || p1.y < 0 || p1.y >= N) return false;
    
    if (!isAxisAligned(p1, p2, p3, p4) && !is45Degree(p1, p2, p3, p4)) {
        return false;
    }
    
    vector<Point> perimeter = getPerimeterPoints(p1, p2, p3, p4);
    for (const Point& p : perimeter) {
        if (!(p == p1 || p == p2 || p == p3 || p == p4)) {
            if (dots.count({p.x, p.y})) return false;
        }
    }
    
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        dots.insert({x, y});
    }
    
    vector<tuple<int, int, int, int, int, int, int, int>> operations;
    
    for (int iter = 0; iter < 10000; iter++) {
        bool found = false;
        
        vector<pair<int, int>> dotList(dots.begin(), dots.end());
        
        for (int x1 = 0; x1 < N && !found; x1++) {
            for (int y1 = 0; y1 < N && !found; y1++) {
                if (dots.count({x1, y1})) continue;
                
                Point p1(x1, y1);
                
                for (int i = 0; i < dotList.size() && !found; i++) {
                    for (int j = i + 1; j < dotList.size() && !found; j++) {
                        for (int k = j + 1; k < dotList.size() && !found; k++) {
                            Point p2(dotList[i].first, dotList[i].second);
                            Point p3(dotList[j].first, dotList[j].second);
                            Point p4(dotList[k].first, dotList[k].second);
                            
                            if (isValidRectangle(p1, p2, p3, p4)) {
                                operations.push_back({x1, y1, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y});
                                dots.insert({x1, y1});
                                rectangles.push_back(Rectangle(p1, p2, p3, p4));
                                found = true;
                            }
                        }
                    }
                }
            }
        }
        
        if (!found) break;
    }
    
    cout << operations.size() << "\n";
    for (const auto& op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << " " << get<3>(op) << " "
             << get<4>(op) << " " << get<5>(op) << " " << get<6>(op) << " " << get<7>(op) << "\n";
    }
    
    return 0;
}