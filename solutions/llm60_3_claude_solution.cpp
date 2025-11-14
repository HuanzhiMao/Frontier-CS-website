#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double query(int x1, int y1, int x2, int y2) {
    cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    cout.flush();
    double result;
    cin >> result;
    return result;
}

void answer(int x, int y, int r) {
    cout << "answer " << x << " " << y << " " << r << endl;
    cout.flush();
}

int main() {
    // First, find approximate center using binary search
    
    // Find x coordinate of center
    int left = 0, right = 100000;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        double len = query(mid, 0, mid, 100000);
        if (len > 0) {
            double len_left = query(mid - 1, 0, mid - 1, 100000);
            double len_right = query(mid + 1, 0, mid + 1, 100000);
            if (len >= len_left && len >= len_right) {
                break;
            } else if (len_left > len_right) {
                right = mid;
            } else {
                left = mid;
            }
        } else {
            // Try both sides
            double len_left = query(left + (mid - left) / 2, 0, left + (mid - left) / 2, 100000);
            if (len_left > 0) {
                right = mid;
            } else {
                left = mid;
            }
        }
    }
    
    // Refine x coordinate
    int cx = (left + right) / 2;
    double max_len = 0;
    for (int x = cx - 5; x <= cx + 5; x++) {
        if (x >= 0 && x <= 100000) {
            double len = query(x, 0, x, 100000);
            if (len > max_len) {
                max_len = len;
                cx = x;
            }
        }
    }
    
    // Find y coordinate of center
    left = 0; right = 100000;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        double len = query(0, mid, 100000, mid);
        if (len > 0) {
            double len_left = query(0, mid - 1, 100000, mid - 1);
            double len_right = query(0, mid + 1, 100000, mid + 1);
            if (len >= len_left && len >= len_right) {
                break;
            } else if (len_left > len_right) {
                right = mid;
            } else {
                left = mid;
            }
        } else {
            double len_left = query(0, left + (mid - left) / 2, 100000, left + (mid - left) / 2);
            if (len_left > 0) {
                right = mid;
            } else {
                left = mid;
            }
        }
    }
    
    // Refine y coordinate
    int cy = (left + right) / 2;
    max_len = 0;
    for (int y = cy - 5; y <= cy + 5; y++) {
        if (y >= 0 && y <= 100000) {
            double len = query(0, y, 100000, y);
            if (len > max_len) {
                max_len = len;
                cy = y;
            }
        }
    }
    
    // Find radius using the vertical line through center
    double diameter = query(cx, 0, cx, 100000);
    int r = (int)(diameter / 2.0 + 0.5);
    
    // Verify and adjust radius if needed
    for (int test_r = r - 2; test_r <= r + 2; test_r++) {
        if (test_r >= 100) {
            // Check if this radius makes sense
            double expected = 2.0 * test_r;
            if (abs(diameter - expected) < abs(diameter - 2.0 * r)) {
                r = test_r;
            }
        }
    }
    
    answer(cx, cy, r);
    return 0;
}