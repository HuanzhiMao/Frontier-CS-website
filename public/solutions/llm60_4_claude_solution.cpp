#include <iostream>
#include <cmath>
#include <iomanip>
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
    // Find center x coordinate
    int centerX = 50000;
    int step = 25000;
    
    while (step >= 1) {
        double left = query(centerX - step, 0, centerX - step, 100000);
        double right = query(centerX + step, 0, centerX + step, 100000);
        
        if (left > right) {
            centerX -= step;
        } else if (right > left) {
            centerX += step;
        }
        step /= 2;
    }
    
    // Find center y coordinate
    int centerY = 50000;
    step = 25000;
    
    while (step >= 1) {
        double down = query(0, centerY - step, 100000, centerY - step);
        double up = query(0, centerY + step, 100000, centerY + step);
        
        if (down > up) {
            centerY -= step;
        } else if (up > down) {
            centerY += step;
        }
        step /= 2;
    }
    
    // Fine-tune center coordinates
    for (int i = 0; i < 3; i++) {
        double curr = query(centerX, 0, centerX, 100000);
        double left = query(centerX - 1, 0, centerX - 1, 100000);
        double right = query(centerX + 1, 0, centerX + 1, 100000);
        
        if (left > curr && left >= right) centerX--;
        else if (right > curr && right > left) centerX++;
        
        curr = query(0, centerY, 100000, centerY);
        double down = query(0, centerY - 1, 100000, centerY - 1);
        double up = query(0, centerY + 1, 100000, centerY + 1);
        
        if (down > curr && down >= up) centerY--;
        else if (up > curr && up > down) centerY++;
    }
    
    // Find radius using binary search
    double diameter = query(centerX, 0, centerX, 100000);
    int radius = (int)(diameter / 2.0 + 0.5);
    
    // Verify and adjust radius
    for (int r = max(100, radius - 2); r <= radius + 2; r++) {
        double expected = 2 * sqrt(r * r - 0.0);
        double actual = query(centerX, 0, centerX, 100000);
        if (abs(actual - 2 * r) < 1.0) {
            radius = r;
            break;
        }
    }
    
    answer(centerX, centerY, radius);
    
    return 0;
}