#include <bits/stdc++.h>
using namespace std;

static const int N = 100000;
static const int STEP = 199; // < 2*min_r to guarantee at least one interior sample
static const double EPS = 1e-9;

double ask(int x1, int y1, int x2, int y2) {
    cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n" << flush;
    double res;
    if (!(cin >> res)) {
        exit(0);
    }
    return res;
}

double askVertical(int x) { return ask(x, 0, x, N); }
double askHorizontal(int y) { return ask(0, y, N, y); }

long long llround_pos(double x) {
    return (long long)floor(x + 0.5);
}

struct CoordResult {
    long long c; // center coordinate (integer)
    vector<double> r2vals; // estimates of r^2 from used points
};

CoordResult solveCoordinate(bool vertical) {
    int found = -1;
    double Lfound = 0.0;

    if (vertical) {
        for (int x = 0; x <= N; x += STEP) {
            double L = askVertical(x);
            if (L > EPS) {
                found = x;
                Lfound = L;
                break;
            }
        }
        if (found == -1) {
            // Fallback scan starting from 1 to be extra safe (shouldn't happen)
            for (int x = 1; x <= N; x += STEP) {
                double L = askVertical(x);
                if (L > EPS) {
                    found = x;
                    Lfound = L;
                    break;
                }
            }
        }
    } else {
        for (int y = 0; y <= N; y += STEP) {
            double L = askHorizontal(y);
            if (L > EPS) {
                found = y;
                Lfound = L;
                break;
            }
        }
        if (found == -1) {
            // Fallback scan starting from 1 to be extra safe (shouldn't happen)
            for (int y = 1; y <= N; y += STEP) {
                double L = askHorizontal(y);
                if (L > EPS) {
                    found = y;
                    Lfound = L;
                    break;
                }
            }
        }
    }

    if (found == -1) {
        // Should never happen with STEP < 200 and r >= 100
        // Output something to avoid undefined behavior
        cout << "answer 0 0 100\n" << flush;
        exit(0);
    }

    vector<pair<int,double>> pts; // (coord, length)
    pts.push_back({found, Lfound});

    auto queryAt = [&](int t)->double{
        if (vertical) return askVertical(t);
        else return askHorizontal(t);
    };

    // Query neighbors to ensure at least two inside points
    if (found - 1 >= 0) {
        double L = queryAt(found - 1);
        if (L > EPS) pts.push_back({found - 1, L});
    }
    if (found + 1 <= N) {
        double L = queryAt(found + 1);
        if (L > EPS) pts.push_back({found + 1, L});
    }

    // If still only one inside point (very unlikely), expand search around found
    int d = 2;
    while ((int)pts.size() < 2) {
        bool added = false;
        if (found - d >= 0) {
            double L = queryAt(found - d);
            if (L > EPS) { pts.push_back({found - d, L}); added = true; }
        }
        if ((int)pts.size() < 2 && found + d <= N) {
            double L = queryAt(found + d);
            if (L > EPS) { pts.push_back({found + d, L}); added = true; }
        }
        d <<= 1;
        if (d > N) break;
        if (!added && d > N) break;
    }

    // Choose two with maximum separation for better numerical stability
    pair<int,double> p1 = pts[0], p2 = pts[0];
    double best_sep = -1;
    for (size_t i = 0; i < pts.size(); ++i) {
        for (size_t j = i + 1; j < pts.size(); ++j) {
            double sep = abs(pts[j].first - pts[i].first);
            if (sep > best_sep) {
                best_sep = sep;
                p1 = pts[i];
                p2 = pts[j];
            }
        }
    }
    if (best_sep < 1) {
        // Fallback: if only one point somehow, duplicate neighbor (should not happen)
        if (pts.size() == 1) {
            // Try to ensure a neighbor
            int t = pts[0].first + 1 <= N ? pts[0].first + 1 : pts[0].first - 1;
            double L = queryAt(t);
            if (L <= EPS) {
                // force slight shift to get inside, move towards center direction unknown; try multiple
                for (int shift = 2; shift <= 1000 && L <= EPS; ++shift) {
                    int t1 = pts[0].first + shift;
                    if (t1 <= N) {
                        L = queryAt(t1);
                        if (L > EPS) { p2 = {t1, L}; break; }
                    }
                    int t2 = pts[0].first - shift;
                    if (t2 >= 0) {
                        L = queryAt(t2);
                        if (L > EPS) { p2 = {t2, L}; break; }
                    }
                }
            } else {
                p2 = {t, L};
            }
            p1 = pts[0];
        } else {
            p1 = pts[0];
            p2 = pts[1];
        }
    }

    auto computeA = [&](double L)->double{
        return 0.25 * L * L;
    };

    double X1 = (double)p1.first, X2 = (double)p2.first;
    double A1 = computeA(p1.second);
    double A2 = computeA(p2.second);
    double g1 = -X1*X1 - A1;
    double g2 = -X2*X2 - A2;

    double m = (g2 - g1) / (X2 - X1);
    double c_est = -m / 2.0;
    long long c_int = llround_pos(c_est);
    if (c_int < 0) c_int = 0;
    if (c_int > N) c_int = N;

    // Compute r^2 estimates from all inside points we have
    vector<double> r2vals;
    for (auto &pt : pts) {
        double Ai = computeA(pt.second);
        double di = (double)pt.first - (double)c_int;
        r2vals.push_back(Ai + di*di);
    }

    return CoordResult{c_int, r2vals};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    CoordResult xres = solveCoordinate(true);
    CoordResult yres = solveCoordinate(false);

    long long cx = xres.c;
    long long cy = yres.c;

    vector<double> r2s;
    r2s.insert(r2s.end(), xres.r2vals.begin(), xres.r2vals.end());
    r2s.insert(r2s.end(), yres.r2vals.begin(), yres.r2vals.end());

    double r2_avg = 0.0;
    for (double v : r2s) r2_avg += v;
    r2_avg /= (double)r2s.size();

    long long r = llround_pos(sqrt(max(0.0, r2_avg)));
    if (r < 0) r = 0;
    if (r > N) r = N;

    cout << "answer " << cx << " " << cy << " " << r << "\n" << flush;
    return 0;
}