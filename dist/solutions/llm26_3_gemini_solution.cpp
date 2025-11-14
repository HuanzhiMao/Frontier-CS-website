#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

using namespace std;

const long long INF = -1e18;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    
    // dp[i] will store pairs of {length, max_sum} for IS ending at v[i]
    vector<vector<pair<int, long long>>> dp(n);
    // max_s[k] = max sum for any IS of length k
    vector<long long> max_s(n + 1, INF);

    for(int i = 0; i < n; ++i) {
        vector<pair<int, long long>> new_pairs;
        new_pairs.push_back({1, (long long)v[i]});

        for(int j = 0; j < i; ++j) {
            if(v[j] < v[i]) {
                for(auto const& [len, sum] : dp[j]) {
                    long long new_sum = sum + v[i];
                    int new_len = len + 1;
                    
                    bool found = false;
                    for(auto& p : new_pairs) {
                        if (p.first == new_len) {
                            p.second = max(p.second, new_sum);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        new_pairs.push_back({new_len, new_sum});
                    }
                }
            }
        }
        dp[i] = new_pairs;
        for(auto const& [len, sum] : dp[i]) {
            if (len <= n) {
                max_s[len] = max(max_s[len], sum);
            }
        }
    }

    long long min_final_cost = -1;
    int best_m = 0;

    for (int m = 1; m <= n; ++m) {
        if (max_s[m] < 0) continue;

        long long s_m = max_s[m];
        long long moves_count = n - m;
        long long total_cost = (long long)m * (n - m) + (long long)m * (m + 1) / 2 - s_m + moves_count;
        
        long long final_cost = (total_cost + 1) * (moves_count + 1);

        if (min_final_cost == -1 || final_cost < min_final_cost) {
            min_final_cost = final_cost;
            best_m = m;
        }
    }
    
    // Case: move all elements
    {
        long long s_total = (long long)n * (n + 1) / 2;
        long long moves_count = n;
        long long total_cost = s_total;
        long long final_cost = (total_cost + 1) * (moves_count + 1);
        if (min_final_cost == -1 || final_cost < min_final_cost) {
            min_final_cost = final_cost;
            best_m = 0;
        }
    }
    
    // Case: move no elements (already sorted)
    bool sorted = true;
    for(int i = 0; i < n; ++i) {
        if(v[i] != i + 1) {
            sorted = false;
            break;
        }
    }
    if(sorted) {
        min_final_cost = 1;
        best_m = n;
    }


    if(best_m == n) {
        cout << "1 0\n";
        return 0;
    }
    
    vector<int> K;
    if (best_m > 0) {
        long long S_m = max_s[best_m];
        int last_val = -1, last_idx = -1;

        for (int i = n - 1; i >= 0; --i) {
            for (auto const& [len, sum] : dp[i]) {
                if (len == best_m && sum == S_m) {
                    last_val = v[i];
                    last_idx = i;
                    break;
                }
            }
            if (last_val != -1) break;
        }
        
        K.push_back(last_val);
        int current_m = best_m - 1;
        long long current_sum = S_m - last_val;

        for (int i = last_idx - 1; i >= 0 && current_m > 0; --i) {
            if (v[i] < last_val) {
                for(auto const& [len, sum] : dp[i]) {
                    if (len == current_m && sum == current_sum) {
                        K.push_back(v[i]);
                        last_val = v[i];
                        current_m--;
                        current_sum -= v[i];
                        break;
                    }
                }
            }
        }
        reverse(K.begin(), K.end());
    }

    vector<bool> in_K(n + 1, false);
    for (int val : K) {
        in_K[val] = true;
    }

    vector<int> M;
    for (int i = 1; i <= n; ++i) {
        if (!in_K[i]) {
            M.push_back(i);
        }
    }
    sort(M.rbegin(), M.rend());

    vector<pair<int, int>> moves_to_print;
    vector<int> current_v = v;

    for (int m_val : M) {
        int target_pos_1based = 1;
        for (int k_val : K) {
            if (k_val < m_val) {
                target_pos_1based++;
            }
        }
        
        int current_pos_1based = -1;
        for (int i = 0; i < current_v.size(); ++i) {
            if (current_v[i] == m_val) {
                current_pos_1based = i + 1;
                break;
            }
        }

        moves_to_print.push_back({current_pos_1based, target_pos_1based});
        
        int elem = current_v[current_pos_1based - 1];
        current_v.erase(current_v.begin() + current_pos_1based - 1);
        current_v.insert(current_v.begin() + target_pos_1based - 1, elem);
    }
    
    cout << min_final_cost << " " << moves_to_print.size() << "\n";
    for (const auto& move : moves_to_print) {
        cout << move.first << " " << move.second << "\n";
    }

    return 0;
}