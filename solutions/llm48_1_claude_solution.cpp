#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y, z - p.z);
    }
    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }
};

double distToFace(const Point& p) {
    return min({p.x, p.y, p.z, 1-p.x, 1-p.y, 1-p.z});
}

double computeRadius(const vector<Point>& points) {
    double minDist = 1e9;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            minDist = min(minDist, (points[i] - points[j]).norm());
        }
        minDist = min(minDist, 2 * distToFace(points[i]));
    }
    
    return minDist / 2;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> points;
    
    if (n <= 8) {
        // Corner placement for small n
        points.push_back(Point(0.2, 0.2, 0.2));
        if (n > 1) points.push_back(Point(0.8, 0.2, 0.2));
        if (n > 2) points.push_back(Point(0.2, 0.8, 0.2));
        if (n > 3) points.push_back(Point(0.2, 0.2, 0.8));
        if (n > 4) points.push_back(Point(0.8, 0.8, 0.2));
        if (n > 5) points.push_back(Point(0.8, 0.2, 0.8));
        if (n > 6) points.push_back(Point(0.2, 0.8, 0.8));
        if (n > 7) points.push_back(Point(0.8, 0.8, 0.8));
    } else {
        // Grid-based approach for larger n
        int gridSize = ceil(pow(n, 1.0/3));
        while (gridSize * gridSize * gridSize < n) gridSize++;
        
        double spacing = 1.0 / gridSize;
        double offset = spacing / 2;
        
        int count = 0;
        for (int i = 0; i < gridSize && count < n; i++) {
            for (int j = 0; j < gridSize && count < n; j++) {
                for (int k = 0; k < gridSize && count < n; k++) {
                    points.push_back(Point(
                        offset + i * spacing,
                        offset + j * spacing,
                        offset + k * spacing
                    ));
                    count++;
                }
            }
        }
    }
    
    // Simulated annealing improvement
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    
    double currentRadius = computeRadius(points);
    double temperature = 0.1;
    
    for (int iter = 0; iter < 10000; iter++) {
        vector<Point> newPoints = points;
        int idx = gen() % n;
        
        double delta = 0.05 * temperature;
        newPoints[idx].x = max(0.0, min(1.0, newPoints[idx].x + (dis(gen) - 0.5) * delta));
        newPoints[idx].y = max(0.0, min(1.0, newPoints[idx].y + (dis(gen) - 0.5) * delta));
        newPoints[idx].z = max(0.0, min(1.0, newPoints[idx].z + (dis(gen) - 0.5) * delta));
        
        double newRadius = computeRadius(newPoints);
        
        if (newRadius > currentRadius || dis(gen) < exp((newRadius - currentRadius) / temperature)) {
            points = newPoints;
            currentRadius = newRadius;
        }
        
        temperature *= 0.9999;
    }
    
    for (const Point& p : points) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << "\n";
    }
    
    return 0;
}