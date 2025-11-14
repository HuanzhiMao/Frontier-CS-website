#include <bits/stdc++.h>
using namespace std;

static const int MAXC = 100000;
static const int STEP = 100;
static const double EPS = 1e-7;

double ask(int x1, int y1, int x2, int y2) {
    cout << "query " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    cout.flush();
    double res;
    if (!(cin >> res)) {
        exit(0);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> xs;
    vector<double> ls;

    // Vertical sweep with step 100
    int x_max = 0;
    double l_max = -1.0;
    vector<pair<int,double>> positives;
    for (int x = 0; x <= MAXC; x += STEP) {
        double L = ask(x, 0, x, MAXC);
        xs.push_back(x);
        ls.push_back(L);
        if (L > l_max) {
            l_max = L;
            x_max = x;
        }
        if (L > EPS) {
            positives.emplace_back(x, L);
        }
    }

    // Compute cx and r using two positive lines if available
    long double cx_est = x_max;
    int cx = x_max;
    long double r_est = max(0.0, l_max / 2.0);
    int r = (int)llround(r_est);

    if (positives.size() >= 2) {
        // choose two with largest lengths
        nth_element(positives.begin(), positives.begin(), positives.end(),
                    [](const pair<int,double>& a, const pair<int,double>& b){ return a.second > b.second; });
        pair<int,double> p1 = positives[0];

        pair<int,double> p2 = p1;
        double second_best = -1.0;
        for (auto &p : positives) {
            if (p.first == p1.first) continue;
            if (p.second > second_best) {
                second_best = p.second;
                p2 = p;
            }
        }
        int x1 = p1.first, x2 = p2.first;
        long double L1 = p1.second, L2 = p2.second;

        // cx formula using two chords on vertical lines
        long double num = (long double)x1 * x1 - (long double)x2 * x2 + 0.25L * (L1 * L1 - L2 * L2);
        long double den = 2.0L * ((long double)x1 - (long double)x2);
        cx_est = num / den;
        cx = (int)llround(cx_est);

        // local adjustment: check cx-1, cx, cx+1 to ensure center
        int best_x = cx;
        double best_L = ask(cx, 0, cx, MAXC);
        if (cx - 1 >= 0) {
            double Lm = ask(cx - 1, 0, cx - 1, MAXC);
            if (Lm > best_L) { best_L = Lm; best_x = cx - 1; }
        }
        if (cx + 1 <= MAXC) {
            double Lp = ask(cx + 1, 0, cx + 1, MAXC);
            if (Lp > best_L) { best_L = Lp; best_x = cx + 1; }
        }
        cx = best_x;
        r = (int)llround(best_L / 2.0);
    } else {
        // Only one positive; center is at that x (likely r=100 and cx%100==0)
        // local adjustment around this cx to confirm and compute r precisely
        int best_x = cx;
        double best_L = l_max;
        if (cx - 1 >= 0) {
            double Lm = ask(cx - 1, 0, cx - 1, MAXC);
            if (Lm > best_L) { best_L = Lm; best_x = cx - 1; }
        }
        if (cx + 1 <= MAXC) {
            double Lp = ask(cx + 1, 0, cx + 1, MAXC);
            if (Lp > best_L) { best_L = Lp; best_x = cx + 1; }
        }
        cx = best_x;
        r = (int)llround(best_L / 2.0);
    }

    // Binary search for y_low = cy - r using vertical segment at x = cx
    int lo = 0, hi = MAXC;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        double L = ask(cx, 0, cx, mid);
        if (L > EPS) hi = mid;
        else lo = mid + 1;
    }
    int y_low = lo; // equals cy - r (since cx is exact center x)
    int cy = y_low + r;

    // Output the final answer
    cout << "answer " << cx << " " << cy << " " << r << endl;
    cout.flush();
    return 0;
}