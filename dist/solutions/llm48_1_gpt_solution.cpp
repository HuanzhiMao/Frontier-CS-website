#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;
    if (n <= 0) return 0;

    // Constants for hexagonal close packing (HCP) with nearest-neighbor distance = 1 in param space
    const double sqrt3 = std::sqrt(3.0);
    const double r2d = sqrt3 / 2.0;       // row spacing in y for triangular lattice
    const double h = std::sqrt(2.0 / 3.0); // layer spacing in z

    auto computeE = [&](int Nx, int Ny, int Nz) -> double {
        double ex = max(0, Nx - 1) + (Ny > 1 ? 0.5 : 0.0) + (Nz > 1 ? 0.5 : 0.0);
        double ey = max(0, Ny - 1) * r2d + (Nz > 1 ? (r2d / 3.0) : 0.0);
        double ez = max(0, Nz - 1) * h;
        return max(ex, max(ey, ez));
    };

    // Greedily grow dimensions to reach at least n points while minimizing the maximal extent E
    int Nx = 1, Ny = 1, Nz = 1;
    long long prod = 1;
    while (prod < n) {
        double Ex = computeE(Nx + 1, Ny, Nz);
        double Ey = computeE(Nx, Ny + 1, Nz);
        double Ez = computeE(Nx, Ny, Nz + 1);

        // Choose the increment that yields the smallest E
        if (Ex <= Ey && Ex <= Ez) {
            Nx++;
        } else if (Ey <= Ex && Ey <= Ez) {
            Ny++;
        } else {
            Nz++;
        }
        prod = 1LL * Nx * Ny * Nz;
    }

    // Final extents and scaling
    double E = computeE(Nx, Ny, Nz);
    if (E <= 0) E = 1.0; // safety, though shouldn't happen for n>=2

    // Minimal pairwise distance in normalized pre-margin coordinates is 1/E
    double dmin_norm = 1.0 / E;

    // Choose margin t slightly below the balancing point to avoid equality due to rounding
    double t = dmin_norm / (2.0 * (1.0 + dmin_norm));
    t = max(1e-12, min(0.4995, t - 1e-12));
    double sc = (1.0 - 2.0 * t) / E;

    cout.setf(std::ios::fixed);
    cout << setprecision(12);

    long long printed = 0;
    for (int kz = 0; kz < Nz && printed < n; ++kz) {
        double offX_layer = (kz & 1) ? 0.5 : 0.0;
        double offY_layer = (kz & 1) ? (r2d / 3.0) : 0.0;
        double zParam = kz * h;
        for (int jy = 0; jy < Ny && printed < n; ++jy) {
            double rowOff = (jy & 1) ? 0.5 : 0.0;
            double yParam = jy * r2d + offY_layer;
            for (int ix = 0; ix < Nx && printed < n; ++ix) {
                double xParam = ix + rowOff + offX_layer;

                double x = t + sc * xParam;
                double y = t + sc * yParam;
                double z = t + sc * zParam;

                // Clamp to [0,1] to be safe with floating error
                if (x < 0) x = 0; else if (x > 1) x = 1;
                if (y < 0) y = 0; else if (y > 1) y = 1;
                if (z < 0) z = 0; else if (z > 1) z = 1;

                cout << x << " " << y << " " << z << "\n";
                ++printed;
            }
        }
    }

    return 0;
}