#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double query(int x1, int y1, int x2, int y2) {
    cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    cout.flush();
    double response;
    cin >> response;
    return response;
}

void answer(int x, int y, int r) {
    cout << "answer " << x << " " << y << " " << r << endl;
    cout.flush();
}

int main() {
    // First, find approximate center using vertical and horizontal lines
    double len1 = query(50000, 0, 50000, 100000);
    double len2 = query(0, 50000, 100000, 50000);
    
    int approx_cx = 50000;
    int approx_cy = 50000;
    
    if (len1 > 0) {
        // Binary search for top and bottom of circle on vertical line x=50000
        int top = 100000, bottom = 0;
        
        // Find top boundary
        int lo = 50000, hi = 100000;
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            double len = query(50000, mid, 50000, mid + 1);
            if (len > 0) lo = mid;
            else hi = mid;
        }
        top = lo;
        
        // Find bottom boundary  
        lo = 0; hi = 50000;
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            double len = query(50000, mid, 50000, mid + 1);
            if (len > 0) hi = mid;
            else lo = mid;
        }
        bottom = hi;
        
        approx_cy = (top + bottom) / 2;
    }
    
    if (len2 > 0) {
        // Binary search for left and right of circle on horizontal line y=approx_cy
        int left = 0, right = 100000;
        
        // Find right boundary
        int lo = 50000, hi = 100000;
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            double len = query(mid, approx_cy, mid + 1, approx_cy);
            if (len > 0) lo = mid;
            else hi = mid;
        }
        right = lo;
        
        // Find left boundary
        lo = 0; hi = 50000;
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            double len = query(mid, approx_cy, mid + 1, approx_cy);
            if (len > 0) hi = mid;
            else lo = mid;
        }
        left = hi;
        
        approx_cx = (left + right) / 2;
    }
    
    // Refine center using the new approximate position
    double len_v = query(approx_cx, 0, approx_cx, 100000);
    double len_h = query(0, approx_cy, 100000, approx_cy);
    
    // The radius should be half of the chord length for lines through center
    int radius = (int)(len_v / 2 + 0.5);
    
    // Final refinement of center
    int cx = approx_cx, cy = approx_cy;
    
    // Try small adjustments around the approximate center
    for (int dx = -5; dx <= 5; dx++) {
        for (int dy = -5; dy <= 5; dy++) {
            int test_x = approx_cx + dx;
            int test_y = approx_cy + dy;
            if (test_x < 0 || test_x > 100000 || test_y < 0 || test_y > 100000) continue;
            
            double test_len = query(test_x, 0, test_x, 100000);
            if (abs(test_len - 2 * radius) < 0.5) {
                double test_len2 = query(0, test_y, 100000, test_y);
                if (abs(test_len2 - 2 * radius) < 0.5) {
                    cx = test_x;
                    cy = test_y;
                    goto found;
                }
            }
        }
    }
    
    found:
    answer(cx, cy, radius);
    
    return 0;
}