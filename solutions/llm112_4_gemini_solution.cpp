#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <algorithm>

const double PI = acos(-1.0);

struct Point3D {
    double x, y, z;

    Point3D operator+(const Point3D& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Point3D& operator+=(const Point3D& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Point3D operator-(const Point3D& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Point3D& operator-=(const Point3D& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Point3D operator*(double s) const { return {x * s, y * s, z * s}; }
    
    double norm_sq() const { return x*x + y*y + z*z; }
    
    void normalize() {
        double n = std::sqrt(norm_sq());
        if (n > 1e-12) {
            x /= n; y /= n; z /= n;
        }
    }
};

double dot(const Point3D& a, const Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<Point3D> points(n);

    // Initialize points using Fibonacci sphere for a good starting distribution
    if (n > 0) {
        const double golden_angle = PI * (3.0 - std::sqrt(5.0));
        for (int i = 0; i < n; ++i) {
            double y = 1.0 - (2.0 * i) / (n - 1.0);
            double radius = std::sqrt(1.0 - y * y);
            double theta = golden_angle * i;
            points[i] = {std::cos(theta) * radius, y, std::sin(theta) * radius};
        }
    }

    // Determine number of iterations based on N to keep total complexity manageable
    long long ops_budget = 25000000LL;
    int iterations = 0;
    if (n > 1) {
       iterations = std::max(50, std::min(10000, (int)(ops_budget / (1LL * n * n))));
    }
    
    // Simulated annealing parameters
    double initial_alpha = 0.2;
    double final_alpha = 1e-5;
    
    double alpha = initial_alpha;
    double decay_rate = 1.0;
    if (iterations > 0) {
        decay_rate = std::pow(final_alpha / initial_alpha, 1.0 / iterations);
    }
    
    std::vector<Point3D> forces(n);

    for (int iter = 0; iter < iterations; ++iter) {
        for(int i=0; i<n; ++i) forces[i] = {0,0,0};

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Point3D diff = points[i] - points[j];
                double dist_sq = diff.norm_sq();
                if (dist_sq < 1e-14) dist_sq = 1e-14;
                
                double inv_dist = 1.0 / std::sqrt(dist_sq);
                double inv_dist_cubed = inv_dist * inv_dist * inv_dist;
                
                Point3D force_on_i = diff * inv_dist_cubed;
                forces[i] += force_on_i;
                forces[j] -= force_on_i;
            }
        }

        for (int i = 0; i < n; ++i) {
            Point3D p = points[i];
            Point3D F = forces[i];
            
            // Project force onto the tangent plane of the sphere at point p
            Point3D F_tangent = F - p * dot(F, p);
            
            points[i] += F_tangent * alpha;
            points[i].normalize();
        }

        alpha *= decay_rate;
    }

    double min_dist_sq = 1e18;
    if (n > 1) {
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                min_dist_sq = std::min(min_dist_sq, (points[i] - points[j]).norm_sq());
            }
        }
    } else {
        min_dist_sq = 4.0; 
    }

    std::cout << std::fixed << std::setprecision(15) << std::sqrt(min_dist_sq) << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << std::fixed << std::setprecision(15) << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
    }

    return 0;
}