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
        if (n == 0) return *this;
        return *this * (1.0 / n);
    }
};

double dist(const Point& a, const Point& b) {
    Point d = a - b;
    return d.norm();
}

double minDist(const vector<Point>& points) {
    double md = 1e9;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            md = min(md, dist(points[i], points[j]));
        }
    }
    return md;
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
        double x = (rand() / (double)RAND_MAX - 0.5) * 2;
        double y = (rand() / (double)RAND_MAX - 0.5) * 2;
        double z = (rand() / (double)RAND_MAX - 0.5) * 2;
        points[i] = Point(x, y, z).normalize();
    }
    
    // Optimize using simulated annealing
    double temp = 1.0;
    double cooling = 0.999;
    
    for (int iter = 0; iter < 100000; iter++) {
        int idx = rand() % n;
        Point old_point = points[idx];
        
        // Small random perturbation
        double dx = (rand() / (double)RAND_MAX - 0.5) * temp;
        double dy = (rand() / (double)RAND_MAX - 0.5) * temp;
        double dz = (rand() / (double)RAND_MAX - 0.5) * temp;
        
        Point new_point = Point(old_point.x + dx, old_point.y + dy, old_point.z + dz);
        
        // Project back to sphere surface
        if (new_point.norm() > 1e-9) {
            new_point = new_point.normalize();
        }
        
        points[idx] = new_point;
        
        double old_min = minDist(vector<Point>(points.begin(), points.begin() + idx)) + 
                        minDist(vector<Point>(points.begin() + idx + 1, points.end()));
        
        // Calculate new minimum distance involving this point
        double new_min = 1e9;
        for (int j = 0; j < n; j++) {
            if (j != idx) {
                new_min = min(new_min, dist(points[idx], points[j]));
            }
        }
        
        // Accept or reject based on improvement
        bool accept = false;
        double current_min = minDist(points);
        static double best_min = current_min;
        
        if (current_min > best_min || 
            (temp > 1e-6 && rand() / (double)RAND_MAX < exp((current_min - best_min) / temp))) {
            accept = true;
            best_min = max(best_min, current_min);
        }
        
        if (!accept) {
            points[idx] = old_point;
        }
        
        temp *= cooling;
    }
    
    // Final optimization: push points away from each other
    for (int iter = 0; iter < 1000; iter++) {
        for (int i = 0; i < n; i++) {
            Point force(0, 0, 0);
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    Point diff = points[i] - points[j];
                    double d = diff.norm();
                    if (d > 1e-9) {
                        force = force + diff * (1.0 / (d * d * d));
                    }
                }
            }
            
            Point new_pos = points[i] + force * 0.001;
            if (new_pos.norm() <= 1.0 + 1e-9) {
                points[i] = new_pos;
            } else {
                points[i] = new_pos.normalize();
            }
        }
    }
    
    double final_min = minDist(points);
    
    cout << fixed << setprecision(10) << final_min << "\n";
    for (const Point& p : points) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}