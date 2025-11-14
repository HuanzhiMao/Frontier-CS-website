#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

using ld = long double;

int N;
ld current_x;
const ld EPS = 1e-5;

// Queries the interactor for the sum of distances from point (x, y) to all N lines.
ld query(ld x, ld y) {
    cout << "? " << fixed << setprecision(10) << x << " " << y << endl;
    ld response;
    cin >> response;
    return response;
}

// Numerically calculates the slope of the queried function with respect to y, at a fixed x.
ld get_slope_y(ld y) {
    ld res1 = query(current_x, y - EPS);
    ld res2 = query(current_x, y + EPS);
    return (res2 - res1) / (2.0L * EPS);
}

map<long long, ld> value_to_C_inv_sum;

// Recursively finds the locations (values of a*x+b) where the slope of the distance sum changes.
// These locations are the "kinks" in the otherwise piecewise linear function.
// The magnitude of the slope change reveals the value of C_i = sqrt(a_i^2 + 1).
void find_kinks(ld L, ld R, ld slope_L, ld slope_R) {
    // If the slope doesn't change significantly over the interval, there are no kinks.
    if (abs(slope_R - slope_L) < 1e-9 * N) {
        return;
    }

    // If the interval is small enough to contain at most one integer, we've found a kink.
    if (R - L < 0.8L) {
        // The total change in slope is 2 * sum(1/C_i) for all lines whose value a_i*x+b_i is in this interval.
        ld C_inv_sum = (slope_R - slope_L) / 2.0L;
        value_to_C_inv_sum[round(L)] += C_inv_sum;
        return;
    }
    
    ld M = L + (R - L) / 2.0L;
    ld slope_M = get_slope_y(M);
    
    find_kinks(L, M, slope_L, slope_M);
    find_kinks(M, R, slope_M, slope_R);
}

// For a given x, this function identifies all pairs (a_i*x + b_i, C_i).
vector<pair<ld, ld>> solve_for_x(ld x_val) {
    current_x = x_val;
    value_to_C_inv_sum.clear();
    
    ld abs_x = abs(current_x);
    if (abs_x < 1.0L) abs_x = 1.0L;

    // Determine a search range for y that is guaranteed to contain all a_i*x + b_i values.
    ld min_val = -10000.0L * abs_x - 10000.0L - 5.0L;
    ld max_val = 10000.0L * abs_x + 10000.0L + 5.0L;
    
    // Get slopes at the boundaries of the search range.
    ld slope_min = get_slope_y(min_val);
    ld slope_max = get_slope_y(max_val);
    
    // Start the recursive search for kinks.
    find_kinks(min_val, max_val, slope_min, slope_max);
    
    vector<pair<ld, ld>> result;
    for (auto const& [val, C_inv] : value_to_C_inv_sum) {
        if (abs(C_inv) > 1e-9) { // Filter out floating point noise
            result.push_back({1.0L / C_inv, (ld)val});
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    // First run with x=1 to get pairs (C_i, a_i + b_i)
    ld x1 = 1.0;
    vector<pair<ld, ld>> res1 = solve_for_x(x1);
    sort(res1.begin(), res1.end());

    // Second run with x=2 to get pairs (C_i, 2*a_i + b_i)
    ld x2 = 2.0;
    vector<pair<ld, ld>> res2 = solve_for_x(x2);
    sort(res2.begin(), res2.end());
    
    vector<long long> a(N), b(N);
    for (int i = 0; i < N; ++i) {
        // Since C_i = sqrt(a_i^2+1) is unique for distinct a_i, we can match pairs by sorting on C.
        ld C = res1[i].first;
        ld u = res1[i].second; // u = a_i*x1 + b_i
        ld v = res2[i].second; // v = a_i*x2 + b_i
        
        // Solve the system of two linear equations for a_i and b_i.
        // v - u = a_i * (x2 - x1)
        a[i] = round((v - u) / (x2 - x1));
        b[i] = round(u - a[i] * x1);
    }
    
    cout << "! ";
    for (int i = 0; i < N; ++i) cout << a[i] << (i == N - 1 ? "" : " ");
    cout << " ";
    for (int i = 0; i < N; ++i) cout << b[i] << (i == N - 1 ? "" : " ");
    cout << endl;

    return 0;
}