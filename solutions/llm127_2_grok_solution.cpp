#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    vector<pair<int, int>> intervals = {{0, n - 1}};
    while (!intervals.empty()) {
        int max_len = -1;
        int chosen = -1;
        for (int j = 0; j < intervals.size(); ++j) {
            int len = intervals[j].second - intervals[j].first + 1;
            if (len > max_len) {
                max_len = len;
                chosen = j;
            }
        }
        if (max_len <= 0) break;
        auto [s, e] = intervals[chosen];
        int i = s + (e - s) / 2;
        cout << "? " << i << endl;
        cout.flush();
        int a0, a1;
        cin >> a0 >> a1;
        if (a0 == 0 && a1 == 0) {
            cout << "! " << i << endl;
            cout.flush();
            return 0;
        }
        vector<pair<int, int>> new_int;
        for (int j = 0; j < intervals.size(); ++j) {
            if (j == chosen) {
                int l1 = s, r1 = i - 1;
                if (l1 <= r1) new_int.emplace_back(l1, r1);
                int l2 = i + 1, r2 = e;
                if (l2 <= r2) new_int.emplace_back(l2, r2);
            } else {
                new_int.push_back(intervals[j]);
            }
        }
        vector<pair<int, int>> final_new;
        bool discard_left = (a0 == 0);
        bool discard_right = (a1 == 0);
        for (auto [cs, ce] : new_int) {
            int ns = cs, ne = ce;
            if (discard_left) {
                ns = max(ns, i);
            }
            if (discard_right) {
                ne = min(ne, i);
            }
            if (ns <= ne) {
                final_new.emplace_back(ns, ne);
            }
        }
        intervals = final_new;
    }
    cout << "! 0" << endl;
    cout.flush();
    return 0;
}