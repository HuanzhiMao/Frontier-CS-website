#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <tuple>
#include <algorithm>

using namespace std;

struct Square {
    double x, y, a;
};

map<int, pair<double, vector<Square>>> memo;

pair<double, vector<Square>> solve(int n) {
    if (memo.count(n)) {
        return memo[n];
    }

    if (n == 0) {
        return memo[n] = {0.0, {}};
    }
    if (n == 1) {
        return memo[n] = {1.0, {{0.5, 0.5, 0.0}}};
    }

    // Baseline solution
    double L_base = ceil(sqrt((double)n));
    
    // Recursive solution
    int k_rec = (n + 3) / 4;
    auto [L_k, pos_k] = solve(k_rec);
    double L_rec = 2.0 * L_k;
    
    if (L_rec < L_base - 1e-9) {
        vector<Square> pos_rec;
        
        int q = n / 4;
        int r = n % 4;
        
        vector<int> counts(4, q);
        for(int i = 0; i < r; ++i) {
            counts[i]++;
        }

        vector<pair<double, double>> offsets = {{0,0}, {0, L_k}, {L_k, 0}, {L_k, L_k}};
        
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < counts[i]; ++j) {
                pos_rec.push_back({pos_k[j].x + offsets[i].first, pos_k[j].y + offsets[i].second, pos_k[j].a});
            }
        }

        return memo[n] = {L_rec, pos_rec};
    } else {
        vector<Square> pos_base;
        int k_base = static_cast<int>(L_base);
        for (int i = 0; i < n; ++i) {
            pos_base.push_back({0.5 + i % k_base, 0.5 + i / k_base, 0.0});
        }
        return memo[n] = {L_base, pos_base};
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    auto result = solve(n);
    
    cout << fixed << setprecision(10) << result.first << endl;
    for (const auto& s : result.second) {
        cout << fixed << setprecision(10) << s.x << " " << s.y << " " << s.a << endl;
    }

    return 0;
}