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

double distance(const Point& a, const Point& b) {
    return (a - b).norm();
}

double minDistance(const vector<Point>& points) {
    double minDist = 1e9;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            minDist = min(minDist, distance(points[i], points[j]));
        }
    }
    return minDist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> points;
    
    if (n == 2) {
        points.push_back(Point(0, 0, 1));
        points.push_back(Point(0, 0, -1));
    } else if (n == 3) {
        points.push_back(Point(0, 0, 1));
        points.push_back(Point(sqrt(3)/2, 0, -0.5));
        points.push_back(Point(-sqrt(3)/2, 0, -0.5));
    } else if (n == 4) {
        double t = 1.0/sqrt(3);
        points.push_back(Point(t, t, t));
        points.push_back(Point(t, -t, -t));
        points.push_back(Point(-t, t, -t));
        points.push_back(Point(-t, -t, t));
    } else {
        // Generate points using spherical coordinates with optimization
        for (int i = 0; i < n; i++) {
            double theta = 2 * M_PI * i / n;
            double phi = acos(1 - 2.0 * i / n);
            double x = sin(phi) * cos(theta);
            double y = sin(phi) * sin(theta);
            double z = cos(phi);
            points.push_back(Point(x, y, z));
        }
        
        // Apply simulated annealing to improve the configuration
        double temperature = 1.0;
        double cooling = 0.999;
        
        for (int iter = 0; iter < 10000; iter++) {
            int idx = rand() % n;
            Point original = points[idx];
            
            // Generate random perturbation
            double dx = (rand() / double(RAND_MAX) - 0.5) * 0.1;
            double dy = (rand() / double(RAND_MAX) - 0.5) * 0.1;
            double dz = (rand() / double(RAND_MAX) - 0.5) * 0.1;
            
            Point newPoint = Point(original.x + dx, original.y + dy, original.z + dz);
            if (newPoint.norm() > 1.0) {
                newPoint = newPoint.normalize();
            }
            
            points[idx] = newPoint;
            double newMinDist = minDistance(points);
            
            points[idx] = original;
            double oldMinDist = minDistance(points);
            
            if (newMinDist > oldMinDist || 
                rand() / double(RAND_MAX) < exp((newMinDist - oldMinDist) / temperature)) {
                points[idx] = newPoint;
            }
            
            temperature *= cooling;
        }
    }
    
    double result = minDistance(points);
    cout << fixed << setprecision(10) << result << "\n";
    
    for (const Point& p : points) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}