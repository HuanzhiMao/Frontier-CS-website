#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <array>

using namespace std;

long long b;
int k;
int w;

const long long C = 100000000;

struct Candidate {
    long long x, y;
    array<long long, 4> dists;
};

vector<Candidate> unique_candidates;
vector<pair<long long, long long>> solution;
map<long long, int> f1, f2, f3, f4;

bool solve(int cand_idx, int k_rem) {
    if (k_rem == 0) {
        return true;
    }
    if (cand_idx == unique_candidates.size()) {
        return false;
    }

    const auto& cand = unique_candidates[cand_idx];
    long long d1 = cand.dists[0];
    long long d2 = cand.dists[1];
    long long d3 = cand.dists[2];
    long long d4 = cand.dists[3];

    int max_take = k_rem;
    if (f1.count(d1)) max_take = min(max_take, f1[d1]); else max_take = 0;
    if (f2.count(d2)) max_take = min(max_take, f2[d2]); else max_take = 0;
    if (f3.count(d3)) max_take = min(max_take, f3[d3]); else max_take = 0;
    if (f4.count(d4)) max_take = min(max_take, f4[d4]); else max_take = 0;
    
    for (int i = max_take; i >= 0; --i) {
        if (i > 0) {
            f1[d1] -= i;
            f2[d2] -= i;
            f3[d3] -= i;
            f4[d4] -= i;
        }

        if (solve(cand_idx + 1, k_rem - i)) {
            for (int j = 0; j < i; ++j) {
                solution.push_back({cand.x, cand.y});
            }
            return true;
        }

        if (i > 0) {
            f1[d1] += i;
            f2[d2] += i;
            f3[d3] += i;
            f4[d4] += i;
        }
    }

    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> b >> k >> w;

    vector<long long> D1(k), D2(k), D3(k), D4(k);

    cout << "? 1 0 " << C << endl;
    for (int i = 0; i < k; ++i) cin >> D1[i];
    
    cout << "? 1 0 " << -C << endl;
    for (int i = 0; i < k; ++i) cin >> D2[i];
    
    cout << "? 1 " << C << " 0" << endl;
    for (int i = 0; i < k; ++i) cin >> D3[i];

    cout << "? 1 " << -C << " 0" << endl;
    for (int i = 0; i < k; ++i) cin >> D4[i];

    for (long long d : D1) f1[d]++;
    for (long long d : D2) f2[d]++;
    for (long long d : D3) f3[d]++;
    for (long long d : D4) f4[d]++;

    vector<long long> uD1, uD2, uD3, uD4;
    for(auto const& [val, count] : f1) uD1.push_back(val);
    for(auto const& [val, count] : f2) uD2.push_back(val);
    for(auto const& [val, count] : f3) uD3.push_back(val);
    for(auto const& [val, count] : f4) uD4.push_back(val);

    map<pair<long long, long long>, array<long long, 4>> candidates_map;

    for (long long d1 : uD1) {
        for (long long d2 : uD2) {
            if ((d2 - d1) % 2 != 0) continue;
            long long y = (d2 - d1) / 2;
            if (abs(y) > b) continue;
            
            long long sum_d1d2_2C = d1 + d2 - 2 * C;
            if (sum_d1d2_2C < 0 || sum_d1d2_2C % 2 != 0) continue;
            
            for (long long d3 : uD3) {
                for (long long d4 : uD4) {
                    if ((d4 - d3) % 2 != 0) continue;
                    long long x = (d4 - d3) / 2;
                    if (abs(x) > b) continue;
                    
                    long long sum_d3d4_2C = d3 + d4 - 2 * C;
                    if (sum_d3d4_2C < 0 || sum_d3d4_2C % 2 != 0) continue;

                    if (2 * abs(x) == sum_d1d2_2C && 2 * abs(y) == sum_d3d4_2C) {
                        candidates_map[{x, y}] = {d1, d2, d3, d4};
                    }
                }
            }
        }
    }
    
    for(auto const& [pos, dists] : candidates_map) {
        unique_candidates.push_back({pos.first, pos.second, dists});
    }

    solve(0, k);

    cout << "! ";
    for (int i = 0; i < k; ++i) {
        cout << solution[i].first << " " << solution[i].second << (i == k - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}