#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    const double PI = acos(-1.0);
    const double golden_angle = PI * (3.0 - sqrt(5.0));
    
    vector<array<double,3>> pts;
    pts.reserve(n);
    
    auto add = [&](double x, double y, double z){
        pts.push_back({x,y,z});
    };
    
    if (n == 2) {
        add(0.0, 0.0, 1.0);
        add(0.0, 0.0, -1.0);
    } else if (n == 3) {
        for (int i = 0; i < 3; ++i) {
            double ang = 2.0 * PI * i / 3.0;
            add(cos(ang), sin(ang), 0.0);
        }
    } else if (n == 4) {
        double s = 1.0 / sqrt(3.0);
        add( s,  s,  s);
        add( s, -s, -s);
        add(-s,  s, -s);
        add(-s, -s,  s);
    } else if (n == 5) {
        add(0.0, 0.0, 1.0);
        add(0.0, 0.0, -1.0);
        for (int i = 0; i < 3; ++i) {
            double ang = 2.0 * PI * i / 3.0;
            add(cos(ang), sin(ang), 0.0);
        }
    } else if (n == 6) {
        add(1.0, 0.0, 0.0);
        add(-1.0, 0.0, 0.0);
        add(0.0, 1.0, 0.0);
        add(0.0, -1.0, 0.0);
        add(0.0, 0.0, 1.0);
        add(0.0, 0.0, -1.0);
    } else {
        for (int i = 0; i < n; ++i) {
            double a = (i + 0.5) / n;
            double z = 1.0 - 2.0 * a;
            double r = sqrt(max(0.0, 1.0 - z * z));
            double phi = golden_angle * i;
            double x = cos(phi) * r;
            double y = sin(phi) * r;
            add(x, y, z);
        }
    }
    
    // Compute minimum pairwise distance
    double minD2 = numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        const auto &a = pts[i];
        for (int j = i + 1; j < n; ++j) {
            const auto &b = pts[j];
            double dx = a[0] - b[0];
            double dy = a[1] - b[1];
            double dz = a[2] - b[2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 < minD2) minD2 = d2;
        }
    }
    double minD = sqrt(minD2);
    
    cout.setf(std::ios::fixed);
    cout << setprecision(15) << minD << "\n";
    for (auto &p : pts) {
        cout << setprecision(15) << p[0] << " " << p[1] << " " << p[2] << "\n";
    }
    return 0;
}