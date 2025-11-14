#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <algorithm>
#include <queue>
#include <tuple>

using namespace std;

using ld = long double;

int N;
const ld X = 50000;
const ld Y_MAX = 5.1e8;
const ld G_PRIME_EPS = 1e-11;
const ld Y_EPS = 1e-3;

map<ld, int> w_to_abs_a;

struct Kink {
    ld y;
    ld w;
};

ld query(ld qx, ld qy) {
    cout << "? " << fixed << setprecision(10) << qx << " " << qy << endl;
    ld result;
    cin >> result;
    return result;
}

ld g_prime(ld y) {
    ld h = 1e-5;
    return (query(X, y + h) - query(X, y - h)) / (2.0L * h);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    for (int a = 0; a <= 10000; ++a) {
        ld w = 1.0L / sqrtl((ld)a * a + 1.0L);
        w_to_abs_a[w] = a;
    }

    vector<Kink> kinks;
    
    queue<tuple<ld, ld, ld, ld>> q;

    ld v_low = g_prime(-Y_MAX);
    ld v_high = g_prime(Y_MAX);

    q.emplace(-Y_MAX, Y_MAX, v_low, v_high);

    while (!q.empty()) {
        auto [L, R, vL, vR] = q.front();
        q.pop();

        if (abs(vL - vR) < G_PRIME_EPS) {
            continue;
        }

        if (R - L < Y_EPS) {
            kinks.push_back({(L + R) / 2.0L, (vR - vL) / 2.0L});
            continue;
        }

        ld mid = L + (R - L) / 2.0L;
        ld v_mid = g_prime(mid);
        
        q.emplace(L, mid, vL, v_mid);
        q.emplace(mid, R, v_mid, vR);
    }
    
    vector<long long> a_s, b_s;

    for (const auto& kink : kinks) {
        ld y = kink.y;
        ld w = kink.w;

        auto it = w_to_abs_a.lower_bound(w);
        int abs_a = -1;

        if (it != w_to_abs_a.end()) {
            if (abs(it->first - w) < 1e-9 * w) { // Relative error check
                abs_a = it->second;
            }
        }
        if (abs_a == -1 && it != w_to_abs_a.begin()) {
            it--;
            if (abs(it->first - w) < 1e-9 * w) {
                abs_a = it->second;
            }
        }
        
        if (abs_a == -1) {
            // Should not happen with proper precision and logic
            continue;
        }

        long long a = roundl(y / X);
        
        if (std::abs(a) != abs_a) {
             a = (a > 0 ? abs_a : -abs_a);
        }

        long long b = roundl(y - a * X);
        
        a_s.push_back(a);
        b_s.push_back(b);
    }

    cout << "!";
    for (long long val : a_s) {
        cout << " " << val;
    }
    for (long long val : b_s) {
        cout << " " << val;
    }
    cout << endl;

    return 0;
}