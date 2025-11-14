#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

struct Line {
    Point p1, p2;
    Line(Point p1, Point p2) : p1(p1), p2(p2) {}
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
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
    
    vector<Line> lines;
    
    // Simple approach: make cuts to separate strawberries into groups
    // Use random lines within the circular cake
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> coord_dist(-10000, 10000);
    
    // Add some strategic cuts
    for (int cuts = 0; cuts < min(K, 50); cuts++) {
        int px = coord_dist(gen);
        int py = coord_dist(gen);
        int qx = coord_dist(gen);
        int qy = coord_dist(gen);
        
        // Ensure the two points are different
        if (px == qx && py == qy) {
            qx += 1;
        }
        
        lines.push_back(Line(Point(px, py), Point(qx, qy)));
    }
    
    cout << lines.size() << "\n";
    for (const auto& line : lines) {
        cout << line.p1.x << " " << line.p1.y << " " << line.p2.x << " " << line.p2.y << "\n";
    }
    
    return 0;
}