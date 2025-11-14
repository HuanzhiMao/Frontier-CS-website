#include <bits/stdc++.h>
using namespace std;

static vector<int> center_out_order(int N) {
    vector<int> order;
    order.reserve(N);
    int left = (N - 1) / 2;
    int right = left + 1;
    while (left >= 0 || right < N) {
        if (left >= 0) order.push_back(left--);
        if (right < N) order.push_back(right++);
    }
    return order;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;

    // Number of FCC cells needed (4 points per cell)
    long long cells_needed = (n + 3) / 4;

    // Determine balanced Nx, Ny, Nz minimizing the maximum dimension
    long long base = pow((long double)cells_needed, 1.0L / 3.0L);
    if (base < 1) base = 1;
    long long Nx = base, Ny = base, Nz = base;
    auto prod = [&]() -> long long { return Nx * Ny * Nz; };
    while (prod() < cells_needed) {
        // Increment the currently smallest dimension to balance
        if (Nx <= Ny && Nx <= Nz) ++Nx;
        else if (Ny <= Nx && Ny <= Nz) ++Ny;
        else ++Nz;
    }

    // FCC lattice scaling
    const double sqrt2 = sqrt(2.0);
    double M = max({Nx - 0.5, Ny - 0.5, Nz - 0.5});
    double s = 1.0 / (M + 1.0 / sqrt2);           // lattice constant
    double rx = 0.5 * (1.0 - s * (Nx - 0.5));     // boundary margin per axis
    double ry = 0.5 * (1.0 - s * (Ny - 0.5));
    double rz = 0.5 * (1.0 - s * (Nz - 0.5));

    // Translation to center within the cube
    double tx = rx;
    double ty = ry;
    double tz = rz;

    // Center-outward ordering for indices
    vector<int> orderX = center_out_order((int)Nx);
    vector<int> orderY = center_out_order((int)Ny);
    vector<int> orderZ = center_out_order((int)Nz);

    cout.setf(std::ios::fmtflags(0), std::ios::floatfield);
    cout << setprecision(17);

    long long printed = 0;

    // Basis points for FCC: prefer interior-like points first
    // b2: (0,1/2,1/2), b3: (1/2,0,1/2), b4: (1/2,1/2,0), b1: (0,0,0)
    const double bx[4] = {0.0, 0.5, 0.5, 0.0};
    const double by[4] = {0.5, 0.0, 0.5, 0.0};
    const double bz[4] = {0.5, 0.5, 0.0, 0.0};
    int basis_order[4] = {0, 1, 2, 3};

    for (int kk : orderZ) {
        for (int jj : orderY) {
            for (int ii : orderX) {
                for (int bi = 0; bi < 4; ++bi) {
                    int b = basis_order[bi];
                    double x = tx + s * (ii + bx[b]);
                    double y = ty + s * (jj + by[b]);
                    double z = tz + s * (kk + bz[b]);

                    // Clamp very tightly to [0,1] to avoid minor rounding issues
                    if (x < 0) x = 0; if (x > 1) x = 1;
                    if (y < 0) y = 0; if (y > 1) y = 1;
                    if (z < 0) z = 0; if (z > 1) z = 1;

                    cout << x << " " << y << " " << z << "\n";
                    if (++printed >= n) return 0;
                }
            }
        }
    }

    return 0;
}