#include <bits/stdc++.h>

using namespace std;

int main() {
    long long n;
    cin >> n;
    long long m = 1;
    while (m * m * m < n) m++;
    double rr = 1.0 / (2 * m);
    double dd = 1.0 / m;
    vector<array<double, 3>> cens;
    for (long long i = 0; i < m; i++) {
        double x = rr + i * dd;
        for (long long j = 0; j < m; j++) {
            double y = rr + j * dd;
            for (long long k = 0; k < m; k++) {
                double z = rr + k * dd;
                cens.push_back({x, y, z});
            }
        }
    }
    for (long long i = 0; i < n; i++) {
        cout << fixed << setprecision(10) << cens[i][0] << " " << cens[i][1] << " " << cens[i][2] << "\n";
    }
    return 0;
}