#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <map>

// Global constants
const int MAX_COORD = 100000;

// Caching to avoid re-querying the same coordinate
std::map<int, double> cache_vertical;
std::map<int, double> cache_horizontal;

// Query helper
double do_query(int x1, int y1, int x2, int y2) {
    std::cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    double response;
    std::cin >> response;
    return response;
}

// Get vertical chord length at x
double L_vertical(int x) {
    if (x < 0 || x > MAX_COORD) return 0;
    if (cache_vertical.count(x)) {
        return cache_vertical[x];
    }
    double len = do_query(x, 0, x, MAX_COORD);
    cache_vertical[x] = len;
    return len;
}

// Get horizontal chord length at y
double L_horizontal(int y) {
    if (y < 0 || y > MAX_COORD) return 0;
    if (cache_horizontal.count(y)) {
        return cache_horizontal[y];
    }
    double len = do_query(0, y, MAX_COORD, y);
    cache_horizontal[y] = len;
    return len;
}

int find_hit_x() {
    double lx_ts = 0, rx_ts = MAX_COORD;
    for (int i = 0; i < 25; ++i) {
        double m1 = lx_ts + (rx_ts - lx_ts) / 3;
        double m2 = rx_ts - (rx_ts - lx_ts) / 3;
        if (L_vertical(round(m1)) < L_vertical(round(m2))) {
            lx_ts = m1;
        } else {
            rx_ts = m2;
        }
    }
    return round((lx_ts + rx_ts) / 2);
}

int find_hit_y() {
    double ly_ts = 0, ry_ts = MAX_COORD;
    for (int i = 0; i < 25; ++i) {
        double m1 = ly_ts + (ry_ts - ly_ts) / 3;
        double m2 = ry_ts - (ry_ts - ly_ts) / 3;
        if (L_horizontal(round(m1)) < L_horizontal(round(m2))) {
            ly_ts = m1;
        } else {
            ry_ts = m2;
        }
    }
    return round((ly_ts + ry_ts) / 2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Solve for X dimension
    double final_lx_b, final_rx_b;
    int x_hit = -1;

    if (L_vertical(0) > 1e-9) {
        final_lx_b = 0;
    } else {
        x_hit = find_hit_x();
        double lo = 0, hi = x_hit;
        for (int i = 0; i < 60; ++i) {
            double mid = lo + (hi - lo) / 2;
            if (L_vertical(round(mid)) > 1e-9) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
        final_lx_b = (lo + hi) / 2;
    }
    
    if (L_vertical(MAX_COORD) > 1e-9) {
        final_rx_b = MAX_COORD;
    } else {
        if (x_hit == -1) {
            x_hit = find_hit_x();
        }
        double lo = x_hit, hi = MAX_COORD;
        for (int i = 0; i < 60; ++i) {
            double mid = lo + (hi - lo) / 2;
            if (L_vertical(round(mid)) > 1e-9) {
                lo = mid;
            } else {
                hi = mid;
            }
        }
        final_rx_b = (lo + hi) / 2;
    }

    int cx = round((final_lx_b + final_rx_b) / 2);
    double r_from_x = (final_rx_b - final_lx_b) / 2;

    // Solve for Y dimension
    double final_ly_b, final_ry_b;
    int y_hit = -1;

    if (L_horizontal(0) > 1e-9) {
        final_ly_b = 0;
    } else {
        y_hit = find_hit_y();
        double lo = 0, hi = y_hit;
        for (int i = 0; i < 60; ++i) {
            double mid = lo + (hi - lo) / 2;
            if (L_horizontal(round(mid)) > 1e-9) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
        final_ly_b = (lo + hi) / 2;
    }

    if (L_horizontal(MAX_COORD) > 1e-9) {
        final_ry_b = MAX_COORD;
    } else {
        if (y_hit == -1) {
            y_hit = find_hit_y();
        }
        double lo = y_hit, hi = MAX_COORD;
        for (int i = 0; i < 60; ++i) {
            double mid = lo + (hi - lo) / 2;
            if (L_horizontal(round(mid)) > 1e-9) {
                lo = mid;
            } else {
                hi = mid;
            }
        }
        final_ry_b = (lo + hi) / 2;
    }
    
    int cy = round((final_ly_b + final_ry_b) / 2);
    double r_from_y = (final_ry_b - final_ly_b) / 2;

    int r = round((r_from_x + r_from_y) / 2);

    std::cout << "answer " << cx << " " << cy << " " << r << std::endl;

    return 0;
}