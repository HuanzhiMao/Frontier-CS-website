#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits>

struct Point3D_int {
    int x, y, z;
};

struct Point3D_double {
    double x, y, z;
};

long long dist_sq(const Point3D_int& p) {
    return (long long)p.x * p.x + (long long)p.y * p.y + (long long)p.z * p.z;
}

long long dist_sq_p2p(const Point3D_int& p1, const Point3D_int& p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    long long dz = p1.z - p2.z;
    return dx * dx + dy * dy + dz * dz;
}

std::vector<Point3D_int> generate_fcc(int n) {
    std::vector<Point3D_int> points;
    if (n > 0) points.push_back({0,0,0});
    int L = 0;
    while (points.size() < n) {
        L++;
        for (int i = -L; i <= L; ++i) {
            for (int j = -L; j <= L; ++j) {
                for (int k = -L; k <= L; ++k) {
                    if (std::max({std::abs(i), std::abs(j), std::abs(k)}) == L) {
                        if ((i + j + k) % 2 == 0) {
                            points.push_back({i, j, k});
                        }
                    }
                }
            }
        }
    }
    std::sort(points.begin(), points.end(), [](const Point3D_int& a, const Point3D_int& b) {
        long long d_a = dist_sq(a);
        long long d_b = dist_sq(b);
        if (d_a != d_b) return d_a < d_b;
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    });
    points.resize(n);
    return points;
}

std::vector<Point3D_int> generate_bcc(int n) {
    std::vector<Point3D_int> points;
    if (n > 0) points.push_back({0,0,0});
    int L = 0;
    while (points.size() < n) {
        L++;
        for (int i = -L; i <= L; ++i) {
            for (int j = -L; j <= L; ++j) {
                for (int k = -L; k <= L; ++k) {
                    if (std::max({std::abs(i), std::abs(j), std::abs(k)}) == L) {
                        if ((std::abs(i) % 2) == (std::abs(j) % 2) && (std::abs(j) % 2) == (std::abs(k) % 2)) {
                             points.push_back({i, j, k});
                        }
                    }
                }
            }
        }
    }
    std::sort(points.begin(), points.end(), [](const Point3D_int& a, const Point3D_int& b) {
        long long d_a = dist_sq(a);
        long long d_b = dist_sq(b);
        if (d_a != d_b) return d_a < d_b;
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    });
    points.resize(n);
    return points;
}

std::pair<double, std::vector<Point3D_double>> solve_lattice(int n, const std::vector<Point3D_int>& points) {
    if (n == 0) return {0.0, {}};
    if (n == 1) return {0.5, {{0.5, 0.5, 0.5}}};

    long long min_d2 = std::numeric_limits<long long>::max();
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            min_d2 = std::min(min_d2, dist_sq_p2p(points[i], points[j]));
        }
    }
    double min_dist = std::sqrt(min_d2);

    int min_x = points[0].x, max_x = points[0].x;
    int min_y = points[0].y, max_y = points[0].y;
    int min_z = points[0].z, max_z = points[0].z;

    for (const auto& p : points) {
        min_x = std::min(min_x, p.x);
        max_x = std::max(max_x, p.x);
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
        min_z = std::min(min_z, p.z);
        max_z = std::max(max_z, p.z);
    }

    double sx = max_x - min_x;
    double sy = max_y - min_y;
    double sz = max_z - min_z;
    double S = std::max({sx, sy, sz});

    if (S == 0) { // All points are the same, for n>1 this means min_dist=0
        if (n > 1) return {0.0, std::vector<Point3D_double>(n, {0.5, 0.5, 0.5})};
    }

    double r = min_dist / (2.0 * (S + min_dist));
    double s = 1.0 / (S + min_dist);

    std::vector<Point3D_double> centers;
    centers.reserve(n);
    for (const auto& p : points) {
        centers.push_back({
            s * (p.x - min_x) + (1.0 - s * sx) / 2.0,
            s * (p.y - min_y) + (1.0 - s * sy) / 2.0,
            s * (p.z - min_z) + (1.0 - s * sz) / 2.0
        });
    }

    return {r, centers};
}

std::pair<double, std::vector<Point3D_double>> solve_sc(int n) {
    if (n == 0) return {0.0, {}};
    if (n == 1) return {0.5, {{0.5, 0.5, 0.5}}};

    int best_m = n, best_k = 1, best_l = 1;
    int best_max_dim = n;

    for (int m = 1; m * m * m <= n; ++m) {
        for (int k = m; m * k <= n; ++k) {
            int l = std::ceil((double)n / (m * k));
            if (std::max({m, k, l}) < best_max_dim) {
                best_max_dim = std::max({m, k, l});
                best_m = m;
                best_k = k;
                best_l = l;
            }
        }
    }
    
    int m = best_m, k = best_k, l = best_l;
    
    double r = 1.0 / (2.0 * std::max({m, k, l}));
    
    std::vector<Point3D_double> centers;
    centers.reserve(n);
    for (int i = 0; i < m && centers.size() < n; ++i) {
        for (int j = 0; j < k && centers.size() < n; ++j) {
            for (int p = 0; p < l && centers.size() < n; ++p) {
                centers.push_back({
                    (2.0 * i + 1.0) / (2.0 * m),
                    (2.0 * j + 1.0) / (2.0 * k),
                    (2.0 * p + 1.0) / (2.0 * l)
                });
            }
        }
    }

    return {r, centers};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    double best_r = 0.0;
    std::vector<Point3D_double> best_centers;

    auto res_sc = solve_sc(n);
    if (res_sc.first > best_r) {
        best_r = res_sc.first;
        best_centers = res_sc.second;
    }
    
    auto fcc_points = generate_fcc(n);
    auto res_fcc = solve_lattice(n, fcc_points);
    if (res_fcc.first > best_r) {
        best_r = res_fcc.first;
        best_centers = res_fcc.second;
    }
    
    auto bcc_points = generate_bcc(n);
    auto res_bcc = solve_lattice(n, bcc_points);
    if (res_bcc.first > best_r) {
        best_r = res_bcc.first;
        best_centers = res_bcc.second;
    }

    std::cout << std::fixed << std::setprecision(17);
    for (const auto& center : best_centers) {
        std::cout << center.x << " " << center.y << " " << center.z << "\n";
    }

    return 0;
}