#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double dist(const Point& p) const {
        double dx = x - p.x, dy = y - p.y, dz = z - p.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }
    double distToBoundary() const {
        return min({x, y, z, 1.0 - x, 1.0 - y, 1.0 - z});
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Point> centers;
    
    // Start with grid-based initialization
    int side = ceil(cbrt(n));
    double step = 1.0 / side;
    double offset = step / 2.0;
    
    for (int i = 0; i < side && centers.size() < n; i++) {
        for (int j = 0; j < side && centers.size() < n; j++) {
            for (int k = 0; k < side && centers.size() < n; k++) {
                centers.push_back(Point(offset + i * step, offset + j * step, offset + k * step));
            }
        }
    }
    
    // Add random points if needed
    srand(42);
    while (centers.size() < n) {
        centers.push_back(Point((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX));
    }
    
    // Simulated annealing optimization
    double temp = 0.1;
    double cooling = 0.999;
    int iterations = 50000;
    
    auto computeRadius = [&](const vector<Point>& pts) {
        double minDist = 1e9;
        for (int i = 0; i < pts.size(); i++) {
            minDist = min(minDist, pts[i].distToBoundary());
            for (int j = i + 1; j < pts.size(); j++) {
                minDist = min(minDist, pts[i].dist(pts[j]) / 2.0);
            }
        }
        return minDist;
    };
    
    double currentRadius = computeRadius(centers);
    
    for (int iter = 0; iter < iterations; iter++) {
        vector<Point> newCenters = centers;
        int idx = rand() % n;
        
        // Small random perturbation
        double dx = (double(rand()) / RAND_MAX - 0.5) * temp;
        double dy = (double(rand()) / RAND_MAX - 0.5) * temp;
        double dz = (double(rand()) / RAND_MAX - 0.5) * temp;
        
        newCenters[idx].x = max(0.0, min(1.0, newCenters[idx].x + dx));
        newCenters[idx].y = max(0.0, min(1.0, newCenters[idx].y + dy));
        newCenters[idx].z = max(0.0, min(1.0, newCenters[idx].z + dz));
        
        double newRadius = computeRadius(newCenters);
        
        if (newRadius > currentRadius || 
            (temp > 1e-6 && double(rand()) / RAND_MAX < exp((newRadius - currentRadius) / temp))) {
            centers = newCenters;
            currentRadius = newRadius;
        }
        
        temp *= cooling;
    }
    
    // Local improvement
    for (int rounds = 0; rounds < 100; rounds++) {
        bool improved = false;
        for (int i = 0; i < n; i++) {
            Point best = centers[i];
            double bestRadius = currentRadius;
            
            for (double dx = -0.01; dx <= 0.01; dx += 0.005) {
                for (double dy = -0.01; dy <= 0.01; dy += 0.005) {
                    for (double dz = -0.01; dz <= 0.01; dz += 0.005) {
                        Point candidate = Point(centers[i].x + dx, centers[i].y + dy, centers[i].z + dz);
                        if (candidate.x >= 0 && candidate.x <= 1 && 
                            candidate.y >= 0 && candidate.y <= 1 && 
                            candidate.z >= 0 && candidate.z <= 1) {
                            
                            centers[i] = candidate;
                            double newRadius = computeRadius(centers);
                            if (newRadius > bestRadius) {
                                best = candidate;
                                bestRadius = newRadius;
                                improved = true;
                            }
                        }
                    }
                }
            }
            centers[i] = best;
            currentRadius = bestRadius;
        }
        if (!improved) break;
    }
    
    for (const Point& p : centers) {
        cout << fixed << setprecision(12) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}