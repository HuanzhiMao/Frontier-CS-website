#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  cin >> n;
  if(n == 2) {
    cout << fixed << setprecision(10) << 2.0 << endl;
    cout << 0.0 << " " << 0.0 << " " << 1.0 << endl;
    cout << 0.0 << " " << 0.0 << " " << -1.0 << endl;
    return 0;
  }
  if(n == 3) {
    double md = sqrt(3.0);
    cout << fixed << setprecision(10) << md << endl;
    cout << 1.0 << " " << 0.0 << " " << 0.0 << endl;
    cout << -0.5 << " " << sqrt(3.0)/2 << " " << 0.0 << endl;
    cout << -0.5 << " " << -sqrt(3.0)/2 << " " << 0.0 << endl;
    return 0;
  }
  if(n == 4) {
    double md = sqrt(8.0/3.0);
    double t = 1.0 / sqrt(3.0);
    cout << fixed << setprecision(10) << md << endl;
    cout << t << " " << t << " " << t << endl;
    cout << t << " " << -t << " " << -t << endl;
    cout << -t << " " << t << " " << -t << endl;
    cout << -t << " " << -t << " " << t << endl;
    return 0;
  }
  // general case
  srand(42);
  vector<array<double,3>> pos(n);
  for(int i=0; i<n; i++) {
    double x,y,z;
    do {
      x = ((double)rand() / RAND_MAX) * 2 - 1;
      y = ((double)rand() / RAND_MAX) * 2 - 1;
      z = ((double)rand() / RAND_MAX) * 2 - 1;
    } while (x*x + y*y + z*z > 1.0);
    pos[i] = {x, y, z};
  }
  // now simulation
  int max_iter = 200;
  if(n < 100) max_iter = 1000;
  else if(n < 500) max_iter = 500;
  else max_iter = 200;
  clock_t start_time = clock();
  for(int iter = 0; iter < max_iter; iter++) {
    vector<array<double,3>> force(n, {0.0,0.0,0.0});
    for(int i=0; i<n; i++) {
      for(int j=i+1; j<n; j++) {
        double dx = pos[j][0] - pos[i][0];
        double dy = pos[j][1] - pos[i][1];
        double dz = pos[j][2] - pos[i][2];
        double dist_sq = dx*dx + dy*dy + dz*dz;
        double dist = sqrt(dist_sq + 1e-20);
        double f = 1.0 / (dist_sq + 1e-8);
        double ux = dx / dist;
        double uy = dy / dist;
        double uz = dz / dist;
        if(dist == 0) {
          ux = uy = uz = 0;
        }
        force[i][0] -= ux * f;
        force[i][1] -= uy * f;
        force[i][2] -= uz * f;
        force[j][0] += ux * f;
        force[j][1] += uy * f;
        force[j][2] += uz * f;
      }
    }
    // find max_f
    double max_f = 0.0;
    for(int i=0; i<n; i++) {
      double fm = sqrt(force[i][0]*force[i][0] + force[i][1]*force[i][1] + force[i][2]*force[i][2]);
      if(fm > max_f) max_f = fm;
    }
    double step = (max_f > 1e-10) ? 0.01 / max_f : 0.0;
    // move
    for(int i=0; i<n; i++) {
      pos[i][0] += step * force[i][0];
      pos[i][1] += step * force[i][1];
      pos[i][2] += step * force[i][2];
      double norm_sq = pos[i][0]*pos[i][0] + pos[i][1]*pos[i][1] + pos[i][2]*pos[i][2];
      if(norm_sq > 1.0 + 1e-10) {
        double norm = sqrt(norm_sq);
        pos[i][0] = pos[i][0] / norm * 1.0;
        pos[i][1] = pos[i][1] / norm * 1.0;
        pos[i][2] = pos[i][2] / norm * 1.0;
      }
    }
    // check time
    double time_elapsed = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    if(time_elapsed > 1.5) break;
  }
  // now compute min_dist
  double min_dist = INFINITY;
  for(int i=0; i<n; i++) {
    for(int j=i+1; j<n; j++) {
      double dx = pos[j][0] - pos[i][0];
      double dy = pos[j][1] - pos[i][1];
      double dz = pos[j][2] - pos[i][2];
      double dist = sqrt(dx*dx + dy*dy + dz*dz);
      if(dist < min_dist) min_dist = dist;
    }
  }
  // output
  cout << fixed << setprecision(10) << min_dist << endl;
  for(auto& p : pos) {
    cout << fixed << setprecision(10) << p[0] << " " << p[1] << " " << p[2] << endl;
  }
  return 0;
}