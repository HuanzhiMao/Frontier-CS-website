#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<bool> poss(n + 1, false);
    vector<int> last_lie(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        poss[i] = true;
    }
    int queries_used = 0;
    int guesses_used = 0;
    while (true) {
        vector<int> curr_poss;
        for (int i = 1; i <= n; ++i) {
            if (poss[i]) {
                curr_poss.push_back(i);
            }
        }
        int m = curr_poss.size();
        if (m == 0) {
            break;
        }
        if (m <= 2) {
            for (auto g : curr_poss) {
                cout << "! " << g << endl;
                cout.flush();
                string res;
                cin >> res;
                if (res == ":)") {
                    return 0;
                }
                poss[g] = false;
                ++guesses_used;
            }
            continue;
        }
        vector<int> groupA, groupB;
        for (auto y : curr_poss) {
            if (last_lie[y] == 0) {
                groupA.push_back(y);
            } else {
                groupB.push_back(y);
            }
        }
        int aa = groupA.size();
        int bb = groupB.size();
        int best_max = INT_MAX;
        int best_b1 = 0;
        for (int b1 = 0; b1 <= bb; ++b1) {
            int r1 = aa + b1;
            int r2 = aa + bb - b1;
            int mx = max(r1, r2);
            if (mx < best_max) {
                best_max = mx;
                best_b1 = b1;
            }
        }
        int best_a1 = aa / 2;
        vector<int> S;
        for (int i = 0; i < best_a1 && i < static_cast<int>(groupA.size()); ++i) {
            S.push_back(groupA[i]);
        }
        for (int i = 0; i < best_b1 && i < static_cast<int>(groupB.size()); ++i) {
            S.push_back(groupB[i]);
        }
        if (S.empty()) {
            if (!groupA.empty()) {
                S.push_back(groupA[0]);
            } else if (!groupB.empty()) {
                S.push_back(groupB[0]);
            }
        }
        cout << "? " << S.size();
        for (auto s : S) {
            cout << " " << s;
        }
        cout << endl;
        cout.flush();
        string ans;
        cin >> ans;
        ++queries_used;
        vector<bool> is_in_s(n + 1, false);
        for (auto s : S) {
            is_in_s[s] = true;
        }
        for (auto y : curr_poss) {
            bool in_s = is_in_s[y];
            string true_ans = in_s ? "YES" : "NO";
            bool lie = (true_ans != ans);
            bool eliminate = lie && (last_lie[y] == 1);
            if (eliminate) {
                poss[y] = false;
            } else {
                last_lie[y] = lie ? 1 : 0;
            }
        }
    }
    return 0;
}