#include <bits/stdc++.h>

using namespace std;

struct Point {
    double x, y, z;
};

double radical_inverse(int base, int i) {
    double res = 0;
    double f = 1.0 / base;
    while (i > 0) {
        res += f * (i % base);
        i /= base;
        f /= base;
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    vector<Point> points(n);
    int bases[3] = {2, 3, 5};
    for (int i = 0; i < n; i++) {
        points[i].x = radical_inverse(bases[0], i + 1);
        points[i].y = radical_inverse(bases[1], i + 1);
        points[i].z = radical_inverse(bases[2], i + 1);
    }
    double cb = pow(n, 1.0 / 3.0);
    double dt = 0.02 / cb;
    int max_iter = 200;
    for (int iter = 0; iter < max_iter; iter++) {
        vector<Point> forces(n, {0,0,0});
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double dx = points[j].x - points[i].x;
                double dy = points[j].y - points[i].y;
                double dz = points[j].z - points[i].z;
                double dist2 = dx*dx + dy*dy + dz*dz;
                if (dist2 < 1e-10) dist2 = 1e-10;
                double dist = sqrt(dist2);
                double force = 1.0 / (dist2);
                double fx = dx / dist * force;
                double fy = dy / dist * force;
                double fz = dz / dist * force;
                forces[i].x += fx;
                forces[i].y += fy;
                forces[i].z += fz;
                forces[j].x -= fx;
                forces[j].y -= fy;
                forces[j].z -= fz;
            }
        }
        // boundary forces
        for (int i = 0; i < n; i++) {
            // x0
            double dist = points[i].x;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].x += 1.0 / (dist * dist);
            // x1
            dist = 1.0 - points[i].x;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].x -= 1.0 / (dist * dist);
            // y0
            dist = points[i].y;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].y += 1.0 / (dist * dist);
            // y1
            dist = 1.0 - points[i].y;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].y -= 1.0 / (dist * dist);
            // z0
            dist = points[i].z;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].z += 1.0 / (dist * dist);
            // z1
            dist = 1.0 - points[i].z;
            if (dist < 1e-10) dist = 1e-10;
            forces[i].z -= 1.0 / (dist * dist);
        }
        // update
        double max_delta = 0;
        for (int i = 0; i < n; i++) {
            double dx = forces[i].x * dt;
            double dy = forces[i].y * dt;
            double dz = forces[i].z * dt;
            points[i].x += dx;
            points[i].y += dy;
            points[i].z += dz;
            if (points[i].x < 0) points[i].x = 0;
            if (points[i].x > 1) points[i].x = 1;
            if (points[i].y < 0) points[i].y = 0;
            if (points[i].y > 1) points[i].y = 1;
            if (points[i].z < 0) points[i].z = 0;
            if (points[i].z > 1) points[i].z = 1;
            double delta = sqrt(dx*dx + dy*dy + dz*dz);
            if (delta > max_delta) max_delta = delta;
        }
        if (max_delta < 1e-5) break;
    }
    for (int i = 0; i < n; i++) {
        printf("%.10f %.10f %.10f\n", points[i].x, points[i].y, points[i].z);
    }
    return 0;
}