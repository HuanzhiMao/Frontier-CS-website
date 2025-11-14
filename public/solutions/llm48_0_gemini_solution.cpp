#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <tuple>

const double SQRT2 = sqrt(2.0);
const double SQRT3 = sqrt(3.0);

struct Point {
    double x, y, z;
};

// Find m_x, m_y, m_z such that m_x*m_y*m_z >= N and max(m_x,m_y,m_z) is minimized.
std::tuple<int, int, int> find_dims(long long N) {
    if (N <= 0) N = 1;
    long long best_mx = N, best_my = 1, best_mz = 1;
    long long best_max_dim = N;

    for (long long mx = 1; mx * mx * mx <= N + 2 && mx <= N; ++mx) {
        if (mx > best_max_dim) break;
        for (long long my = mx; mx * my * my <= N + 2 && mx * my <= N; ++my) {
            if (my > best_max_dim) break;
            long long num = mx * my;
            long long mz = (N + num - 1) / num;
            
            long long max_dim = std::max({mx, my, mz});
            if (max_dim < best_max_dim) {
                best_max_dim = max_dim;
                best_mx = mx;
                best_my = my;
                best_mz = mz;
            }
        }
    }
    return {static_cast<int>(best_mx), static_cast<int>(best_my), static_cast<int>(best_mz)};
}

std::pair<double, std::vector<Point>> solve_sc(int n) {
    long long N = n;
    auto [mx, my, mz] = find_dims(N);
    
    double r = 1.0 / (2.0 * std::max({mx, my, mz}));
    
    std::vector<Point> points;
    points.reserve(mx*my*mz);
    for (int i = 0; i < mx; ++i) {
        for (int j = 0; j < my; ++j) {
            for (int k = 0; k < mz; ++k) {
                points.push_back({(2.0 * i + 1.0) / (2.0*mx), 
                                  (2.0 * j + 1.0) / (2.0*my), 
                                  (2.0 * k + 1.0) / (2.0*mz)});
            }
        }
    }
    
    return {r, points};
}

std::pair<double, std::vector<Point>> solve_bcc(int n) {
    long long N = (n + 1) / 2;
    auto [mx, my, mz] = find_dims(N);
    int m_max = std::max({mx, my, mz});
    
    double d_min = SQRT3 / 2.0;
    double delta_max = m_max - 0.5;
    double S = 1.0 / (d_min + delta_max);
    double r = S * d_min / 2.0;
    
    double tx = (1.0 - S * (mx - 0.5)) / 2.0;
    double ty = (1.0 - S * (my - 0.5)) / 2.0;
    double tz = (1.0 - S * (mz - 0.5)) / 2.0;

    std::vector<Point> points;
    points.reserve(2 * mx * my * mz);
    for (int i = 0; i < mx; ++i) {
        for (int j = 0; j < my; ++j) {
            for (int k = 0; k < mz; ++k) {
                points.push_back({S * i + tx, S * j + ty, S * k + tz});
                points.push_back({S * (i + 0.5) + tx, S * (j + 0.5) + ty, S * (k + 0.5) + tz});
            }
        }
    }
    return {r, points};
}

std::pair<double, std::vector<Point>> solve_fcc(int n) {
    long long N = (n + 3) / 4;
    auto [mx, my, mz] = find_dims(N);
    int m_max = std::max({mx, my, mz});

    double d_min = 1.0 / SQRT2;
    double delta_max = m_max - 0.5;
    double S = 1.0 / (d_min + delta_max);
    double r = S * d_min / 2.0;

    double tx = (1.0 - S * (mx - 0.5)) / 2.0;
    double ty = (1.0 - S * (my - 0.5)) / 2.0;
    double tz = (1.0 - S * (mz - 0.5)) / 2.0;

    std::vector<Point> points;
    points.reserve(4 * mx * my * mz);
    for (int i = 0; i < mx; ++i) {
        for (int j = 0; j < my; ++j) {
            for (int k = 0; k < mz; ++k) {
                points.push_back({S * i + tx, S * j + ty, S * k + tz});
                points.push_back({S * (i + 0.5) + tx, S * (j + 0.5) + ty, S * k + tz});
                points.push_back({S * (i + 0.5) + tx, S * j + ty, S * (k + 0.5) + tz});
                points.push_back({S * i + tx, S * (j + 0.5) + ty, S * (k + 0.5) + tz});
            }
        }
    }
    return {r, points};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    auto res_sc = solve_sc(n);
    auto res_bcc = solve_bcc(n);
    auto res_fcc = solve_fcc(n);

    double best_r = 0.0;
    std::vector<Point> best_points;

    if (res_sc.first > best_r) {
        best_r = res_sc.first;
        best_points = res_sc.second;
    }
    if (res_bcc.first > best_r) {
        best_r = res_bcc.first;
        best_points = res_bcc.second;
    }
    if (res_fcc.first > best_r) {
        best_r = res_fcc.first;
        best_points = res_fcc.second;
    }
    
    std::cout << std::fixed << std::setprecision(17);
    for (int i = 0; i < n; ++i) {
        std::cout << best_points[i].x << " " << best_points[i].y << " " << best_points[i].z << "\n";
    }

    return 0;
}