#include <bits/stdc++.h>
using namespace std;

vector<pair<bool, int>> get_trans(int s) {
    vector<pair<bool, int>> res;
    if (s == 4) {
        res.push_back({false, 0});
        res.push_back({true, 2});
    } else if (s == 0) {
        res.push_back({false, 1});
        res.push_back({true, 2});
    } else if (s == 1) {
        res.push_back({true, 2});
    } else if (s == 2) {
        res.push_back({false, 0});
        res.push_back({true, 3});
    } else if (s == 3) {
        res.push_back({false, 0});
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    for (int test = 0; test < t; test++) {
        int n;
        cin >> n;
        vector<vector<int>> possible(5);
        possible[4].reserve(n);
        for (int i = 1; i <= n; i++) {
            possible[4].push_back(i);
        }
        vector<int> stamp(n + 2, 0);
        int current_stamp = 0;
        bool done = false;
        while (!done) {
            current_stamp++;
            int num_unique = 0;
            for (int s = 0; s < 4; s++) {
                for (int kk : possible[s]) {
                    if (stamp[kk] != current_stamp) {
                        stamp[kk] = current_stamp;
                        num_unique++;
                    }
                }
            }
            if (num_unique <= 2) {
                set<int> sus;
                for (int s = 0; s < 4; s++) {
                    for (int kk : possible[s]) {
                        sus.insert(kk);
                    }
                }
                vector<int> suspects(sus.begin(), sus.end());
                int nums = suspects.size();
                if (nums == 1) {
                    cout << "! " << suspects[0] << endl;
                    int y;
                    cin >> y;
                } else if (nums == 2) {
                    cout << "! " << suspects[0] << endl;
                    int y;
                    cin >> y;
                    if (y == 0) {
                        cout << "! " << suspects[1] << endl;
                        int yy;
                        cin >> yy;
                    }
                }
                cout << "#" << endl;
                done = true;
                continue;
            }
            vector<int> all_k;
            for (int s = 0; s < 4; s++) {
                all_k.insert(all_k.end(), possible[s].begin(), possible[s].end());
            }
            sort(all_k.begin(), all_k.end());
            size_t sz = all_k.size();
            size_t mid_idx = sz / 2;
            int l = all_k[0];
            int r = all_k[mid_idx];
            cout << "? " << l << " " << r << endl;
            cout.flush();
            int x;
            cin >> x;
            int ss = r - l + 1;
            bool received_rep = (x == ss - 1);
            vector<vector<int>> new_possible(4);
            vector<vector<char>> added(4, vector<char>(n + 2, 0));
            for (int old_s = 0; old_s < 5; old_s++) {
                if (possible[old_s].empty()) continue;
                auto trans = get_trans(old_s);
                for (auto [is_D, new_s] : trans) {
                    for (int k : possible[old_s]) {
                        bool true_m = (l <= k && k <= r);
                        bool exp_rep = true_m ^ is_D;
                        if (exp_rep == received_rep) {
                            if (added[new_s][k] == 0) {
                                added[new_s][k] = 1;
                                new_possible[new_s].push_back(k);
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < 4; i++) {
                possible[i] = std::move(new_possible[i]);
            }
            possible[4].clear();
        }
    }
    return 0;
}