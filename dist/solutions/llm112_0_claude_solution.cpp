#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double norm() const { return sqrt(x * x + y * y + z * z); }
    Point normalize() const { 
        double n = norm(); 
        if (n < 1e-12) return Point(0, 0, 0);
        return Point(x / n, y / n, z / n); 
    }
};

double dist(const Point& a, const Point& b) {
    return (a - b).norm();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Point> points(n);
    
    // Initialize with random points on sphere
    mt19937 rng(42);
    uniform_real_distribution<double> dis(-1.0, 1.0);
    
    for (int i = 0; i < n; i++) {
        do {
            points[i] = Point(dis(rng), dis(rng), dis(rng));
        } while (points[i].norm() > 1.0);
        if (points[i].norm() > 0) {
            points[i] = points[i].normalize();
        }
    }
    
    // Optimize using force-based approach
    double temp = 1.0;
    double cooling = 0.99;
    
    for (int iter = 0; iter < 5000; iter++) {
        for (int i = 0; i < n; i++) {
            Point force(0, 0, 0);
            
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    Point diff = points[i] - points[j];
                    double d = diff.norm();
                    if (d > 1e-12) {
                        force = force + diff * (1.0 / (d * d * d));
                    }
                }
            }
            
            Point new_pos = points[i] + force * temp;
            if (new_pos.norm() > 1.0) {
                new_pos = new_pos.normalize();
            }
            points[i] = new_pos;
        }
        temp *= cooling;
    }
    
    // Calculate minimum distance
    double min_dist = 1e9;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            min_dist = min(min_dist, dist(points[i], points[j]));
        }
    }
    
    cout << fixed << setprecision(9) << min_dist << "\n";
    for (int i = 0; i < n; i++) {
        cout << fixed << setprecision(9) << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
    }
    
    return 0;
}