#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <map>
#include <queue>
#include <tuple>
#include <algorithm>
#include <numeric>

using namespace std;

int N;
vector<pair<long long, long long>> result_lines;
const long double EPS = 1e-9;

long double query(long double x, long double y) {
    cout << "? " << fixed << setprecision(10) << x << " " << y << endl;
    long double dist;
    cin >> dist;
    return dist;
}

// Global cache for slopes to reuse across different calls if X is the same.
map<long long, map<long long, long double>> slope_caches;

long double get_slope(long long X, long long y) {
    if (slope_caches.count(X) && slope_caches[X].count(y)) {
        return slope_caches[X][y];
    }
    long double y_coord = y;
    // Use a small epsilon to approximate derivative at y
    long double eps = 0.25L;
    long double d1 = query((long double)X, y_coord - eps);
    long double d2 = query((long double)X, y_coord + eps);
    return slope_caches[X][y] = (d2 - d1) / (2.0L * eps);
}

map<long long, int> find_all_kinks(long long X) {
    map<long long, int> kink_counts;
    
    long long y_min, y_max;
    if (X == 1 || X == -1) {
        y_min = -20001; y_max = 20001;
    } else { // X == 2
        y_min = -30001; y_max = 30001;
    }

    queue<tuple<long long, long long, long double, long double>> q;
    
    long double slope_min = get_slope(X, y_min);
    long double slope_max = get_slope(X, y_max);

    if (abs(slope_min - slope_max) > EPS) {
        q.emplace(y_min, y_max, slope_min, slope_max);
    }

    int kinks_found = 0;
    while (!q.empty() && kinks_found < N) {
        auto [L, R, slope_L, slope_R] = q.front();
        q.pop();

        if (L >= R) continue;
        
        if (L + 1 == R) {
            // A kink is present between L and R (which are consecutive integers)
            // The kink must be at an integer coordinate. The slope changes at L.
            kink_counts[L]++;
            kinks_found++;
            continue;
        }
        
        long long M = L + (R - L) / 2;
        long double slope_M = get_slope(X, M);
        
        if (abs(slope_L - slope_M) > EPS) {
            q.emplace(L, M, slope_L, slope_M);
        }
        if (abs(slope_M - slope_R) > EPS) {
            q.emplace(M, R, slope_M, slope_R);
        }
    }
    
    return kink_counts;
}

bool solve_matching(
    map<long long, int>& freq1,
    map<long long, int>& freq2,
    map<long long, int>& freq3) {

    if (result_lines.size() == N) {
        return true;
    }

    auto it1 = freq1.begin();
    while (it1 != freq1.end() && it1->second == 0) {
        ++it1;
    }
    if (it1 == freq1.end()) {
        return result_lines.size() == N;
    }
    long long s1 = it1->first;
    
    freq1[s1]--;

    for (auto const& [s2_key, s2_val] : freq2) {
        if (s2_val > 0) {
            long long s2 = s2_key;
            if ((s1 % 2 + 2) % 2 != (s2 % 2 + 2) % 2) continue;

            long long a = (s1 - s2) / 2;
            long long b = (s1 + s2) / 2;
            
            if (abs(a) > 10000 || abs(b) > 10000) continue;

            long long s3 = 2 * a + b;

            if (freq3.count(s3) && freq3[s3] > 0) {
                freq2[s2]--;
                freq3[s3]--;
                result_lines.push_back({a, b});

                if (solve_matching(freq1, freq2, freq3)) {
                    return true;
                }

                result_lines.pop_back();
                freq3[s3]++;
                freq2[s2]++;
            }
        }
    }
    freq1[s1]++;
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    
    map<long long, int> kinks1 = find_all_kinks(1);
    map<long long, int> kinks2 = find_all_kinks(-1);
    map<long long, int> kinks3 = find_all_kinks(2);
    
    solve_matching(kinks1, kinks2, kinks3);

    cout << "!" << flush;
    for (int i = 0; i < N; ++i) {
        cout << " " << result_lines[i].first;
    }
    for (int i = 0; i < N; ++i) {
        cout << " " << result_lines[i].second;
    }
    cout << endl;

    return 0;
}