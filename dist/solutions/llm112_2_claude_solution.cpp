#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double norm() const { return sqrt(x * x + y * y + z * z); }
    Point normalize() const { double n = norm(); return n > 0 ? Point(x/n, y/n, z/n) : Point(); }
};

double dist(const Point& a, const Point& b) {
    return (a - b).norm();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> points(n);
    
    // Initialize with random points on sphere
    srand(42);
    for (int i = 0; i < n; i++) {
        double theta = 2 * M_PI * rand() / RAND_MAX;
        double phi = acos(2.0 * rand() / RAND_MAX - 1.0);
        points[i] = Point(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
    }
    
    // Iterative improvement
    for (int iter = 0; iter < 1000; iter++) {
        bool improved = false;
        
        for (int i = 0; i < n; i++) {
            Point force(0, 0, 0);
            
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    Point diff = points[i] - points[j];
                    double d = diff.norm();
                    if (d > 0) {
                        force = force + diff * (1.0 / (d * d * d));
                    }
                }
            }
            
            double step = 0.01;
            Point new_pos = points[i] + force * step;
            
            // Project to sphere surface
            if (new_pos.norm() > 1e-9) {
                new_pos = new_pos.normalize();
            }
            
            // Check if this improves minimum distance
            double old_min_dist = 1e9;
            double new_min_dist = 1e9;
            
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    old_min_dist = min(old_min_dist, dist(points[i], points[j]));
                    new_min_dist = min(new_min_dist, dist(new_pos, points[j]));
                }
            }
            
            if (new_min_dist > old_min_dist) {
                points[i] = new_pos;
                improved = true;
            }
        }
        
        if (!improved) break;
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