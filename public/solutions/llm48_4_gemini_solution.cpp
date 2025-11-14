#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const double SQRT2 = sqrt(2.0);

struct Point3D {
    double x, y, z;
};

struct Point3I {
    int i, j, k;
    int dist_sq;
};

// Comparator for sorting lattice points by distance from origin
bool comparePoints(const Point3I& a, const Point3I& b) {
    if (a.dist_sq != b.dist_sq) {
        return a.dist_sq < b.dist_sq;
    }
    if (a.i != b.i) return a.i < b.i;
    if (a.j != b.j) return a.j < b.j;
    return a.k < b.k;
}

// Baseline solution: packing on a simple cubic grid
std::pair<double, std::vector<Point3D>> solve_grid(int n) {
    if (n == 0) return {0.0, {}};
    if (n == 1) return {0.5, {{0.5, 0.5, 0.5}}};

    int best_m = n, best_k = 1, best_l = 1;
    int min_max_dim = n;

    int m_limit = static_cast<int>(cbrt(n)) + 2;
    for (int m = 1; m <= m_limit; ++m) {
        if ((long long)m * m * m > n * 2 && m > 10) continue; // Optimization
        int k_limit = static_cast<int>(sqrt(n / (double)m)) + 2;
        for (int k = m; k <= k_limit; ++k) {
            if ((long long)m * k > n * 2 && k > 10) continue; // Optimization
            int l = (n + (long long)m * k - 1) / ((long long)m * k);
            int max_dim = std::max({m, k, l});
            if (max_dim < min_max_dim) {
                min_max_dim = max_dim;
                best_m = m;
                best_k = k;
                best_l = l;
            }
        }
    }
    
    double radius = 1.0 / (2.0 * min_max_dim);
    std::vector<Point3D> centers;
    centers.reserve(n);
    for (int i = 0; i < best_m && centers.size() < n; ++i) {
        for (int j = 0; j < best_k && centers.size() < n; ++j) {
            for (int p = 0; p < best_l && centers.size() < n; ++p) {
                centers.push_back({
                    (i + 0.5) / best_m,
                    (j + 0.5) / best_k,
                    (p + 0.5) / best_l
                });
            }
        }
    }
    return {radius, centers};
}

// FCC lattice packing
std::pair<double, std::vector<Point3D>> solve_fcc(int n) {
    if (n == 0) return {0.0, {}};
    if (n == 1) return {0.5, {{0.5, 0.5, 0.5}}};
    
    std::vector<Point3I> lattice_points;
    int d_max = 0;
    if (n > 1) {
        // Estimate a search cube radius that's guaranteed to contain at least N points
        d_max = static_cast<int>(ceil(0.5 * (pow(2.0*n, 1.0/3.0) - 1.0))) + 1;
    }

    for (int i = -d_max; i <= d_max; ++i) {
        for (int j = -d_max; j <= d_max; ++j) {
            for (int k = -d_max; k <= d_max; ++k) {
                if ((i + j + k) % 2 == 0) {
                    lattice_points.push_back({i, j, k, i * i + j * j + k * k});
                }
            }
        }
    }

    std::sort(lattice_points.begin(), lattice_points.end(), comparePoints);
    
    std::vector<Point3I> points(lattice_points.begin(), lattice_points.begin() + n);

    int i_min = points[0].i, i_max = points[0].i;
    int j_min = points[0].j, j_max = points[0].j;
    int k_min = points[0].k, k_max = points[0].k;

    for (size_t idx = 1; idx < points.size(); ++idx) {
        i_min = std::min(i_min, points[idx].i);
        i_max = std::max(i_max, points[idx].i);
        j_min = std::min(j_min, points[idx].j);
        j_max = std::max(j_max, points[idx].j);
        k_min = std::min(k_min, points[idx].k);
        k_max = std::max(k_max, points[idx].k);
    }
    
    int L = std::max({i_max - i_min, j_max - j_min, k_max - k_min});
    if (L == 0 && n==1) L = 1; // Special case for n=1 to avoid division by zero
    double s = 1.0 / (L + SQRT2);
    double radius = s * SQRT2 / 2.0;
    
    double i_center = (i_min + i_max) / 2.0;
    double j_center = (j_min + j_max) / 2.0;
    double k_center = (k_min + k_max) / 2.0;

    std::vector<Point3D> centers;
    centers.reserve(n);
    for (int i = 0; i < n; ++i) {
        centers.push_back({
            (points[i].i - i_center) * s + 0.5,
            (points[i].j - j_center) * s + 0.5,
            (points[i].k - k_center) * s + 0.5
        });
    }

    return {radius, centers};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    auto grid_solution = solve_grid(n);
    auto fcc_solution = solve_fcc(n);

    std::cout << std::fixed << std::setprecision(17);

    if (grid_solution.first > fcc_solution.first) {
        for (const auto& p : grid_solution.second) {
            std::cout << p.x << " " << p.y << " " << p.z << "\n";
        }
    } else {
        for (const auto& p : fcc_solution.second) {
            std::cout << p.x << " " << p.y << " " << p.z << "\n";
        }
    }

    return 0;
}