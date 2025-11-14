#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <set>

using namespace std;

struct Point {
    double x, y, z;
};

struct IntPoint {
    int x, y, z;
    bool operator<(const IntPoint& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
};

// --- Method 1: Simple Cubic Grid ---
pair<double, vector<Point>> solve_grid(int n) {
    if (n <= 1) {
        return {0.5, {{0.5, 0.5, 0.5}}};
    }
    int best_m = n, best_k = 1, best_l = 1;
    int min_max_dim = n;

    for (int m = 1; m <= n; ++m) {
        if (m > min_max_dim) break;
        for (int k = 1; (long long)m * k <= n; ++k) {
            if (max(m, k) > min_max_dim) break;
            int l = (n + (long long)m * k - 1) / ((long long)m * k);
            if (max({m, k, l}) < min_max_dim) {
                min_max_dim = max({m, k, l});
                best_m = m;
                best_k = k;
                best_l = l;
            }
        }
    }
    
    double r = 0.5 / min_max_dim;
    vector<Point> centers;
    int count = 0;
    for (int i = 0; i < best_m && count < n; ++i) {
        for (int j = 0; j < best_k && count < n; ++j) {
            for (int p = 0; p < best_l && count < n; ++p) {
                centers.push_back({(i + 0.5) / best_m, (j + 0.5) / best_k, (p + 0.5) / best_l});
                count++;
            }
        }
    }
    return {r, centers};
}

// --- Method 2 & 3: Lattice-based (FCC/BCC) ---
vector<IntPoint> generate_lattice_points(int n, bool is_fcc) {
    vector<pair<int, IntPoint>> candidates;
    
    for (long long d_sq = 0; candidates.size() < n; ++d_sq) {
        if (d_sq == 0) {
            candidates.push_back({0, {0, 0, 0}});
            continue;
        }
        for (int i = 0; (long long)i * i <= d_sq; ++i) {
            for (int j = i; (long long)i * i + (long long)j * j <= d_sq; ++j) {
                long long k_sq_ll = d_sq - (long long)i * i - (long long)j * j;
                int k = round(sqrt(k_sq_ll));
                if ((long long)k * k != k_sq_ll) continue;
                if (k < j) continue;

                bool parity_ok = false;
                if (is_fcc) {
                    if ((i + j + k) % 2 == 0) parity_ok = true;
                } else { // BCC
                    if ((i % 2 == j % 2) && (j % 2 == k % 2)) parity_ok = true;
                }

                if (parity_ok) {
                    vector<int> p_mags = {i, j, k};
                    sort(p_mags.begin(), p_mags.end());
                    set<IntPoint> unique_points;
                    do {
                        for (int sx = (p_mags[0] == 0 ? 1 : -1); sx <= 1; sx += 2) {
                            for (int sy = (p_mags[1] == 0 ? 1 : -1); sy <= 1; sy += 2) {
                                for (int sz = (p_mags[2] == 0 ? 1 : -1); sz <= 1; sz += 2) {
                                    unique_points.insert({sx * p_mags[0], sy * p_mags[1], sz * p_mags[2]});
                                }
                            }
                        }
                    } while (next_permutation(p_mags.begin(), p_mags.end()));
                    
                    for (const auto& pt : unique_points) {
                        candidates.push_back({(int)d_sq, pt});
                    }
                }
            }
        }
    }
    
    sort(candidates.begin(), candidates.end());
    vector<IntPoint> points;
    for(size_t i=0; i < n && i < candidates.size(); ++i) {
        points.push_back(candidates[i].second);
    }
    return points;
}

pair<double, vector<Point>> solve_lattice(int n, bool is_fcc) {
    if (n <= 1) {
        return {0.5, {{0.5, 0.5, 0.5}}};
    }
    
    vector<IntPoint> lattice_points = generate_lattice_points(n, is_fcc);
    
    int min_x = 1e9, max_x = -1e9;
    int min_y = 1e9, max_y = -1e9;
    int min_z = 1e9, max_z = -1e9;

    for (const auto& p : lattice_points) {
        min_x = min(min_x, p.x); max_x = max(max_x, p.x);
        min_y = min(min_y, p.y); max_y = max(max_y, p.y);
        min_z = min(min_z, p.z); max_z = max(max_z, p.z);
    }
    
    double Lx = max_x - min_x;
    double Ly = max_y - min_y;
    double Lz = max_z - min_z;
    double L = max({Lx, Ly, Lz});

    if (L == 0) {
      return {0.0, {}};
    }

    double min_dist_lattice = is_fcc ? sqrt(2.0) : sqrt(3.0);
    
    double r = min_dist_lattice / (2.0 * (L + min_dist_lattice));
    double scale = (1.0 - 2.0 * r) / L;

    double center_offset_x = (1.0 - 2.0 * r - Lx * scale) / 2.0;
    double center_offset_y = (1.0 - 2.0 * r - Ly * scale) / 2.0;
    double center_offset_z = (1.0 - 2.0 * r - Lz * scale) / 2.0;

    double total_offset_x = r - (double)min_x * scale + center_offset_x;
    double total_offset_y = r - (double)min_y * scale + center_offset_y;
    double total_offset_z = r - (double)min_z * scale + center_offset_z;
    
    vector<Point> centers;
    for (const auto& p : lattice_points) {
        centers.push_back({
            total_offset_x + (double)p.x * scale,
            total_offset_y + (double)p.y * scale,
            total_offset_z + (double)p.z * scale
        });
    }

    return {r, centers};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    auto res_grid = solve_grid(n);
    auto res_fcc = solve_lattice(n, true);
    auto res_bcc = solve_lattice(n, false);
    
    double best_r = 0.0;
    vector<Point> best_centers;

    if (res_grid.first > best_r) {
        best_r = res_grid.first;
        best_centers = res_grid.second;
    }
    if (res_fcc.first > best_r) {
        best_r = res_fcc.first;
        best_centers = res_fcc.second;
    }
    if (res_bcc.first > best_r) {
        best_r = res_bcc.first;
        best_centers = res_bcc.second;
    }
    
    cout << fixed << setprecision(17);
    for (const auto& p : best_centers) {
        cout << p.x << " " << p.y << " " << p.z << "\n";
    }

    return 0;
}