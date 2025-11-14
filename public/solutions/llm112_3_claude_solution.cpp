#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double norm() const { return sqrt(x * x + y * y + z * z); }
    Point normalize() const { double n = norm(); return n > 0 ? *this * (1.0 / n) : *this; }
};

double dist(const Point& a, const Point& b) {
    Point d = a - b;
    return d.norm();
}

double getMinDist(const vector<Point>& points) {
    double minDist = 1e9;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            minDist = min(minDist, dist(points[i], points[j]));
        }
    }
    return minDist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> points(n);
    
    // Initialize random points on sphere
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);
    uniform_real_distribution<> angle(0, 2 * M_PI);
    uniform_real_distribution<> costheta(-1.0, 1.0);
    
    for (int i = 0; i < n; i++) {
        double theta = angle(gen);
        double phi = acos(costheta(gen));
        points[i].x = sin(phi) * cos(theta);
        points[i].y = sin(phi) * sin(theta);
        points[i].z = cos(phi);
    }
    
    // Simulated annealing
    double temp = 1.0;
    double cooling = 0.99;
    
    for (int iter = 0; iter < 100000; iter++) {
        int idx = gen() % n;
        Point old_point = points[idx];
        
        // Generate new random point on sphere
        double theta = angle(gen);
        double phi = acos(costheta(gen));
        Point new_point;
        new_point.x = sin(phi) * cos(theta);
        new_point.y = sin(phi) * sin(theta);
        new_point.z = cos(phi);
        
        points[idx] = new_point;
        double new_min = getMinDist(points);
        
        // Also try small perturbation
        Point perturb = old_point;
        perturb.x += dis(gen) * temp * 0.1;
        perturb.y += dis(gen) * temp * 0.1;
        perturb.z += dis(gen) * temp * 0.1;
        
        if (perturb.norm() <= 1.0) {
            points[idx] = perturb;
            double perturb_min = getMinDist(points);
            if (perturb_min > new_min) {
                new_min = perturb_min;
            } else {
                points[idx] = new_point;
            }
        }
        
        double old_min = getMinDist({points.begin(), points.begin() + idx});
        for (int i = idx + 1; i < n; i++) {
            old_min = min(old_min, dist(old_point, points[i]));
        }
        for (int i = 0; i < idx; i++) {
            old_min = min(old_min, dist(old_point, points[i]));
        }
        
        if (new_min < old_min && exp((new_min - old_min) / temp) < uniform_real_distribution<>(0, 1)(gen)) {
            points[idx] = old_point;
        }
        
        temp *= cooling;
    }
    
    // Local optimization
    for (int iter = 0; iter < 1000; iter++) {
        bool improved = false;
        for (int i = 0; i < n; i++) {
            Point best = points[i];
            double best_min = getMinDist(points);
            
            for (int trial = 0; trial < 20; trial++) {
                double theta = angle(gen);
                double phi = acos(costheta(gen));
                points[i].x = sin(phi) * cos(theta);
                points[i].y = sin(phi) * sin(theta);
                points[i].z = cos(phi);
                
                double curr_min = getMinDist(points);
                if (curr_min > best_min) {
                    best = points[i];
                    best_min = curr_min;
                    improved = true;
                }
            }
            points[i] = best;
        }
        if (!improved) break;
    }
    
    double min_dist = getMinDist(points);
    
    cout << fixed << setprecision(10) << min_dist << "\n";
    for (const Point& p : points) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}