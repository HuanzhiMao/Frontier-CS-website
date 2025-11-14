#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <algorithm>

const double PI = acos(-1.0);

struct Point {
    double x = 0, y = 0, z = 0;

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    Point operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
    double dot(const Point& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    double mag_sq() const {
        return x * x + y * y + z * z;
    }
    double mag() const {
        return sqrt(mag_sq());
    }
    void normalize() {
        double m = mag();
        if (m > 1e-12) {
            x /= m;
            y /= m;
            z /= m;
        }
    }
};

std::vector<Point> points;

void solve_hardcoded(int n) {
    points.resize(n);
    if (n == 2) {
        points[0] = {0, 0, 1};
        points[1] = {0, 0, -1};
    } else if (n == 3) {
        points[0] = {1, 0, 0};
        points[1] = {-0.5, sqrt(3.0) / 2.0, 0};
        points[2] = {-0.5, -sqrt(3.0) / 2.0, 0};
    } else if (n == 4) {
        double val = 1.0 / sqrt(3.0);
        points[0] = {val, val, val};
        points[1] = {val, -val, -val};
        points[2] = {-val, val, -val};
        points[3] = {-val, -val, val};
    } else if (n == 6) {
        points[0] = {1, 0, 0};
        points[1] = {-1, 0, 0};
        points[2] = {0, 1, 0};
        points[3] = {0, -1, 0};
        points[4] = {0, 0, 1};
        points[5] = {0, 0, -1};
    } else if (n == 12) {
        double phi = (1.0 + sqrt(5.0)) / 2.0;
        double norm_inv = 1.0 / sqrt(1 + phi * phi);
        
        points[0] = {norm_inv * 1, norm_inv * phi, 0};
        points[1] = {norm_inv * -1, norm_inv * phi, 0};
        points[2] = {norm_inv * 1, norm_inv * -phi, 0};
        points[3] = {norm_inv * -1, norm_inv * -phi, 0};

        points[4] = {0, norm_inv * 1, norm_inv * phi};
        points[5] = {0, norm_inv * -1, norm_inv * phi};
        points[6] = {0, norm_inv * 1, norm_inv * -phi};
        points[7] = {0, norm_inv * -1, norm_inv * -phi};

        points[8] = {norm_inv * phi, 0, norm_inv * 1};
        points[9] = {norm_inv * -phi, 0, norm_inv * 1};
        points[10] = {norm_inv * phi, 0, norm_inv * -1};
        points[11] = {norm_inv * -phi, 0, norm_inv * -1};
    }
}

void solve_general(int n) {
    points.resize(n);

    // Fibonacci lattice initialization
    double golden_angle = PI * (3.0 - sqrt(5.0));
    for (int i = 0; i < n; ++i) {
        double z = 1.0 - (2.0 * i) / (n - 1.0);
        double radius = sqrt(1.0 - z * z);
        double theta = golden_angle * i;
        double x = cos(theta) * radius;
        double y = sin(theta) * radius;
        points[i] = {x, y, z};
    }

    // Physics simulation
    int iterations = std::min(2000, std::max(200, 400000 / n));
    double alpha0 = 0.8 / sqrt(n);

    std::vector<Point> forces(n);
    for (int iter = 0; iter < iterations; ++iter) {
        double decay_factor = 1.0 - (double)iter / iterations;
        double alpha = alpha0 * decay_factor * decay_factor;
        std::fill(forces.begin(), forces.end(), Point{0,0,0});

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Point diff = points[i] - points[j];
                double r_sq = diff.mag_sq();
                if (r_sq < 1e-12) r_sq = 1e-12;
                double r = sqrt(r_sq);
                Point force = diff * (1.0 / (r * r_sq));
                forces[i] = forces[i] + force;
                forces[j] = forces[j] - force;
            }
        }

        for (int i = 0; i < n; ++i) {
            Point tangent_force = forces[i] - (points[i] * forces[i].dot(points[i]));
            points[i] = points[i] + tangent_force * alpha;
            points[i].normalize();
        }
    }
}

void calculate_and_print(int n) {
    double min_dist_sq = 1e18;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            min_dist_sq = std::min(min_dist_sq, (points[i] - points[j]).mag_sq());
        }
    }
    
    double min_dist = sqrt(min_dist_sq);

    std::cout << std::fixed << std::setprecision(15) << min_dist << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << std::fixed << std::setprecision(15) << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    
    if (n==2 || n==3 || n==4 || n==6 || n==12) {
        solve_hardcoded(n);
    } else {
        solve_general(n);
    }
    
    calculate_and_print(n);

    return 0;
}