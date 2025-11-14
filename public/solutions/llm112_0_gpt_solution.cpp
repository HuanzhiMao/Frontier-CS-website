#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<double> x(n), y(n), z(n);
    
    auto normalize = [&](double &a, double &b, double &c) {
        double r = sqrt(a*a + b*b + c*c);
        if (r == 0.0) { a = 1.0; b = 0.0; c = 0.0; return; }
        a /= r; b /= r; c /= r;
    };
    
    if (n == 2) {
        x[0] = 0.0; y[0] = 0.0; z[0] = 1.0;
        x[1] = 0.0; y[1] = 0.0; z[1] = -1.0;
    } else {
        // Initial placement: spherical Fibonacci lattice
        const double ga = M_PI * (3.0 - sqrt(5.0)); // golden angle
        for (int i = 0; i < n; ++i) {
            double zc = 1.0 - 2.0 * ( (i + 0.5) / (double)n );
            double r = sqrt(max(0.0, 1.0 - zc*zc));
            double theta = ga * i;
            x[i] = r * cos(theta);
            y[i] = r * sin(theta);
            z[i] = zc;
            // already on unit sphere
        }
        // Refinement: simple Coulomb repulsion constrained to the sphere
        int iter;
        if (n <= 10) iter = 600;
        else if (n <= 50) iter = 400;
        else if (n <= 200) iter = 250;
        else if (n <= 500) iter = 150;
        else iter = 100;
        
        double s0, s1;
        if (n <= 50) { s0 = 0.20; s1 = 0.02; }
        else if (n <= 200) { s0 = 0.12; s1 = 0.015; }
        else { s0 = 0.10; s1 = 0.010; }
        
        vector<double> fx(n), fy(n), fz(n);
        const double eps = 1e-12;
        
        for (int it = 0; it < iter; ++it) {
            // zero forces
            fill(fx.begin(), fx.end(), 0.0);
            fill(fy.begin(), fy.end(), 0.0);
            fill(fz.begin(), fz.end(), 0.0);
            
            // pairwise Coulomb-like repulsion
            for (int i = 0; i < n; ++i) {
                double xi = x[i], yi = y[i], zi = z[i];
                for (int j = i + 1; j < n; ++j) {
                    double dx = xi - x[j];
                    double dy = yi - y[j];
                    double dz = zi - z[j];
                    double r2 = dx*dx + dy*dy + dz*dz + eps;
                    double inv_r = 1.0 / sqrt(r2);
                    double inv_r3 = inv_r / r2; // 1 / r^3
                    double fx_ = dx * inv_r3;
                    double fy_ = dy * inv_r3;
                    double fz_ = dz * inv_r3;
                    fx[i] += fx_;
                    fy[i] += fy_;
                    fz[i] += fz_;
                    fx[j] -= fx_;
                    fy[j] -= fy_;
                    fz[j] -= fz_;
                }
            }
            // project forces onto tangent plane and find max magnitude
            double maxLen = 0.0;
            for (int i = 0; i < n; ++i) {
                double dot = fx[i]*x[i] + fy[i]*y[i] + fz[i]*z[i];
                fx[i] -= dot * x[i];
                fy[i] -= dot * y[i];
                fz[i] -= dot * z[i];
                double L = sqrt(fx[i]*fx[i] + fy[i]*fy[i] + fz[i]*fz[i]);
                if (L > maxLen) maxLen = L;
            }
            if (maxLen < 1e-18) break;
            double t = (iter <= 1) ? 1.0 : (double)it / (double)(iter - 1);
            double stepScale = s0 + (s1 - s0) * t; // linear cooling
            double s = stepScale / maxLen;
            for (int i = 0; i < n; ++i) {
                x[i] += fx[i] * s;
                y[i] += fy[i] * s;
                z[i] += fz[i] * s;
                normalize(x[i], y[i], z[i]);
            }
        }
    }
    
    // Compute minimum pairwise distance
    double minD2 = 1e100;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 < minD2) minD2 = d2;
        }
    }
    double minDist = sqrt(minD2);
    
    cout.setf(std::ios::fixed); 
    cout << setprecision(12) << minDist << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setprecision(12) << x[i] << " " << y[i] << " " << z[i] << "\n";
    }
    return 0;
}