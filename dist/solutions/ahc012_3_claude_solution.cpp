#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    int d;
    Point(long long x = 0, long long y = 0, int d = 0) : x(x), y(y), d(d) {}
};

struct Line {
    long long x1, y1, x2, y2;
    Line(long long x1, long long y1, long long x2, long long y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

long long cross(long long ax, long long ay, long long bx, long long by, long long cx, long long cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

bool pointInCircle(long long x, long long y) {
    return x * x + y * y < 100000000LL;
}

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
    vector<int> strawberry_d(N);
    int idx = 0;
    for (int d = 1; d <= 10; d++) {
        for (int cnt = 0; cnt < a[d]; cnt++) {
            for (int j = 0; j < d; j++) {
                long long x, y;
                cin >> x >> y;
                strawberries[idx] = Point(x, y, d);
                strawberry_d[idx] = d;
                idx++;
            }
        }
    }
    
    vector<Line> cuts;
    
    // Simple greedy approach: try to separate strawberries by their d values
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> coord_dist(-1000000000LL, 1000000000LL);
    
    int max_cuts = min(K, 50);
    
    for (int cut_idx = 0; cut_idx < max_cuts; cut_idx++) {
        // Generate random line
        long long x1 = coord_dist(gen);
        long long y1 = coord_dist(gen);
        long long x2 = coord_dist(gen);
        long long y2 = coord_dist(gen);
        
        // Ensure the two points are different
        if (x1 == x2 && y1 == y2) {
            x2 += 1;
        }
        
        cuts.push_back(Line(x1, y1, x2, y2));
    }
    
    cout << cuts.size() << "\n";
    for (const auto& cut : cuts) {
        cout << cut.x1 << " " << cut.y1 << " " << cut.x2 << " " << cut.y2 << "\n";
    }
    
    return 0;
}