#include <bits/stdc++.h>

using namespace std;

const double EPS = 1e-10;

struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y, z - p.z); }
    Point operator*(double s) const { return Point(x * s, y * s, z * s); }
    double norm2() const { return x * x + y * y + z * z; }
    double norm() const { return sqrt(norm2()); }
};

double dist(const Point &a, const Point &b) { return (a - b).norm(); }

struct Ball {
    Point c;
    double r;
};

Ball trivial(const vector<Point> &R) {
    int m = R.size();
    if (m == 0) return {Point(), 0};
    if (m == 1) return {R[0], 0};
    if (m == 2) {
        Point mid = Point((R[0].x + R[1].x) / 2, (R[0].y + R[1].y) / 2, (R[0].z + R[1].z) / 2);
        return {mid, dist(R[0], R[1]) / 2};
    }
    if (m == 3) {
        Point A = R[0], B = R[1], C = R[2];
        Point ab = B - A;
        Point ac = C - A;
        Point cross = Point(ab.y * ac.z - ab.z * ac.y, ab.z * ac.x - ab.x * ac.z, ab.x * ac.y - ab.y * ac.x);
        if (cross.norm2() < EPS) { // collinear
            vector<double> d = {dist(R[0],R[1]), dist(R[0],R[2]), dist(R[1],R[2])};
            int maxid = max_element(d.begin(), d.end()) - d.begin();
            if (maxid == 0) return trivial({R[0], R[1]});
            if (maxid == 1) return trivial({R[0], R[2]});
            return trivial({R[1], R[2]});
        }
        double alpha = dist(B, C) * dist(B, C);
        double beta = dist(A, C) * dist(A, C);
        double gamma = dist(A, B) * dist(A, B);
        double sum = alpha + beta + gamma;
        Point o = Point( (alpha * A.x + beta * B.x + gamma * C.x) / sum,
                         (alpha * A.y + beta * B.y + gamma * C.y) / sum,
                         (alpha * A.z + beta * B.z + gamma * C.z) / sum );
        return {o, dist(o, A)};
    }
    if (m == 4) {
        Point p1 = R[0], p2 = R[1], p3 = R[2], p4 = R[3];
        Point v1 = p2 - p1; double d1 = (p2.norm2() - p1.norm2()) / 2;
        Point v2 = p3 - p1; double d2 = (p3.norm2() - p1.norm2()) / 2;
        Point v3 = p4 - p1; double d3 = (p4.norm2() - p1.norm2()) / 2;
        double det = v1.x * (v2.y * v3.z - v2.z * v3.y) - v1.y * (v2.x * v3.z - v2.z * v3.x) + v1.z * (v2.x * v3.y - v2.y * v3.x);
        if (abs(det) < EPS) {
            double min_r = 1e100;
            Ball best;
            int combs[4][3] = {{0,1,2}, {0,1,3}, {0,2,3}, {1,2,3}};
            for (int i = 0; i < 4; i++) {
                vector<Point> sub;
                for (int j = 0; j < 3; j++) sub.push_back(R[combs[i][j]]);
                Ball bb = trivial(sub);
                bool covers = true;
                for (int j = 0; j < 4; j++) {
                    if (dist(bb.c, R[j]) > bb.r + EPS) {
                        covers = false;
                        break;
                    }
                }
                if (covers && bb.r < min_r) {
                    min_r = bb.r;
                    best = bb;
                }
            }
            return best;
        }
        double detx = d1 * (v2.y * v3.z - v2.z * v3.y) - d2 * (v1.y * v3.z - v1.z * v3.y) + d3 * (v1.y * v2.z - v1.z * v2.y);
        double dety = v1.x * (d2 * v3.z - d3 * v2.z) - d1 * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * d3 - v3.x * d2);
        double detz = v1.x * (v2.y * d3 - v2.z * d2) - v1.y * (v2.x * d3 - v2.z * d2) + d1 * (v2.x * v3.y - v2.y * v3.x);
        double ox = detx / det;
        double oy = dety / det;
        double oz = detz / det;
        Point o(ox, oy, oz);
        return {o, dist(o, p1)};
    }
    assert(false);
    return {Point(), 0};
}

bool inside(const Ball &b, const Point &p) {
    return dist(b.c, p) <= b.r + EPS;
}

Ball welzl(vector<Point> &ps, int idx, vector<Point> r) {
    if (idx == 0 || r.size() == 4) {
        return trivial(r);
    }
    Ball b = welzl(ps, idx - 1, r);
    if (inside(b, ps[idx - 1])) return b;
    r.push_back(ps[idx - 1]);
    return welzl(ps, idx - 1, r);
}

int main() {
    int n;
    cin >> n;
    double a = sqrt(2.0);
    vector<Point> all_points;
    int N = 15;
    for (int m = -N; m <= N; m++) {
        for (int nn = -N; nn <= N; nn++) {
            for (int p = -N; p <= N; p++) {
                double bx[4] = {0, a/2, a/2, 0};
                double by[4] = {0, a/2, 0, a/2};
                double bz[4] = {0, 0, a/2, a/2};
                for (int b = 0; b < 4; b++) {
                    double px = m * a + bx[b];
                    double py = nn * a + by[b];
                    double pz = p * a + bz[b];
                    all_points.push_back(Point(px, py, pz));
                }
            }
        }
    }
    vector<pair<double, int>> norms(all_points.size());
    for (int i = 0; i < all_points.size(); i++) {
        norms[i] = {all_points[i].norm2(), i};
    }
    sort(norms.begin(), norms.end());
    vector<Point> selected;
    for (int i = 0; i < n; i++) {
        int id = norms[i].second;
        selected.push_back(all_points[id]);
    }
    random_shuffle(selected.begin(), selected.end());
    Ball b = welzl(selected, selected.size(), {});
    vector<Point> final_points;
    double s = 1.0 / b.r;
    for (auto p : selected) {
        Point q = (p - b.c) * s;
        final_points.push_back(q);
    }
    double min_d = 1e100;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            min_d = min(min_d, dist(final_points[i], final_points[j]));
        }
    }
    cout << fixed << setprecision(10) << min_d << endl;
    for (auto p : final_points) {
        cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << endl;
    }
    return 0;
}