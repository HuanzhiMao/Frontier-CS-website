#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

typedef long long ll;
typedef long double ld;

// A cache to store query results for a fixed x to reduce redundant queries.
map<ll, ld> query_cache;
ll fixed_x;

// Function to make a query to the interactor.
ld ask(ll x, ll y) {
    if (x == fixed_x && query_cache.count(y)) {
        return query_cache[y];
    }
    cout << "? " << x << " " << y << endl;
    ld dist;
    cin >> dist;
    if (x == fixed_x) {
        query_cache[y] = dist;
    }
    return dist;
}

struct Line {
    ll a, b;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Choose X > 2*10^4 to guarantee y_i = a_i*X + b_i are distinct.
    const ll X = 20001;
    fixed_x = X;
    
    vector<Line> found_lines;

    // The range of y_i values.
    ll R = X * 10000LL + 10000LL;
    ll L = -R;

    query_cache.clear();
    // The derivative of D(X, y) for y > max(y_i)
    ld current_slope = ask(X, R + 1) - ask(X, R);

    ll upper_y = R + 1;

    for (int i = 0; i < n; ++i) {
        ll low = L, high = upper_y - 2;
        ll kink_y_minus_1 = L - 1;

        // Binary search for the largest y where the derivative changes.
        while (low <= high) {
            ll mid = low + (high - low) / 2;
            if (mid >= upper_y - 1) {
                high = mid - 1;
                continue;
            }
            ld slope_at_mid = ask(X, mid + 1) - ask(X, mid);
            if (abs(slope_at_mid - current_slope) > 1e-9) {
                kink_y_minus_1 = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        ll y_kink = kink_y_minus_1 + 1;
        
        // The derivative value just below the kink.
        ld new_slope = ask(X, y_kink) - ask(X, y_kink - 1);
        
        // The change in derivative at the kink gives us c_i.
        ld sum_2_over_c = current_slope - new_slope;
        ld c = 2.0 / sum_2_over_c;

        // From c_i, find |a_i|.
        ld a_sq = c * c - 1.0;
        ll a_abs = round(sqrt(max((ld)0.0, a_sq)));
        
        // Two candidates for b_i based on a_i = +|a_i| or a_i = -|a_i|.
        ll b_cand1 = y_kink - a_abs * X;
        ll b_cand2 = y_kink + a_abs * X;

        // Only one candidate for b_i will be in the valid range [-10000, 10000].
        if (abs(b_cand1) <= 10000) {
            found_lines.push_back({a_abs, b_cand1});
        } else {
            found_lines.push_back({-a_abs, b_cand2});
        }
        
        // Update for the next kink search.
        current_slope = new_slope;
        upper_y = y_kink;
    }

    cout << "!" << flush;
    for (int i = 0; i < n; ++i) {
        cout << " " << found_lines[i].a << flush;
    }
    for (int i = 0; i < n; ++i) {
        cout << " " << found_lines[i].b << flush;
    }
    cout << endl;

    return 0;
}