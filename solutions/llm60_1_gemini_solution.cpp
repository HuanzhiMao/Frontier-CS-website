#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

// Function to perform a query and get the result
double do_query(long long x1, long long y1, long long x2, long long y2) {
    std::cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    double length;
    std::cin >> length;
    return length;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout << std::fixed << std::setprecision(10);

    // Ternary search for x-coordinate of the center
    double lx = 0.0, hx = 100000.0;
    for (int i = 0; i < 100; ++i) {
        double m1 = lx + (hx - lx) / 3.0;
        double m2 = hx - (hx - lx) / 3.0;
        double len1 = do_query(round(m1), 0, round(m1), 100000);
        double len2 = do_query(round(m2), 0, round(m2), 100000);
        if (len1 < len2) {
            lx = m1;
        } else {
            hx = m2;
        }
    }
    long long final_x = round((lx + hx) / 2.0);

    // Ternary search for y-coordinate of the center
    double ly = 0.0, hy = 100000.0;
    for (int i = 0; i < 100; ++i) {
        double m1 = ly + (hy - ly) / 3.0;
        double m2 = hy - (hy - ly) / 3.0;
        double len1 = do_query(0, round(m1), 100000, round(m1));
        double len2 = do_query(0, round(m2), 100000, round(m2));
        if (len1 < len2) {
            ly = m1;
        } else {
            hy = m2;
        }
    }
    long long final_y = round((ly + hy) / 2.0);

    // Find the radius by querying through the estimated center
    double diameter = do_query(final_x, 0, final_x, 100000);
    long long final_r = round(diameter / 2.0);
    
    std::cout << "answer " << final_x << " " << final_y << " " << final_r << std::endl;

    return 0;
}