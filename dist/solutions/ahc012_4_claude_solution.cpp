#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    Point(long long x = 0, long long y = 0) : x(x), y(y) {}
};

struct Line {
    Point p1, p2;
    Line(Point p1, Point p2) : p1(p1), p2(p2) {}
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    vector<int> a(11);
    for (int i = 1; i <= 10; i++) {
        cin >> a[i];
    }
    
    vector<Point> strawberries(N);
    for (int i = 0; i < N; i++) {
        cin >> strawberries[i].x >> strawberries[i].y;
    }
    
    vector<Line> cuts;
    
    // Simple strategy: make radial cuts from center
    int num_cuts = min(K, 50);
    
    for (int i = 0; i < num_cuts; i++) {
        double angle = 2.0 * M_PI * i / num_cuts;
        long long dx = (long long)(cos(angle) * 1e9);
        long long dy = (long long)(sin(angle) * 1e9);
        
        Point p1(0, 0);
        Point p2(dx, dy);
        cuts.push_back(Line(p1, p2));
    }
    
    // Add some horizontal and vertical cuts
    if (cuts.size() < K) {
        cuts.push_back(Line(Point(-1e9, 0), Point(1e9, 0)));
    }
    if (cuts.size() < K) {
        cuts.push_back(Line(Point(0, -1e9), Point(0, 1e9)));
    }
    
    // Add some diagonal cuts
    if (cuts.size() < K) {
        cuts.push_back(Line(Point(-1e9, -1e9), Point(1e9, 1e9)));
    }
    if (cuts.size() < K) {
        cuts.push_back(Line(Point(-1e9, 1e9), Point(1e9, -1e9)));
    }
    
    cout << cuts.size() << "\n";
    for (const auto& cut : cuts) {
        cout << cut.p1.x << " " << cut.p1.y << " " << cut.p2.x << " " << cut.p2.y << "\n";
    }
    
    return 0;
}