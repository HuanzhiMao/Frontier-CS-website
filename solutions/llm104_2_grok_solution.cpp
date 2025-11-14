#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100001;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    for (int test = 0; test < t; ++test) {
        int n;
        cin >> n;
        vector<vector<int>> poss(5);
        poss[0].resize(n);
        for (int i = 0; i < n; ++i) {
            poss[0][i] = i + 1;
        }
        while (true) {
            bitset<MAXN> cand;
            cand.reset();
            for (int s = 0; s < 5; ++s) {
                for (int kk : poss[s]) {
                    cand[kk] = true;
                }
            }
            int num_cand = cand.count();
            if (num_cand <= 2) {
                vector<int> suspects;
                for (int i = 1; i <= n; ++i) {
                    if (cand[i]) {
                        suspects.push_back(i);
                    }
                }
                for (size_t ii = 0; ii < suspects.size(); ++ii) {
                    int a = suspects[ii];
                    cout << "! " << a << endl;
                    cout.flush();
                    int y;
                    cin >> y;
                    if (y == 1) break;
                }
                cout << "#" << endl;
                cout.flush();
                break;
            }
            vector<int> curr_possible;
            for (int i = 1; i <= n; ++i) {
                if (cand[i]) {
                    curr_possible.push_back(i);
                }
            }
            int sz = curr_possible.size();
            int idx = sz / 2;
            int mid = curr_possible[idx];
            int l = 1;
            int r = mid;
            cout << "? " << l << " " << r << endl;
            cout.flush();
            int x;
            cin >> x;
            vector<vector<int>> new_poss(5);
            for (int s = 0; s < 5; ++s) {
                for (int kk : poss[s]) {
                    bool in_range = (l <= kk && kk <= r);
                    int m = r - l + 1;
                    int tp = m - (in_range ? 1 : 0);
                    for (int c = 0; c < 2; ++c) {
                        bool allowed = false;
                        if (s == 0) allowed = true;
                        else if (s == 1) allowed = true;
                        else if (s == 2) allowed = (c == 1);
                        else if (s == 3) allowed = true;
                        else if (s == 4) allowed = (c == 0);
                        if (allowed) {
                            int this_x = tp;
                            if (c == 1) {
                                this_x += in_range ? 1 : -1;
                            }
                            if (this_x == x) {
                                int news = -1;
                                if (s == 0) {
                                    news = (c == 0 ? 1 : 3);
                                } else if (s == 1) {
                                    news = (c == 0 ? 2 : 3);
                                } else if (s == 2) {
                                    news = 3;
                                } else if (s == 3) {
                                    news = (c == 0 ? 1 : 4);
                                } else if (s == 4) {
                                    news = 1;
                                }
                                new_poss[news].push_back(kk);
                            }
                        }
                    }
                }
            }
            poss = move(new_poss);
        }
    }
    return 0;
}