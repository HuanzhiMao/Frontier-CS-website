#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    long long nn = n;
    int k = 0;
    while ((long long) k * k < nn) k++;
    double L = k;
    cout << fixed << setprecision(6) << L << endl;
    int placed = 0;
    int row = 0;
    while (placed < n) {
        int in_row = min(k, n - placed);
        for (int col = 0; col < in_row; col++) {
            double x = 0.5 + col;
            double y = 0.5 + row;
            double a = 0.0;
            cout << fixed << setprecision(6) << x << " " << y << " " << a << endl;
            placed++;
        }
        row++;
    }
    return 0;
}