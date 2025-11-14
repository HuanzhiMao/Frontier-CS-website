#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

long long b;
int k;
int w;

long long L = 100000000;

vector<pair<long long, long long>> solutions;
vector<long long> u_list, v_list;
vector<int> u_indices_sorted_by_options;
map<long long, int> d2_counts;

bool solve(int u_sorted_idx, vector<bool>& v_used) {
    if (u_sorted_idx == u_list.size()) {
        return true;
    }
    
    int u_idx = u_indices_sorted_by_options[u_sorted_idx];
    long long u_val = u_list[u_idx];

    for (int j = 0; j < v_list.size(); ++j) {
        if (!v_used[j]) {
            long long v_val = v_list[j];

            // Parity must match for x and y to be integers
            if (((u_val % 2) + 2) % 2 != ((v_val % 2) + 2) % 2) continue;

            long long x_p_y = v_val;
            long long x_m_y = u_val;

            if ((x_p_y + x_m_y) % 2 != 0) continue; // Should be covered by parity check
            long long x = (x_p_y + x_m_y) / 2;

            if ((x_p_y - x_m_y) % 2 != 0) continue; // Should be covered by parity check
            long long y = (x_p_y - x_m_y) / 2;

            if (abs(x) > b || abs(y) > b) continue;

            long long d2_check = abs(x) + L - y;
            if (d2_counts.count(d2_check) && d2_counts[d2_check] > 0) {
                v_used[j] = true;
                d2_counts[d2_check]--;
                solutions.push_back({x, y});

                if (solve(u_sorted_idx + 1, v_used)) {
                    return true;
                }

                solutions.pop_back();
                d2_counts[d2_check]++;
                v_used[j] = false;
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> b >> k >> w;

    cout << "? 5 "
         << -L << " " << L << " "
         << 0 << " " << L << " "
         << L << " " << L << " "
         << -L << " " << -L << " "
         << L << " " << -L << endl;

    vector<long long> all_distances(5 * k);
    for(int i = 0; i < 5 * k; ++i) {
        cin >> all_distances[i];
    }

    vector<vector<long long>> D(5, vector<long long>(k));
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < k; ++j) {
            D[i][j] = all_distances[i*k + j];
        }
    }
    
    map<long long, int> initial_d2_counts;
    for (int j = 0; j < k; ++j) {
        initial_d2_counts[D[1][j]]++;
    }

    sort(D[0].begin(), D[0].end());
    sort(D[2].begin(), D[2].end());
    sort(D[3].begin(), D[3].end());
    sort(D[4].begin(), D[4].end());

    vector<long long> u_prime(k), v_prime(k);

    for (int i = 0; i < k; ++i) {
        long long d1 = D[0][i];
        long long d5 = D[4][k - 1 - i];
        u_prime[i] = (d1 - d5) / 2;
    }
    for (int i = 0; i < k; ++i) {
        long long d3 = D[2][i];
        long long d4 = D[3][k - 1 - i];
        v_prime[i] = (d4 - d3) / 2;
    }
    
    u_list = u_prime;
    v_list = v_prime;
    d2_counts = initial_d2_counts;
    
    vector<pair<int, int>> u_options;
    for (int i = 0; i < u_list.size(); ++i) {
        long long u_val = u_list[i];
        int options = 0;
        for (int j = 0; j < v_list.size(); ++j) {
            long long v_val = v_list[j];
            if (((u_val % 2) + 2) % 2 != ((v_val % 2) + 2) % 2) continue;
            long long x = (v_val + u_val) / 2;
            long long y = (v_val - u_val) / 2;
            if (abs(x) <= b && abs(y) <= b) {
                options++;
            }
        }
        u_options.push_back({options, i});
    }
    sort(u_options.begin(), u_options.end());
    u_indices_sorted_by_options.resize(k);
    for(int i=0; i < k; ++i) u_indices_sorted_by_options[i] = u_options[i].second;

    vector<bool> v_used(k, false);
    solve(0, v_used);

    cout << "!";
    for (const auto& p : solutions) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;

    return 0;
}