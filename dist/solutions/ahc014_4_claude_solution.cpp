#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
};

int N, M;
set<Point> dots;
set<pair<Point, Point>> edges;
double center;

double getWeight(const Point& p) {
    double dx = p.x - center;
    double dy = p.y - center;
    return dx * dx + dy * dy + 1;
}

bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

bool hasEdge(const Point& a, const Point& b) {
    Point p1 = min(a, b);
    Point p2 = max(a, b);
    return edges.count({p1, p2}) > 0;
}

void addEdge(const Point& a, const Point& b) {
    Point p1 = min(a, b);
    Point p2 = max(a, b);
    edges.insert({p1, p2});
}

vector<Point> getPointsOnSegment(const Point& a, const Point& b) {
    vector<Point> points;
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int steps = max(abs(dx), abs(dy));
    
    for (int i = 0; i <= steps; i++) {
        int x = a.x + dx * i / steps;
        int y = a.y + dy * i / steps;
        points.push_back(Point(x, y));
    }
    return points;
}

bool canFormRectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    vector<Point> rect = {p1, p2, p3, p4};
    
    // Check if edges would overlap with existing edges
    for (int i = 0; i < 4; i++) {
        Point a = rect[i];
        Point b = rect[(i + 1) % 4];
        if (hasEdge(a, b)) return false;
        
        // Check no other dots on perimeter
        vector<Point> segPoints = getPointsOnSegment(a, b);
        for (const Point& sp : segPoints) {
            if (sp == a || sp == b) continue;
            if (dots.count(sp)) return false;
        }
    }
    
    return true;
}

bool isRectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    vector<Point> points = {p1, p2, p3, p4};
    
    // Check if it forms a valid rectangle (axis-aligned or 45-degree)
    vector<int> dx, dy;
    for (int i = 0; i < 4; i++) {
        dx.push_back(points[(i+1)%4].x - points[i].x);
        dy.push_back(points[(i+1)%4].y - points[i].y);
    }
    
    // Check if opposite sides are parallel and equal
    if (dx[0] == -dx[2] && dy[0] == -dy[2] && dx[1] == -dx[3] && dy[1] == -dy[3]) {
        // Check if adjacent sides are perpendicular
        if (dx[0] * dx[1] + dy[0] * dy[1] == 0) {
            return true;
        }
    }
    
    return false;
}

int main() {
    cin >> N >> M;
    center = (N - 1) / 2.0;
    
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        dots.insert(Point(x, y));
    }
    
    vector<tuple<Point, Point, Point, Point>> operations;
    
    // Try to find rectangles
    for (int iter = 0; iter < 10000; iter++) {
        Point bestP1;
        Point bestP2, bestP3, bestP4;
        double bestScore = -1;
        
        // Try all possible positions for new dot
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                Point p1(x, y);
                if (dots.count(p1)) continue;
                
                // Try all combinations of 3 existing dots
                vector<Point> dotList;
                for (const Point& p : dots) {
                    dotList.push_back(p);
                }
                
                if (dotList.size() < 3) continue;
                
                for (int i = 0; i < dotList.size(); i++) {
                    for (int j = i + 1; j < dotList.size(); j++) {
                        for (int k = j + 1; k < dotList.size(); k++) {
                            Point p2 = dotList[i];
                            Point p3 = dotList[j];
                            Point p4 = dotList[k];
                            
                            if (isRectangle(p1, p2, p3, p4) && canFormRectangle(p1, p2, p3, p4)) {
                                double score = getWeight(p1);
                                if (score > bestScore) {
                                    bestScore = score;
                                    bestP1 = p1;
                                    bestP2 = p2;
                                    bestP3 = p3;
                                    bestP4 = p4;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if (bestScore > -1) {
            operations.push_back({bestP1, bestP2, bestP3, bestP4});
            dots.insert(bestP1);
            
            vector<Point> rect = {bestP1, bestP2, bestP3, bestP4};
            for (int i = 0; i < 4; i++) {
                addEdge(rect[i], rect[(i + 1) % 4]);
            }
        } else {
            break;
        }
    }
    
    cout << operations.size() << "\n";
    for (auto& op : operations) {
        Point p1, p2, p3, p4;
        tie(p1, p2, p3, p4) = op;
        cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " ";
        cout << p3.x << " " << p3.y << " " << p4.x << " " << p4.y << "\n";
    }
    
    return 0;
}