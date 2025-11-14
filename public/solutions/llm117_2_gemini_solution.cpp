#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

using ll = long long;
using ld = long double;

int N;
const ld EPS = 1e-9;

map<pair<ll, ll>, ld> memo;

ld query(ld x, ld y) {
    ll ix = round(x * 1e6);
    ll iy = round(y * 1e6);
    if (memo.count({ix, iy})) {
        return memo[{ix, iy}];
    }
    cout << "? " << fixed << setprecision(10) << x << " " << y << endl;
    ld dist;
    cin >> dist;
    if (dist < 0) exit(0);
    return memo[{ix, iy}] = dist;
}

ld get_g(ld x, ld y) {
    return (query(x, y + EPS) - query(x, y - EPS)) / (2.0 * EPS);
}

void find_corners_recursive(ld x, ll y_min, ll y_max, ld g_min, ld g_max, map<ll, ld>& corners) {
    if (abs(g_min - g_max) < 1e-7) {
        return;
    }

    if (y_min == y_max) {
        corners[y_min] += (g_max - g_min);
        return;
    }

    ll y_mid = y_min + (y_max - y_min) / 2;
    ld g_mid = get_g(x, (ld)y_mid + 0.5);
    
    find_corners_recursive(x, y_min, y_mid, g_min, g_mid, corners);
    find_corners_recursive(x, y_mid + 1, y_max, g_mid, g_max, corners);
}

struct Line {
    ll a, b;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    
    map<ll, ld> b_jumps;
    ld g_b_start = get_g(0, -10000.5);
    ld g_b_end = get_g(0, 10000.5);
    find_corners_recursive(0, -10000, 10000, g_b_start, g_b_end, b_jumps);

    map<ll, ld> c_jumps;
    ld g_c_start = get_g(1, -20000.5);
    ld g_c_end = get_g(1, 20000.5);
    find_corners_recursive(1, -20000, 20000, g_c_start, g_c_end, c_jumps);
    
    map<ll, vector<ll>> jump_to_b;
    for(auto const& [b, jump] : b_jumps) {
        jump_to_b[round(jump * 1e7)].push_back(b);
    }
    
    map<ll, vector<ll>> jump_to_c;
    for(auto const& [c, jump] : c_jumps) {
        jump_to_c[round(jump * 1e7)].push_back(c);
    }

    vector<Line> result_lines;

    for(auto const& [jump_key, b_list] : jump_to_b) {
        auto it = jump_to_c.find(jump_key);
        if (it == jump_to_c.end()) continue;

        const auto& c_list = it->second;

        if (b_list.size() != c_list.size()) continue;
        
        ld jump_val = jump_key / 1e7;
        ld val = jump_val * jump_val;
        ll abs_a = round(sqrt(max((ld)0.0, 4.0 / val - 1.0)));

        vector<int> p(b_list.size());
        iota(p.begin(), p.end(), 0);

        function<bool(int)> find_perm = 
            [&](int k) {
            if (k == (int)b_list.size()) {
                for(size_t i=0; i<b_list.size(); ++i) {
                    ll b = b_list[i];
                    ll c = c_list[p[i]];
                    result_lines.push_back({c-b, b});
                }
                return true;
            }

            for(size_t i=k; i<b_list.size(); ++i) {
                swap(p[k], p[i]);
                ll b = b_list[k];
                ll c = c_list[p[k]];
                ll a = c - b;
                if (abs(a) == abs_a) {
                    if (find_perm(k+1)) return true;
                }
                swap(p[k], p[i]);
            }
            return false;
        };
        
        find_perm(0);
    }

    cout << "! ";
    for (int i = 0; i < N; ++i) cout << result_lines[i].a << (i == N - 1 ? "" : " ");
    cout << " ";
    for (int i = 0; i < N; ++i) cout << result_lines[i].b << (i == N - 1 ? "" : " ");
    cout << endl;

    return 0;
}