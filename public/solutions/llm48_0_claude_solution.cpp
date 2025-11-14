#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double norm() const { return sqrt(x * x + y * y + z * z); }
};

double distToBoundary(const Point& p) {
    return min({p.x, p.y, p.z, 1.0 - p.x, 1.0 - p.y, 1.0 - p.z});
}

double computeRadius(const vector<Point>& centers) {
    double minDist = 1e9;
    int n = centers.size();
    
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, distToBoundary(centers[i]));
        for (int j = i + 1; j < n; j++) {
            double dist = (centers[i] - centers[j]).norm() * 0.5;
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

vector<Point> createGrid(int n) {
    vector<Point> centers;
    
    // Find best grid dimensions
    int bestM = 1, bestK = 1, bestL = 1;
    double bestRadius = 0;
    
    for (int m = 1; m <= n && m <= 20; m++) {
        for (int k = 1; k * m <= n && k <= 20; k++) {
            int l = (n + m * k - 1) / (m * k);
            if (l <= 20) {
                double radius = 1.0 / (2.0 * max({m, k, l}));
                if (radius > bestRadius) {
                    bestRadius = radius;
                    bestM = m;
                    bestK = k;
                    bestL = l;
                }
            }
        }
    }
    
    double dx = 1.0 / bestM;
    double dy = 1.0 / bestK;
    double dz = 1.0 / bestL;
    
    int count = 0;
    for (int i = 0; i < bestM && count < n; i++) {
        for (int j = 0; j < bestK && count < n; j++) {
            for (int k = 0; k < bestL && count < n; k++) {
                centers.push_back(Point(
                    (i + 0.5) * dx,
                    (j + 0.5) * dy,
                    (k + 0.5) * dz
                ));
                count++;
            }
        }
    }
    
    return centers;
}

void improve(vector<Point>& centers, int iterations = 1000) {
    int n = centers.size();
    double step = 0.01;
    
    for (int iter = 0; iter < iterations; iter++) {
        vector<Point> forces(n, Point(0, 0, 0));
        
        // Repulsion between spheres
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                Point diff = centers[i] - centers[j];
                double dist = diff.norm();
                if (dist > 0) {
                    Point force = diff * (1.0 / (dist * dist * dist));
                    forces[i] = forces[i] + force;
                    forces[j] = forces[j] - force;
                }
            }
        }
        
        // Boundary repulsion
        for (int i = 0; i < n; i++) {
            const double margin = 0.05;
            if (centers[i].x < margin) forces[i].x += (margin - centers[i].x) * 10;
            if (centers[i].x > 1 - margin) forces[i].x -= (centers[i].x - (1 - margin)) * 10;
            if (centers[i].y < margin) forces[i].y += (margin - centers[i].y) * 10;
            if (centers[i].y > 1 - margin) forces[i].y -= (centers[i].y - (1 - margin)) * 10;
            if (centers[i].z < margin) forces[i].z += (margin - centers[i].z) * 10;
            if (centers[i].z > 1 - margin) forces[i].z -= (centers[i].z - (1 - margin)) * 10;
        }
        
        // Update positions
        for (int i = 0; i < n; i++) {
            centers[i] = centers[i] + forces[i] * step;
            centers[i].x = max(0.0, min(1.0, centers[i].x));
            centers[i].y = max(0.0, min(1.0, centers[i].y));
            centers[i].z = max(0.0, min(1.0, centers[i].z));
        }
        
        step *= 0.999;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> centers = createGrid(n);
    improve(centers);
    
    cout << fixed << setprecision(10);
    for (const Point& p : centers) {
        cout << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}