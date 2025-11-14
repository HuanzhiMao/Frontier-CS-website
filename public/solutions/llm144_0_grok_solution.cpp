#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    int m = n / 2;
    int neg_max = m;
    int tests_per = neg_max + 1;
    int u = -1, v = -1;
    bool found = false;
    for (int center = 1; center <= n && !found; ++center) {
        vector<int> y_list;
        for (int i = 1; i <= n; ++i) {
            if (i != center) y_list.push_back(i);
        }
        bool found_for_this = false;
        for (int idx = 0; idx < tests_per; ++idx) {
            int y = y_list[idx];
            vector<int> sub;
            for (int i = 1; i <= n; ++i) {
                if (i != center && i != y) sub.push_back(i);
            }
            cout << "0 " << sub.size();
            for (int pos : sub) cout << " " << pos;
            cout << endl;
            cout.flush();
            int m1, m2;
            cin >> m1 >> m2;
            if (m1 == m && m2 == m + 1) {
                u = min(center, y);
                v = max(center, y);
                found_for_this = true;
                found = true;
                break;
            }
        }
    }
    vector<pair<int, int>> results(n + 1, make_pair(-1, -1));
    for (int x = 1; x <= n; ++x) {
        if (x == u || x == v) continue;
        // test with u
        vector<int> sub;
        for (int i = 1; i <= n; ++i) {
            if (i != x && i != u) sub.push_back(i);
        }
        cout << "0 " << sub.size();
        for (int pos : sub) cout << " " << pos;
        cout << endl;
        cout.flush();
        int m1, m2;
        cin >> m1 >> m2;
        results[x].first = (m1 == m && m2 == m + 1) ? 1 : 0;
        // test with v
        sub.clear();
        for (int i = 1; i <= n; ++i) {
            if (i != x && i != v) sub.push_back(i);
        }
        cout << "0 " << sub.size();
        for (int pos : sub) cout << " " << pos;
        cout << endl;
        cout.flush();
        cin >> m1 >> m2;
        results[x].second = (m1 == m && m2 == m + 1) ? 1 : 0;
    }
    vector<int> medians;
    for (int x = 1; x <= n; ++x) {
        if (x == u || x == v) continue;
        if (results[x].first == 0 && results[x].second == 0) {
            medians.push_back(x);
        }
    }
    sort(medians.begin(), medians.end());
    cout << "1 " << medians[0] << " " << medians[1] << endl;
    cout.flush();
    return 0;
}