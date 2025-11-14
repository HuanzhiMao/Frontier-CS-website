#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <tuple>
#include <map>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

struct Square {
    double x, y, a;
};

map<int, pair<double, vector<Square>>> memo;

pair<double, vector<Square>> baseline(int num_squares) {
    if (num_squares == 0) return {0.0, {}};
    long long m = ceil(sqrt(static_cast<double>(num_squares)));
    double l = m;
    vector<Square> squares;
    for (int i = 0; i < num_squares; ++i) {
        long long row = i / m;
        long long col = i % m;
        squares.push_back({col + 0.5, row + 0.5, 0.0});
    }
    return {l, squares};
}

pair<double, vector<Square>> solve(int n) {
    if (memo.count(n)) {
        return memo[n];
    }

    if (n == 0) return {0.0, {}};
    if (n == 1) return {1.0, {{0.5, 0.5, 0.0}}};
    
    int k = (n + 3) / 4;
    auto [Lk, Sk] = solve(k);
    
    double L = 2.0 * Lk;
    vector<Square> Sn;
    
    vector<pair<double, double>> offsets = {{0, 0}, {0, Lk}, {Lk, 0}, {Lk, Lk}};
    
    int count = 0;
    for (int i = 0; i < 4 && count < n; ++i) {
        double dx = offsets[i].first;
        double dy = offsets[i].second;
        for (const auto& s : Sk) {
            if (count < n) {
                Sn.push_back({s.x + dx, s.y + dy, s.a});
                count++;
            } else {
                break;
            }
        }
    }
    
    pair<double, vector<Square>> result = {L, Sn};

    if (n <= 100) {
        auto base_res = baseline(n);
        if (base_res.first < result.first) {
            result = base_res;
        }
    }
    
    return memo[n] = result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    
    pair<double, vector<Square>> result = solve(n);

    cout << fixed << setprecision(10) << result.first << endl;
    for (const auto& s : result.second) {
        cout << fixed << setprecision(10) << s.x << " " << s.y << " " << s.a << endl;
    }

    return 0;
}