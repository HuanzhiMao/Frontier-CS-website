#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <algorithm>

const double PI = acos(-1.0);

// A simple 3D vector struct with basic operations.
struct Vec3 {
    double x = 0.0, y = 0.0, z = 0.0;

    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    double norm_sq() const {
        return x * x + y * y + z * z;
    }

    double norm() const {
        return sqrt(norm_sq());
    }

    void normalize() {
        double n = norm();
        if (n > 1e-12) {
            x /= n; y /= n; z /= n;
        }
    }
};

Vec3 operator+(Vec3 a, const Vec3& b) { a += b; return a; }
Vec3 operator-(Vec3 a, const Vec3& b) { a -= b; return a; }
Vec3 operator*(Vec3 a, double s) { a.x *= s; a.y *= s; a.z *= s; return a; }
Vec3 operator*(double s, Vec3 a) { return a * s; }
double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Initializes points on a sphere using the Fibonacci lattice method.
// This provides a good, uniform starting distribution.
void fibonacci_sphere(std::vector<Vec3>& points, int n) {
    const double golden_angle = PI * (3.0 - sqrt(5.0));
    for (int i = 0; i < n; ++i) {
        double y = 1.0 - (2.0 * i) / (n - 1);
        double radius = sqrt(1.0 - y * y);
        double theta = golden_angle * i;
        double x = cos(theta) * radius;
        double z = sin(theta) * radius;
        points[i] = {x, y, z};
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // A simple hardcoded case for n=2, which is optimal.
    if (n == 2) {
        std::cout << std::fixed << std::setprecision(15) << 2.0 << std::endl;
        std::cout << std::fixed << std::setprecision(15) << 0.0 << " " << 0.0 << " " << 1.0 << std::endl;
        std::cout << std::fixed << std::setprecision(15) << 0.0 << " " << 0.0 << " " << -1.0 << std::endl;
        return 0;
    }

    std::vector<Vec3> points(n);
    fibonacci_sphere(points, n);
    
    // Heuristics for simulation parameters. Tuned to balance quality and time limit.
    int max_iter = std::min(5000, 200000 / n + 300);
    double initial_step_size = 0.2 * pow(n, -0.75);

    std::vector<Vec3> forces(n);
    for (int iter = 0; iter < max_iter; ++iter) {
        for (int i = 0; i < n; ++i) {
            forces[i] = {0, 0, 0};
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Vec3 diff = points[i] - points[j];
                double dist_sq = diff.norm_sq();
                if (dist_sq < 1e-18) continue;
                
                // Repulsive force proportional to 1/d^2, so vector is diff/d^3
                double inv_dist = 1.0 / sqrt(dist_sq);
                double magnitude = inv_dist * inv_dist * inv_dist;
                
                Vec3 force_ij = diff * magnitude;
                forces[i] += force_ij;
                forces[j] -= force_ij;
            }
        }

        // Use a decaying step size. Quadratic decay works well.
        double step_size = initial_step_size * pow(1.0 - (double)iter / max_iter, 2.0);

        for (int i = 0; i < n; ++i) {
            // Project force onto the tangent plane at the point's position
            Vec3 tangent_force = forces[i] - points[i] * dot(forces[i], points[i]);
            
            // Move the point along the tangent direction
            points[i] += tangent_force * step_size;

            // Project the point back onto the sphere surface
            points[i].normalize();
        }
    }

    // Calculate the minimum distance between any pair of points.
    double min_dist_sq = 1e18;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            min_dist_sq = std::min(min_dist_sq, (points[i] - points[j]).norm_sq());
        }
    }
    double min_dist = sqrt(min_dist_sq);

    // Output the result.
    std::cout << std::fixed << std::setprecision(15) << min_dist << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << std::fixed << std::setprecision(15) << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
    }

    return 0;
}