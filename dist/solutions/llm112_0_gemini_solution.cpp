#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// A simple 3D point/vector struct
struct Point {
    double x, y, z;

    Point() : x(0), y(0), z(0) {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {}

    Point operator+(const Point& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Point& operator+=(const Point& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Point& operator-=(const Point& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Point operator*(double s) const { return {x * s, y * s, z * s}; }
    Point& operator*=(double s) { x *= s; y *= s; z *= s; return *this; }
    Point operator/(double s) const { return {x / s, y / s, z / s}; }
    Point& operator/=(double s) { x /= s; y /= s; z /= s; return *this; }
    
    double dot(const Point& other) const { return x * other.x + y * other.y + z * other.z; }
    double norm_sq() const { return x * x + y * y + z * z; }
    double norm() const { return std::sqrt(norm_sq()); }
    
    void normalize() {
        double n = norm();
        if (n > 1e-12) {
            *this /= n;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<Point> points(n);

    // Initialize points using Fibonacci lattice for a good starting distribution
    if (n > 0) {
        const double gr = (1.0 + std::sqrt(5.0)) / 2.0;
        for (int i = 0; i < n; ++i) {
            double y_coord = 1.0 - 2.0 * (i + 0.5) / n;
            double radius = std::sqrt(1.0 - y_coord * y_coord);
            double theta = 2.0 * M_PI * i / gr;
            double x_coord = radius * std::cos(theta);
            double z_coord = radius * std::sin(theta);
            points[i] = {x_coord, y_coord, z_coord};
        }
    }
    
    if (n > 1) {
        // Use a force-directed simulation to spread points
        // Number of iterations is scaled based on N to fit within time limits
        long long total_ops = 400000000LL;
        int num_iterations = total_ops / (1LL * n * n + 1);
        num_iterations = std::max(200, std::min(5000, num_iterations));
        
        // Step size (alpha) is annealed over iterations
        double initial_alpha = 0.1 / (n * std::sqrt(static_cast<double>(n)));
        double final_alpha = 1e-9;

        for (int iter = 0; iter < num_iterations; ++iter) {
            double alpha;
            if (num_iterations > 1) {
                alpha = initial_alpha * std::pow(final_alpha / initial_alpha, static_cast<double>(iter) / (num_iterations - 1));
            } else {
                alpha = initial_alpha;
            }

            std::vector<Point> forces(n, {0, 0, 0});

            // Calculate repulsive forces between all pairs of points
            // Force is proportional to 1/d^2 (from potential -log(d))
            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    Point vec = points[i] - points[j];
                    double dist_sq = vec.norm_sq();
                    if (dist_sq < 1e-12) dist_sq = 1e-12; // Avoid division by zero
                    Point force_vec = vec / dist_sq;
                    forces[i] += force_vec;
                    forces[j] -= force_vec;
                }
            }
            
            // Apply forces to move points
            for (int i = 0; i < n; ++i) {
                Point p = points[i];
                Point f = forces[i];
                // Project force onto the tangent plane of the sphere
                Point f_tangent = f - p * p.dot(f);
                p += f_tangent * alpha;
                // Normalize point to keep it on the unit sphere
                p.normalize();
                points[i] = p;
            }
        }
    }

    // Calculate the minimum pairwise distance
    double min_dist_sq = 1e18;
    if (n > 1) {
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                min_dist_sq = std::min(min_dist_sq, (points[i] - points[j]).norm_sq());
            }
        }
    } else {
        // For n=1, any large distance is fine as there are no pairs.
        min_dist_sq = 4.0;
    }

    // Output the results
    std::cout << std::fixed << std::setprecision(15) << std::sqrt(min_dist_sq) << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << std::fixed << std::setprecision(15) << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
    }

    return 0;
}