#include <bits/stdc++.h>

using namespace std;

int main() {
    int n;
    cin >> n;
    int k = 0;
    while (k * k < n) k++;
    double L = k;
    printf("%.6f\n", L);
    int placed = 0;
    for (int row = 0; placed < n; ++row) {
        for (int col = 0; col < k && placed < n; ++col) {
            double x = 0.5 + col;
            double y = 0.5 + row;
            double a = 0.0;
            printf("%.6f %.6f %.6f\n", x, y, a);
            placed++;
        }
    }
    return 0;
}