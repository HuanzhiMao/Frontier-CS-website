#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }
    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }
};

double distToBoundary(const Point& p) {
    return min({p.x, 1.0 - p.x, p.y, 1.0 - p.y, p.z, 1.0 - p.z});
}

double computeRadius(const vector<Point>& centers) {
    int n = centers.size();
    double minPairDist = 1e9;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = (centers[i] - centers[j]).norm();
            minPairDist = min(minPairDist, dist);
        }
    }
    
    double minBoundaryDist = 1e9;
    for (int i = 0; i < n; i++) {
        minBoundaryDist = min(minBoundaryDist, distToBoundary(centers[i]));
    }
    
    return min(minPairDist / 2.0, minBoundaryDist);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> centers;
    
    // Find optimal grid dimensions
    int bestM = 1, bestK = 1, bestL = 1;
    double bestRadius = 0;
    
    for (int m = 1; m <= n; m++) {
        for (int k = 1; k * m <= n; k++) {
            int l = (n + m*k - 1) / (m*k);
            if (m*k*l >= n) {
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
    
    // Generate grid centers
    centers.clear();
    double stepX = 1.0 / bestM;
    double stepY = 1.0 / bestK;
    double stepZ = 1.0 / bestL;
    
    for (int i = 0; i < bestM && centers.size() < n; i++) {
        for (int j = 0; j < bestK && centers.size() < n; j++) {
            for (int k = 0; k < bestL && centers.size() < n; k++) {
                double x = stepX * (i + 0.5);
                double y = stepY * (j + 0.5);
                double z = stepZ * (k + 0.5);
                centers.push_back(Point(x, y, z));
            }
        }
    }
    
    // Simple local optimization
    for (int iter = 0; iter < 100; iter++) {
        bool improved = false;
        for (int i = 0; i < n; i++) {
            Point best = centers[i];
            double bestR = computeRadius(centers);
            
            for (double dx = -0.01; dx <= 0.01; dx += 0.005) {
                for (double dy = -0.01; dy <= 0.01; dy += 0.005) {
                    for (double dz = -0.01; dz <= 0.01; dz += 0.005) {
                        Point newPos(centers[i].x + dx, centers[i].y + dy, centers[i].z + dz);
                        if (newPos.x >= 0 && newPos.x <= 1 && 
                            newPos.y >= 0 && newPos.y <= 1 && 
                            newPos.z >= 0 && newPos.z <= 1) {
                            
                            centers[i] = newPos;
                            double newR = computeRadius(centers);
                            if (newR > bestR) {
                                best = newPos;
                                bestR = newR;
                                improved = true;
                            }
                        }
                    }
                }
            }
            centers[i] = best;
        }
        if (!improved) break;
    }
    
    for (const Point& p : centers) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}