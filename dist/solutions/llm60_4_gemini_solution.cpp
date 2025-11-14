#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>

// A cache to store query results to avoid re-querying the same line.
std::map<std::pair<int, int>, double> vertical_cache;
std::map<std::pair<int, int>, double> horizontal_cache;

// Function to perform a query, with caching.
double do_query(int x1, int y1, int x2, int y2) {
    if (x1 == x2) { // Vertical line
        if (vertical_cache.count({x1, y1})) {
            return vertical_cache[{x1, y1}];
        }
    } else { // Horizontal line
        if (horizontal_cache.count({x1, y1})) {
            return horizontal_cache[{x1, y1}];
        }
    }

    std::cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    double length;
    std::cin >> length;

    if (x1 == x2) {
        vertical_cache[{x1, y1}] = length;
    } else {
        horizontal_cache[{x1, y1}] = length;
    }
    return length;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int cx, cy, r;

    // Ternary search for cx
    int cx_low = 0, cx_high = 100000;
    while (cx_high - cx_low >= 3) {
        int m1 = cx_low + (cx_high - cx_low) / 3;
        int m2 = cx_high - (cx_high - cx_low) / 3;
        double l1 = do_query(m1, 0, m1, 100000);
        double l2 = do_query(m2, 0, m2, 100000);
        if (l1 < l2) {
            cx_low = m1;
        } else {
            cx_high = m2;
        }
    }

    // Local search for the exact integer cx
    double max_l_x = -1.0;
    int best_x = -1;
    for (int x = cx_low; x <= cx_high; ++x) {
        double l = do_query(x, 0, x, 100000);
        if (l > max_l_x) {
            max_l_x = l;
            best_x = x;
        }
    }
    cx = best_x;

    // Ternary search for cy
    int cy_low = 0, cy_high = 100000;
    while (cy_high - cy_low >= 3) {
        int m1 = cy_low + (cy_high - cy_low) / 3;
        int m2 = cy_high - (cy_high - cy_low) / 3;
        double l1 = do_query(0, m1, 100000, m1);
        double l2 = do_query(0, m2, 100000, m2);
        if (l1 < l2) {
            cy_low = m1;
        } else {
            cy_high = m2;
        }
    }

    // Local search for the exact integer cy
    double max_l_y = -1.0;
    int best_y = -1;
    for (int y = cy_low; y <= cy_high; ++y) {
        double l = do_query(0, y, 100000, y);
        if (l > max_l_y) {
            max_l_y = l;
            best_y = y;
        }
    }
    cy = best_y;

    // Find radius using the diameter length at cx
    double diameter = do_query(cx, 0, cx, 100000);
    r = static_cast<int>(round(diameter / 2.0));

    std::cout << "answer " << cx << " " << cy << " " << r << std::endl;

    return 0;
}