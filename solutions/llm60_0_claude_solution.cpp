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
    // First, find approximate center using diagonal queries
    double d1 = query(0, 0, 100000, 100000);
    double d2 = query(100000, 0, 0, 100000);
    
    // Binary search for x coordinate
    int left = 100, right = 100000 - 100;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        double len = query(mid, 0, mid, 100000);
        if (len > 0) {
            // Binary search within this vertical line
            int bot = 0, top = 100000;
            while (top - bot > 1) {
                int m = (bot + top) / 2;
                double test = query(mid, m, mid, 100000);
                if (test > 0) {
                    bot = m;
                } else {
                    top = m;
                }
            }
            right = mid;
        } else {
            left = mid;
        }
    }
    
    int cx = right;
    
    // Find more precise x by checking intersection length
    for (int x = max(100, cx - 10); x <= min(99900, cx + 10); x++) {
        double len = query(x, 0, x, 100000);
        if (len > 0) {
            cx = x;
            break;
        }
    }
    
    // Binary search for y coordinate using horizontal line through found x
    int bottom = 100, top = 100000 - 100;
    while (top - bottom > 1) {
        int mid = (bottom + top) / 2;
        double len = query(0, mid, 100000, mid);
        if (len > 0) {
            // Binary search within this horizontal line
            int lft = 0, rgt = 100000;
            while (rgt - lft > 1) {
                int m = (lft + rgt) / 2;
                double test = query(m, mid, 100000, mid);
                if (test > 0) {
                    rgt = m;
                } else {
                    lft = m;
                }
            }
            top = mid;
        } else {
            bottom = mid;
        }
    }
    
    int cy = top;
    
    // Find more precise y
    for (int y = max(100, cy - 10); y <= min(99900, cy + 10); y++) {
        double len = query(0, y, 100000, y);
        if (len > 0) {
            cy = y;
            break;
        }
    }
    
    // Refine center by checking multiple lines
    double maxLen = 0;
    int bestX = cx, bestY = cy;
    
    for (int dx = -5; dx <= 5; dx++) {
        for (int dy = -5; dy <= 5; dy++) {
            int testX = cx + dx;
            int testY = cy + dy;
            if (testX >= 100 && testX <= 99900 && testY >= 100 && testY <= 99900) {
                double len = query(testX, 0, testX, 100000);
                if (len > maxLen) {
                    maxLen = len;
                    bestX = testX;
                    bestY = testY;
                }
            }
        }
    }
    
    cx = bestX;
    cy = bestY;
    
    // Find radius by measuring chord length
    double chordLen = query(cx, 0, cx, 100000);
    int radius = (int)(chordLen / 2.0 + 0.5);
    
    // Verify and adjust radius
    for (int r = max(100, radius - 5); r <= min(50000, radius + 5); r++) {
        // Check if this radius makes sense
        double expectedChord = 2.0 * r;
        if (abs(expectedChord - chordLen) < abs(2.0 * radius - chordLen)) {
            radius = r;
        }
    }
    
    answer(cx, cy, radius);
    
    return 0;
}