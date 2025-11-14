#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

const long double PI = acosl(-1.0L);

struct Vec3 {
    long double x, y, z;

    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    Vec3 operator-(const Vec3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    Vec3 operator*(long double s) const {
        return {x * s, y * s, z * s};
    }
    Vec3 operator/(long double s) const {
        return {x / s, y / s, z / s};
    }
    long double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    long double norm_sq() const {
        return x * x + y * y + z * z;
    }
    long double norm() const {
        return sqrtl(norm_sq());
    }
    void normalize() {
        long double n = norm();
        if (n > 1e-12) {
            x /= n;
            y /= n;
            z /= n;
        }
    }
};

void solve() {
    int n;
    std::cin >> n;

    std::vector<Vec3> points;

    if (n == 2) {
        points.push_back({0, 0, 1});
        points.push_back({0, 0, -1});
    } else if (n == 3) {
        points.push_back({1, 0, 0});
        points.push_back({-0.5L, sqrtl(3.0L)/2.0L, 0});
        points.push_back({-0.5L, -sqrtl(3.0L)/2.0L, 0});
    } else if (n == 4) {
        long double s = 1.0L / sqrtl(3.0L);
        points.push_back({s, s, s});
        points.push_back({s, -s, -s});
        points.push_back({-s, s, -s});
        points.push_back({-s, -s, s});
    } else if (n == 5) {
        points.push_back({0,0,1});
        points.push_back({0,0,-1});
        points.push_back({1,0,0});
        points.push_back({-0.5L, sqrtl(3.0L)/2.0L, 0});
        points.push_back({-0.5L, -sqrtl(3.0L)/2.0L, 0});
    } else if (n == 6) {
        points.push_back({1,0,0}); points.push_back({-1,0,0});
        points.push_back({0,1,0}); points.push_back({0,-1,0});
        points.push_back({0,0,1}); points.push_back({0,0,-1});
    } else if (n == 8) {
        long double s = 1.0L / sqrtl(3.0L);
        for (int i = -1; i <= 1; i += 2)
            for (int j = -1; j <= 1; j += 2)
                for (int k = -1; k <= 1; k += 2)
                    points.push_back({s*i, s*j, s*k});
    } else if (n == 12) {
        long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
        long double norm_val = sqrtl(1 + phi*phi);
        long double a = 1.0L / norm_val;
        long double b = phi / norm_val;
        points.push_back({0, a, b}); points.push_back({0, a, -b});
        points.push_back({0, -a, b}); points.push_back({0, -a, -b});
        points.push_back({b, 0, a}); points.push_back({b, 0, -a});
        points.push_back({-b, 0, a}); points.push_back({-b, 0, -a});
        points.push_back({a, b, 0}); points.push_back({a, -b, 0});
        points.push_back({-a, b, 0}); points.push_back({-a, -b, 0});
    } else if (n == 20) {
        long double s = 1.0L / sqrtl(3.0L);
        for (int i = -1; i <= 1; i += 2)
            for (int j = -1; j <= 1; j += 2)
                for (int k = -1; k <= 1; k += 2)
                    points.push_back({s*i, s*j, s*k});
    
        long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
        long double a = s / phi;
        long double b = s * phi;
        
        points.push_back({0, a, b}); points.push_back({0, a, -b});
        points.push_back({0, -a, b}); points.push_back({0, -a, -b});

        points.push_back({a, b, 0}); points.push_back({a, -b, 0});
        points.push_back({-a, b, 0}); points.push_back({-a, -b, 0});

        points.push_back({b, 0, a}); points.push_back({b, 0, -a});
        points.push_back({-b, 0, a}); points.push_back({-b, 0, -a});
    } else {
        points.resize(n);
        long double golden_angle = PI * (3.0L - sqrtl(5.0L));
        for (int i = 0; i < n; ++i) {
            long double z = 1.0L - (2.0L * (i + 0.5L)) / n;
            long double r = sqrtl(1.0L - z * z);
            long double theta = golden_angle * (i + 0.5L);
            points[i] = {r * cosl(theta), r * sinl(theta), z};
        }
        
        int iterations = 25000000LL / (1LL * n * n) + 25;
        
        long double alpha = 0.3;

        for (int iter = 0; iter < iterations; ++iter) {
            std::vector<Vec3> forces(n, {0, 0, 0});
            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    Vec3 diff = points[i] - points[j];
                    long double dist_sq = diff.norm_sq();
                    if (dist_sq < 1e-20) dist_sq = 1e-20;
                    Vec3 force = diff / (dist_sq * dist_sq);
                    forces[i] = forces[i] + force;
                    forces[j] = forces[j] - force;
                }
            }
            long double max_force_norm_sq = 0;
            for(int i = 0; i < n; ++i) {
                max_force_norm_sq = std::max(max_force_norm_sq, forces[i].norm_sq());
            }

            long double current_alpha = alpha / std::max(1.0L, sqrtl(max_force_norm_sq));

            for (int i = 0; i < n; ++i) {
                Vec3 p = points[i];
                Vec3 f = forces[i];
                Vec3 tangent_force = f - p * p.dot(f);
                p = p + tangent_force * current_alpha;
                p.normalize();
                points[i] = p;
            }
            alpha *= 0.99;
        }
    }

    long double min_dist_sq = 1e18;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            min_dist_sq = std::min(min_dist_sq, (points[i] - points[j]).norm_sq());
        }
    }

    std::cout << std::fixed << std::setprecision(15) << (double)sqrtl(min_dist_sq) << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << std::fixed << std::setprecision(15) << (double)points[i].x << " " << (double)points[i].y << " " << (double)points[i].z << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}