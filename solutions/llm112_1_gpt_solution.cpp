#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    const double PI = acos(-1.0);
    const double GOLDEN_ANGLE = PI * (3.0 - sqrt(5.0));
    vector<double> x(n), y(n), z(n);
    // Initialize with Fibonacci sphere
    for (int i = 0; i < n; ++i) {
        double zi = 1.0 - 2.0 * (i + 0.5) / n;
        double r = sqrt(max(0.0, 1.0 - zi * zi));
        double phi = GOLDEN_ANGLE * i;
        x[i] = r * cos(phi);
        y[i] = r * sin(phi);
        z[i] = zi;
    }

    // Small random jitter to break symmetries
    mt19937_64 rng(71236721);
    uniform_real_distribution<double> ur(-1e-6, 1e-6);
    for (int i = 0; i < n; ++i) {
        x[i] += ur(rng);
        y[i] += ur(rng);
        z[i] += ur(rng);
        double inv = 1.0 / sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
        x[i] *= inv; y[i] *= inv; z[i] *= inv;
    }

    // Force-directed refinement on the sphere (time-bounded)
    vector<double> fx(n), fy(n), fz(n);
    auto start = chrono::steady_clock::now();
    const double time_limit_sec = 1.35; // Leave time for final computations and printing
    double step = (n <= 50 ? 0.05 : n <= 200 ? 0.03 : n <= 400 ? 0.02 : n <= 700 ? 0.015 : n <= 1000 ? 0.012 : 0.01);
    int iter = 0;
    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - start).count();
        if (elapsed > time_limit_sec) break;

        // Zero forces
        fill(fx.begin(), fx.end(), 0.0);
        fill(fy.begin(), fy.end(), 0.0);
        fill(fz.begin(), fz.end(), 0.0);

        // Pairwise repulsion
        for (int i = 0; i < n; ++i) {
            double xi = x[i], yi = y[i], zi = z[i];
            for (int j = i + 1; j < n; ++j) {
                double dx = xi - x[j];
                double dy = yi - y[j];
                double dz = zi - z[j];
                double d2 = dx*dx + dy*dy + dz*dz + 1e-12; // regularization to avoid div by zero
                double inv = 1.0 / d2; // 1/r^2 repulsion
                double fxij = dx * inv;
                double fyij = dy * inv;
                double fzij = dz * inv;
                fx[i] += fxij; fy[i] += fyij; fz[i] += fzij;
                fx[j] -= fxij; fy[j] -= fyij; fz[j] -= fzij;
            }
        }

        // Project to tangent plane and move, then renormalize onto the sphere
        for (int i = 0; i < n; ++i) {
            double dotp = fx[i]*x[i] + fy[i]*y[i] + fz[i]*z[i];
            fx[i] -= dotp * x[i];
            fy[i] -= dotp * y[i];
            fz[i] -= dotp * z[i];
            double fn = sqrt(fx[i]*fx[i] + fy[i]*fy[i] + fz[i]*fz[i]);
            if (fn > 1e-18) {
                double lam = step / fn;
                x[i] += fx[i] * lam;
                y[i] += fy[i] * lam;
                z[i] += fz[i] * lam;
            }
            double inv = 1.0 / sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
            x[i] *= inv; y[i] *= inv; z[i] *= inv;
        }

        // Slowly reduce step
        step *= 0.998;
        ++iter;
    }

    // Compute true minimum pairwise distance
    double minDist = 1e100;
    for (int i = 0; i < n; ++i) {
        double xi = x[i], yi = y[i], zi = z[i];
        for (int j = i + 1; j < n; ++j) {
            double dx = xi - x[j];
            double dy = yi - y[j];
            double dz = zi - z[j];
            double d = sqrt(dx*dx + dy*dy + dz*dz);
            if (d < minDist) minDist = d;
        }
    }

    cout.setf(std::ios::fixed); 
    cout << setprecision(15) << minDist << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setprecision(15) << x[i] << " " << y[i] << " " << z[i] << "\n";
    }
    return 0;
}