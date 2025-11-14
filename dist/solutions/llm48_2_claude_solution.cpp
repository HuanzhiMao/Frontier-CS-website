#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double t) const { return Point(x * t, y * t, z * t); }
    double norm() const { return sqrt(x * x + y * y + z * z); }
    double distToBoundary() const {
        return min({x, 1 - x, y, 1 - y, z, 1 - z});
    }
};

double getRRadius(const vector<Point>& centers) {
    double minDist = 1e9;
    int n = centers.size();
    
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, 2 * centers[i].distToBoundary());
        for (int j = i + 1; j < n; j++) {
            minDist = min(minDist, (centers[i] - centers[j]).norm());
        }
    }
    return minDist / 2;
}

vector<Point> generateGrid(int n) {
    vector<Point> centers;
    
    int m = ceil(pow(n, 1.0/3));
    while (m * m * m < n) m++;
    
    double spacing = 1.0 / m;
    double offset = spacing / 2;
    
    for (int i = 0; i < m && centers.size() < n; i++) {
        for (int j = 0; j < m && centers.size() < n; j++) {
            for (int k = 0; k < m && centers.size() < n; k++) {
                centers.push_back(Point(
                    offset + i * spacing,
                    offset + j * spacing,
                    offset + k * spacing
                ));
            }
        }
    }
    
    return centers;
}

void optimizePacking(vector<Point>& centers, int iterations = 1000) {
    int n = centers.size();
    double step = 0.01;
    
    for (int iter = 0; iter < iterations; iter++) {
        vector<Point> forces(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                Point diff = centers[i] - centers[j];
                double dist = diff.norm();
                if (dist > 0) {
                    forces[i] = forces[i] + diff * (1.0 / (dist * dist * dist));
                }
            }
            
            // Force to stay away from boundaries
            forces[i].x += -1.0 / (centers[i].x * centers[i].x) + 1.0 / ((1 - centers[i].x) * (1 - centers[i].x));
            forces[i].y += -1.0 / (centers[i].y * centers[i].y) + 1.0 / ((1 - centers[i].y) * (1 - centers[i].y));
            forces[i].z += -1.0 / (centers[i].z * centers[i].z) + 1.0 / ((1 - centers[i].z) * (1 - centers[i].z));
        }
        
        double currentStep = step * exp(-iter * 0.001);
        
        for (int i = 0; i < n; i++) {
            Point newPos = centers[i] + forces[i] * currentStep;
            newPos.x = max(0.001, min(0.999, newPos.x));
            newPos.y = max(0.001, min(0.999, newPos.y));
            newPos.z = max(0.001, min(0.999, newPos.z));
            centers[i] = newPos;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> centers = generateGrid(n);
    optimizePacking(centers, min(2000, 10000 / n));
    
    for (const auto& p : centers) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}