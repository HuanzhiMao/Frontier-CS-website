#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

// Helper function to perform a query and get the result.
// It prints the query to standard output and reads the floating point response
// from standard input.
double do_query(long long x1, long long y1, long long x2, long long y2) {
    std::cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    double result;
    std::cin >> result;
    return result;
}

int main() {
    // Untie cin/cout from stdio for potentially faster I/O in competitive programming.
    // std::endl ensures flushing, which is critical for interactive problems.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Find y_min_pos: the smallest integer y-coordinate for which a horizontal line
    // segment spanning the box intersects the disk.
    // For a circle with integer center (cx, cy) and integer radius r, this
    // y-coordinate is cy - r + 1.
    long long y_min_pos;
    {
        long long low = 0, high = 100000;
        while (low < high) {
            long long mid = low + (high - low) / 2;
            // A positive length means the line intersects the disk.
            // We search for the first such line from y=0 upwards.
            if (do_query(0, mid, 100000, mid) > 1e-7) {
                high = mid; // Found an intersection, try smaller y.
            } else {
                low = mid + 1; // No intersection, y must be larger.
            }
        }
        y_min_pos = low;
    }

    // Query at y_min_pos to find the exact chord length.
    double L_y = do_query(0, y_min_pos, 100000, y_min_pos);

    // From the chord length at y = cy - r + 1, we can calculate r.
    // The chord length L is given by L = 2 * sqrt(r^2 - (y - cy)^2).
    // Substituting y = cy - r + 1, we get L_y = 2 * sqrt(2r - 1).
    // Rearranging for r: r = (L_y^2 / 4 + 1) / 2.
    // We round to the nearest integer because r is an integer and L_y has small errors.
    long long r = round((L_y * L_y / 4.0 + 1.0) / 2.0);
    
    // Now that we have r, we can find cy from y_min_pos.
    // y_min_pos = cy - r + 1  =>  cy = y_min_pos + r - 1.
    long long cy = y_min_pos + r - 1;

    // Similarly, find x_min_pos: the smallest integer x-coordinate for which a
    // vertical line segment intersects the disk.
    // This x-coordinate is cx - r + 1.
    long long x_min_pos;
    {
        long long low = 0, high = 100000;
        while (low < high) {
            long long mid = low + (high - low) / 2;
            if (do_query(mid, 0, mid, 100000) > 1e-7) {
                high = mid; // Found an intersection, try smaller x.
            } else {
                low = mid + 1; // No intersection, x must be larger.
            }
        }
        x_min_pos = low;
    }

    // With r and x_min_pos, we can find cx.
    // x_min_pos = cx - r + 1  =>  cx = x_min_pos + r - 1.
    long long cx = x_min_pos + r - 1;

    // We have found cx, cy, and r. Output the answer.
    std::cout << "answer " << cx << " " << cy << " " << r << std::endl;

    return 0;
}