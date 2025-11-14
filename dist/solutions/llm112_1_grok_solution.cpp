#include <bits/stdc++.h>

using namespace std;

struct Point {
  double x, y, z;
  Point(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {}
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }
  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }
  Point operator*(double s) const { return Point(x * s, y * s, z * s); }
  double norm2() const { return x * x + y * y + z * z; }
  double norm() const { return sqrt(norm2()); }
  Point normalize() const { return *this * (1.0 / norm()); }
};

double compute_min_dist(const vector<Point>& points) {
  int n = points.size();
  double min_d = 1e9;
  for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
    double d = (points[i] - points[j]).norm();
    min_d = min(min_d, d);
  }
  return min_d;
}

int main() {
  int n;
  cin >> n;
  vector<Point> points(n);
  if (n <= 12) {
    double golden = (1 + sqrt(5)) / 2;
    for (int i = 0; i < n; i++) {
      double k = i + 0.5;
      double ph = acos(1 - 2 * k / n);
      double th = M_PI * golden * k;
      double sx = sin(ph) * cos(th);
      double sy = sin(ph) * sin(th);
      double sz = cos(ph);
      points[i] = Point(sx, sy, sz);
    }
    int num_iters = 5000;
    double dt = 0.02;
    for (int iter = 0; iter < num_iters; iter++) {
      vector<Point> forces(n);
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          Point diff = points[i] - points[j];
          double d2 = diff.norm2();
          if (d2 < 1e-10) continue;
          double inv = 1.0 / (d2 * sqrt(d2));
          Point f = diff * inv;
          forces[i] = forces[i] + f;
          forces[j] = forces[j] - f;
        }
      }
      for (int i = 0; i < n; i++) {
        Point newp = points[i] + forces[i] * dt;
        double r = newp.norm();
        if (r > 1.0) {
          newp = newp.normalize();
        }
        points[i] = newp;
      }
    }
  } else {
    double s = 1.0 / sqrt(2.0);
    Point v1(0, s, s);
    Point v2(s, 0, s);
    Point v3(s, s, 0);
    vector<pair<double, Point>> list;
    int M = 25;
    for (int m = -M; m <= M; m++) {
      for (int nn = -M; nn <= M; nn++) {
        for (int k = -M; k <= M; k++) {
          double xx = m * v1.x + nn * v2.x + k * v3.x;
          double yy = m * v1.y + nn * v2.y + k * v3.y;
          double zz = m * v1.z + nn * v2.z + k * v3.z;
          double r2 = xx * xx + yy * yy + zz * zz;
          if (r2 > 100) continue;
          double r = sqrt(r2);
          list.push_back({r, Point(xx, yy, zz)});
        }
      }
    }
    sort(list.begin(), list.end());
    double max_r = list[n - 1].first;
    for (int i = 0; i < n; i++) {
      Point p = list[i].second * (1.0 / max_r);
      points[i] = p;
    }
  }
  double min_dist = compute_min_dist(points);
  cout << fixed << setprecision(10) << min_dist << endl;
  for (auto& p : points) {
    cout << fixed << setprecision(10) << p.x << " " << p.y << " " << p.z << endl;
  }
  return 0;
}