#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <map>
#include <algorithm>

using namespace std;

struct State {
    int h = 0, d = 0;

    bool operator<(const State& other) const {
        if (h != other.h) return h < other.h;
        return d < other.d;
    }
};

int n;

void split(map<int, pair<int, State>>& intervals, int p) {
    if (p > n) return;
    auto it = intervals.upper_bound(p);
    if (it == intervals.begin()) return;
    it--;
    if (it->first < p && p <= it->second.first) {
        int end = it->second.first;
        State s = it->second.second;
        it->second.first = p - 1;
        intervals[p] = {end, s};
    }
}

void solve() {
    cin >> n;

    map<int, pair<int, State>> intervals;
    intervals[1] = {n, {0, 0}};

    while (true) {
        long long total_size = 0;
        for (const auto& p : intervals) {
            total_size += (long long)p.second.first - p.first + 1;
        }

        if (total_size <= 2) {
            break;
        }

        int q_l = -1, q_r = -1;

        bool forced_found = false;
        for (const auto& p : intervals) {
            if (p.second.second.h == 2 || p.second.second.d == 2) {
                q_l = p.first;
                q_r = p.first + (p.second.first - p.first) / 2;
                forced_found = true;
                break;
            }
        }

        if (!forced_found) {
            long long max_len = 0;
            int best_l = -1;
            for (const auto& p : intervals) {
                if ((long long)p.second.first - p.first + 1 > max_len) {
                    max_len = (long long)p.second.first - p.first + 1;
                    best_l = p.first;
                }
            }
            q_l = best_l;
            q_r = best_l + (max_len - 1) / 2;
        }
        
        cout << "? " << q_l << " " << q_r << endl;
        int x;
        cin >> x;

        map<State, vector<pair<int, int>>> next_state_intervals;
        
        split(intervals, q_l);
        split(intervals, q_r + 1);

        for (const auto& p : intervals) {
            int start = p.first;
            int end = p.second.first;
            State current_state = p.second.second;

            bool is_in = (start >= q_l && end <= q_r);
            int len = q_r - q_l + 1;

            // Honest path
            if (current_state.h < 2) {
                bool consistent = false;
                if (is_in && x == len - 1) consistent = true;
                if (!is_in && x == len) consistent = true;
                if (consistent) {
                    State next_s = {current_state.h + 1, 0};
                    next_state_intervals[next_s].push_back({start, end});
                }
            }

            // Dishonest path
            if (current_state.d < 2) {
                bool consistent = false;
                if (is_in && x == len) consistent = true;
                if (!is_in && x == len - 1) consistent = true;
                if (consistent) {
                    State next_s = {0, current_state.d + 1};
                    next_state_intervals[next_s].push_back({start, end});
                }
            }
        }
        
        intervals.clear();
        for(auto const& [s, v_orig] : next_state_intervals) {
            if (v_orig.empty()) continue;
            auto v = v_orig;
            sort(v.begin(), v.end());
            vector<pair<int, int>> merged;
            merged.push_back(v[0]);
            for(size_t i = 1; i < v.size(); ++i) {
                if (v[i].first == merged.back().second + 1) {
                    merged.back().second = v[i].second;
                } else {
                    merged.push_back(v[i]);
                }
            }
            for(const auto& p : merged) {
                intervals[p.first] = {p.second, s};
            }
        }
    }

    vector<int> candidates;
    for (const auto& p : intervals) {
        for (int i = p.first; i <= p.second.first; ++i) {
            candidates.push_back(i);
        }
    }

    for (int cand : candidates) {
        cout << "! " << cand << endl;
        int res;
        cin >> res;
        if (res == 1) {
            break;
        }
    }
    
    cout << "#" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}