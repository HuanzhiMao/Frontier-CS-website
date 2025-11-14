#include <bits/stdc++.h>
using namespace std;

struct Vec {
    double x, y, z;
    Vec() : x(0), y(0), z(0) {}
    Vec(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y, z + o.z); }
    Vec operator-(const Vec& o) const { return Vec(x - o.x, y - o.y, z - o.z); }
    Vec operator*(double k) const { return Vec(x * k, y * k, z * k); }
    Vec& operator+=(const Vec& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec& operator-=(const Vec& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
};

static inline double dot(const Vec& a, const Vec& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline double norm2(const Vec& a) { return dot(a,a); }
static inline double norm(const Vec& a) { return sqrt(norm2(a)); }
static inline Vec normalize(const Vec& a) {
    double l = norm(a);
    if (l == 0) return a;
    return a * (1.0 / l);
}

vector<Vec> fibonacci_sphere(int n) {
    const double phi = (3.0 - sqrt(5.0)) * M_PI; // golden angle in radians
    vector<Vec> pts;
    pts.reserve(n);
    for (int i = 0; i < n; ++i) {
        double y = 1.0 - 2.0 * ( (i + 0.5) / n );
        double r = sqrt(max(0.0, 1.0 - y*y));
        double theta = phi * i;
        double x = cos(theta) * r;
        double z = sin(theta) * r;
        pts.emplace_back(x, y, z);
    }
    return pts;
}

bool special_points(int n, vector<Vec>& pts) {
    pts.clear();
    const double INV_SQRT3 = 1.0 / sqrt(3.0);
    if (n == 2) {
        pts.emplace_back(0, 0, 1);
        pts.emplace_back(0, 0, -1);
        return true;
    }
    if (n == 3) {
        double c = cos(2.0 * M_PI / 3.0);
        double s = sin(2.0 * M_PI / 3.0);
        pts.emplace_back(1, 0, 0);
        pts.emplace_back(c, s, 0);
        pts.emplace_back(c, -s, 0);
        return true;
    }
    if (n == 4) {
        pts.emplace_back( 1*INV_SQRT3,  1*INV_SQRT3,  1*INV_SQRT3);
        pts.emplace_back( 1*INV_SQRT3, -1*INV_SQRT3, -1*INV_SQRT3);
        pts.emplace_back(-1*INV_SQRT3,  1*INV_SQRT3, -1*INV_SQRT3);
        pts.emplace_back(-1*INV_SQRT3, -1*INV_SQRT3,  1*INV_SQRT3);
        return true;
    }
    if (n == 6) {
        pts.emplace_back(1,0,0); pts.emplace_back(-1,0,0);
        pts.emplace_back(0,1,0); pts.emplace_back(0,-1,0);
        pts.emplace_back(0,0,1); pts.emplace_back(0,0,-1);
        return true;
    }
    if (n == 8) {
        // Cube vertices normalized
        for (int sx : {-1,1})
            for (int sy : {-1,1})
                for (int sz : {-1,1})
                    pts.emplace_back(sx*INV_SQRT3, sy*INV_SQRT3, sz*INV_SQRT3);
        return true;
    }
    if (n == 12) {
        // Icosahedron vertices
        double phi = (1.0 + sqrt(5.0)) * 0.5;
        double a = 1.0, b = phi;
        double inv = 1.0 / sqrt(a*a + b*b);
        // (0, ±1, ±phi)
        for (int sy : {-1,1}) for (int sz : {-1,1})
            pts.emplace_back(0, sy*a*inv, sz*b*inv);
        // (±1, ±phi, 0)
        for (int sx : {-1,1}) for (int sy : {-1,1})
            pts.emplace_back(sx*a*inv, sy*b*inv, 0);
        // (±phi, 0, ±1)
        for (int sx : {-1,1}) for (int sz : {-1,1})
            pts.emplace_back(sx*b*inv, 0, sz*a*inv);
        return true;
    }
    return false;
}

void relax_points(vector<Vec>& p) {
    int n = (int)p.size();
    if (n <= 12) return; // skip relaxation for small special cases
    long long pairOpsPerIter = 1LL * n * (n - 1) / 2;
    if (pairOpsPerIter <= 0) return;
    const double pairBudget = 1.2e7; // total pair computations budget
    int iters = (int)min(80LL, max(0LL, (long long)(pairBudget / max(1LL, pairOpsPerIter))));
    if (iters <= 0) return;

    vector<Vec> F(n);
    double step = 0.05; // initial target step (tangential)
    for (int it = 0; it < iters; ++it) {
        // zero forces
        for (int i = 0; i < n; ++i) F[i] = Vec(0,0,0);

        // pairwise repulsion
        for (int i = 0; i < n; ++i) {
            const Vec& pi = p[i];
            for (int j = i+1; j < n; ++j) {
                Vec d = Vec(pi.x - p[j].x, pi.y - p[j].y, pi.z - p[j].z);
                double d2 = d.x*d.x + d.y*d.y + d.z*d.z + 1e-12;
                double inv = 1.0 / d2; // inverse-square law
                Vec ff = d * inv;
                F[i] += ff;
                F[j] -= ff;
            }
        }

        // project forces to tangent plane and compute RMS
        double sumSq = 0.0;
        for (int i = 0; i < n; ++i) {
            // remove radial component
            double proj = F[i].x * p[i].x + F[i].y * p[i].y + F[i].z * p[i].z;
            F[i].x -= proj * p[i].x;
            F[i].y -= proj * p[i].y;
            F[i].z -= proj * p[i].z;
            sumSq += F[i].x*F[i].x + F[i].y*F[i].y + F[i].z*F[i].z;
        }
        double rms = sqrt(sumSq / n) + 1e-18;
        double scale = step / rms;

        // update points and renormalize
        for (int i = 0; i < n; ++i) {
            p[i].x += scale * F[i].x;
            p[i].y += scale * F[i].y;
            p[i].z += scale * F[i].z;
            double l = sqrt(p[i].x*p[i].x + p[i].y*p[i].y + p[i].z*p[i].z);
            if (l > 0) {
                p[i].x /= l; p[i].y /= l; p[i].z /= l;
            } else {
                // fallback to previous or a random direction; here normalize won't change much
                // reset to unit vector
                p[i] = normalize(p[i]);
            }
        }
        step *= 0.98; // anneal
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;

    vector<Vec> pts;
    if (!special_points(n, pts)) {
        pts = fibonacci_sphere(n);
        relax_points(pts);
    }

    // Scale slightly inward to avoid any rounding pushing outside unit sphere
    const double shrink = 1.0 - 1e-12;
    for (int i = 0; i < n; ++i) {
        pts[i].x *= shrink;
        pts[i].y *= shrink;
        pts[i].z *= shrink;
    }

    // Compute minimum pairwise distance
    double min_d2 = 1e100;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = pts[i].x - pts[j].x;
            double dy = pts[i].y - pts[j].y;
            double dz = pts[i].z - pts[j].z;
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 < min_d2) min_d2 = d2;
        }
    }
    double min_dist = sqrt(min_d2);

    cout.setf(std::ios::fixed); 
    cout << setprecision(15) << min_dist << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setprecision(15) << pts[i].x << " " << pts[i].y << " " << pts[i].z << "\n";
    }
    return 0;
}