#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

double do_query(int x1, int y1, int x2, int y2) {
    std::cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    double length;
    std::cin >> length;
    return length;
}

void submit_answer(int x, int y, int r) {
    std::cout << "answer " << x << " " << y << " " << r << std::endl;
}

const int MAX_COORD = 100000;
const double NON_ZERO_THRESHOLD = 1e-5;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int low, high;
    
    low = 0; high = MAX_COORD;
    int x_first = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (do_query(mid, 0, mid, MAX_COORD) > NON_ZERO_THRESHOLD) {
            x_first = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    low = 0; high = MAX_COORD;
    int x_last = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (do_query(mid, 0, mid, MAX_COORD) > NON_ZERO_THRESHOLD) {
            x_last = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    low = 0; high = MAX_COORD;
    int y_first = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (do_query(0, mid, MAX_COORD, mid) > NON_ZERO_THRESHOLD) {
            y_first = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    low = 0; high = MAX_COORD;
    int y_last = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (do_query(0, mid, MAX_COORD, mid) > NON_ZERO_THRESHOLD) {
            y_last = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    int x_c = (x_first + x_last) / 2;
    int y_c = (y_first + y_last) / 2;
    int r = (x_last - x_first + 2) / 2;
    
    submit_answer(x_c, y_c, r);

    return 0;
}