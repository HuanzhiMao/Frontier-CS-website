#include <bits/stdc++.h>
using namespace std;

struct Vec {
    double x, y, z;
    Vec() : x(0), y(0), z(0) {}
    Vec(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y, z + o.z); }
    Vec operator-(const Vec& o) const { return Vec(x - o.x, y - o.y, z - o.z); }
    Vec operator*(double k) const { return Vec(x * k, y * k, z * k); }
    Vec operator/(double k) const { return Vec(x / k, y / k, z / k); }
    Vec& operator+=(const Vec& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec& operator-=(const Vec& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
};

static inline double dot(const Vec& a, const Vec& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline double norm2(const Vec& a) { return dot(a, a); }
static inline double norm(const Vec& a) { return sqrt(norm2(a)); }
static inline void normalize(Vec& a) {
    double l = norm(a);
    if (l > 0) { a.x /= l; a.y /= l; a.z /= l; }
}

vector<Vec> specialCasePoints(int n) {
    vector<Vec> p;
    const double PI = acos(-1.0);
    if (n == 2) {
        p.push_back(Vec(0, 0, 1));
        p.push_back(Vec(0, 0, -1));
    } else if (n == 3) {
        for (int k = 0; k < 3; ++k) {
            double phi = 2 * PI * k / 3.0;
            p.emplace_back(cos(phi), sin(phi), 0.0);
        }
    } else if (n == 4) {
        double s = 1.0 / sqrt(3.0);
        p.emplace_back( s,  s,  s);
        p.emplace_back( s, -s, -s);
        p.emplace_back(-s,  s, -s);
        p.emplace_back(-s, -s,  s);
    } else if (n == 6) {
        p.emplace_back(1, 0, 0);
        p.emplace_back(-1, 0, 0);
        p.emplace_back(0, 1, 0);
        p.emplace_back(0, -1, 0);
        p.emplace_back(0, 0, 1);
        p.emplace_back(0, 0, -1);
    } else if (n == 8) {
        double s = 1.0 / sqrt(3.0);
        for (int sx : {-1, 1})
            for (int sy : {-1, 1})
                for (int sz : {-1, 1})
                    p.emplace_back(sx * s, sy * s, sz * s);
    } else if (n == 12) {
        // Icosahedron
        double phi = (1.0 + sqrt(5.0)) * 0.5;
        vector<Vec> raw = {
            {0, -1, -phi}, {0, -1, phi}, {0, 1, -phi}, {0, 1, phi},
            {-1, -phi, 0}, {-1, phi, 0}, {1, -phi, 0}, {1, phi, 0},
            {-phi, 0, -1}, {phi, 0, -1}, {-phi, 0, 1}, {phi, 0, 1}
        };
        double L = sqrt(1.0 + phi*phi);
        for (auto &v : raw) p.emplace_back(v.x / L, v.y / L, v.z / L);
    }
    return p;
}

vector<Vec> sphericalFibonacci(int n, uint64_t seed = 123456789) {
    vector<Vec> p(n);
    const double PI = acos(-1.0);
    const double ga = PI * (3.0 - sqrt(5.0)); // golden angle
    for (int k = 0; k < n; ++k) {
        double z = 1.0 - 2.0 * (k + 0.5) / n;
        double r = sqrt(max(0.0, 1.0 - z * z));
        double phi = k * ga;
        double x = cos(phi) * r;
        double y = sin(phi) * r;
        p[k] = Vec(x, y, z);
    }
    // Optional: slight random rotation to avoid pathological alignments
    // Create a random axis and angle and rotate all points
    std::mt19937_64 rng(seed);
    uniform_real_distribution<double> U01(0.0, 1.0);
    double ax = U01(rng) * 2.0 - 1.0;
    double ay = U01(rng) * 2.0 - 1.0;
    double az = U01(rng) * 2.0 - 1.0;
    Vec axis(ax, ay, az);
    if (norm2(axis) < 1e-20) axis = Vec(1, 0, 0);
    normalize(axis);
    double angle = U01(rng) * 2.0 * PI;
    double c = cos(angle), s = sin(angle);
    double ux = axis.x, uy = axis.y, uz = axis.z;
    // Rodrigues' rotation formula matrix
    double R[3][3] = {
        {c + ux*ux*(1-c),      ux*uy*(1-c) - uz*s, ux*uz*(1-c) + uy*s},
        {uy*ux*(1-c) + uz*s,   c + uy*uy*(1-c),    uy*uz*(1-c) - ux*s},
        {uz*ux*(1-c) - uy*s,   uz*uy*(1-c) + ux*s, c + uz*uz*(1-c)}
    };
    for (int i = 0; i < n; ++i) {
        Vec v = p[i];
        p[i].x = R[0][0]*v.x + R[0][1]*v.y + R[0][2]*v.z;
        p[i].y = R[1][0]*v.x + R[1][1]*v.y + R[1][2]*v.z;
        p[i].z = R[2][0]*v.x + R[2][1]*v.y + R[2][2]*v.z;
        normalize(p[i]);
    }
    return p;
}

void relax_on_sphere(vector<Vec>& p, int iterations, double step0, double step_min = 0.002) {
    int n = (int)p.size();
    if (n <= 1 || iterations <= 0) return;
    vector<Vec> f(n);
    const double EPS = 1e-12;
    for (int it = 0; it < iterations; ++it) {
        // reset forces
        for (int i = 0; i < n; ++i) f[i] = Vec(0, 0, 0);
        // accumulate pairwise symmetric forces
        for (int i = 0; i < n; ++i) {
            const Vec &pi = p[i];
            for (int j = i + 1; j < n; ++j) {
                Vec diff = Vec(pi.x - p[j].x, pi.y - p[j].y, pi.z - p[j].z);
                double d2 = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z + EPS;
                // Use inverse-square-like repulsion
                double w = 1.0 / d2;
                Vec fv = diff * w;
                f[i] += fv;
                f[j] -= fv;
            }
        }
        double alpha = (iterations > 1) ? (double)it / (double)(iterations - 1) : 1.0;
        double step = step0 * (1.0 - alpha) + step_min;
        // project to tangent and update
        for (int i = 0; i < n; ++i) {
            // project force onto tangent plane at p[i]
            double fdotp = f[i].x * p[i].x + f[i].y * p[i].y + f[i].z * p[i].z;
            Vec ft = Vec(f[i].x - fdotp * p[i].x, f[i].y - fdotp * p[i].y, f[i].z - fdotp * p[i].z);
            double fl = norm(ft);
            if (fl > 1e-18) {
                Vec dir = ft / fl;
                Vec ni = Vec(p[i].x + step * dir.x, p[i].y + step * dir.y, p[i].z + step * dir.z);
                normalize(ni);
                p[i] = ni;
            }
        }
    }
    // final normalization to ensure on/within sphere
    for (int i = 0; i < n; ++i) {
        double l2 = norm2(p[i]);
        if (l2 > 1.0) {
            double l = sqrt(l2);
            p[i] = p[i] / l;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;

    vector<Vec> pts = specialCasePoints(n);
    if ((int)pts.size() != n) {
        // Generate initial points
        uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        pts = sphericalFibonacci(n, seed);

        // Determine iterations and step based on n
        int iterations;
        double step0;
        if (n <= 10) { iterations = 300; step0 = 0.25; }
        else if (n <= 30) { iterations = 200; step0 = 0.18; }
        else if (n <= 100) { iterations = 150; step0 = 0.12; }
        else if (n <= 300) { iterations = 100; step0 = 0.08; }
        else if (n <= 600) { iterations = 60; step0 = 0.06; }
        else { iterations = 40; step0 = 0.05; }

        relax_on_sphere(pts, iterations, step0, 0.002);
    }

    // Ensure all points are within or on the unit sphere
    for (auto &v : pts) {
        double l2 = norm2(v);
        if (l2 > 1.0) {
            double l = sqrt(l2);
            v = v / l;
        }
    }

    // Compute minimum pairwise distance
    double best2 = 1e300;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Vec d = pts[i] - pts[j];
            double d2 = d.x*d.x + d.y*d.y + d.z*d.z;
            if (d2 < best2) best2 = d2;
        }
    }
    double min_dist = sqrt(best2);

    cout.setf(std::ios::fixed); 
    cout << setprecision(12) << min_dist << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setprecision(12) << pts[i].x << " " << pts[i].y << " " << pts[i].z << "\n";
    }
    return 0;
}