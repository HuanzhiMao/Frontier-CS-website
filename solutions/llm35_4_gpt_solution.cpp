#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    for (int tc = 0; tc < t; ++tc) {
        int n;
        if (!(cin >> n)) return 0;
        if (n == -1) return 0;

        int L = 2 * n - 1; // number of positions
        const int m = 12;  // number of subsets
        const int r = 6;   // weight of each index code

        // Generate first L combinations of size r from [0..m-1]
        vector<vector<int>> combs;
        combs.reserve(L);
        vector<int> curr;
        function<void(int,int)> dfs = [&](int start, int left) {
            if ((int)combs.size() >= L) return;
            if (left == 0) {
                combs.push_back(curr);
                return;
            }
            for (int i = start; i <= m - left; ++i) {
                curr.push_back(i);
                dfs(i + 1, left - 1);
                curr.pop_back();
                if ((int)combs.size() >= L) return;
            }
        };
        dfs(0, r);

        // Build S_k sets: indices whose code contains bit k (1-indexed indices)
        vector<vector<int>> S(m);
        for (int idx = 1; idx <= L; ++idx) {
            for (int b : combs[idx - 1]) {
                S[b].push_back(idx);
            }
        }

        // Precompute strings for each S_k: "<size> i1 i2 ... "
        vector<string> Sstr(m);
        for (int k = 0; k < m; ++k) {
            string s = to_string((int)S[k].size());
            for (int id : S[k]) {
                s.push_back(' ');
                s += to_string(id);
            }
            Sstr[k] = move(s);
        }

        int answerX = -1;
        for (int x = 1; x <= n; ++x) {
            int ones = 0;
            int processed = 0;
            for (int k = 0; k < m; ++k) {
                cout << "? " << x << " " << Sstr[k] << '\n';
                cout.flush();
                int resp;
                if (!(cin >> resp)) return 0;
                if (resp == -1) return 0;
                if (resp == 1) ++ones;
                ++processed;
                int remaining = m - processed;
                if (ones > r) break;
                if (ones + remaining < r) break;
            }
            if (ones == r && processed == m) {
                answerX = x;
                break;
            }
        }

        if (answerX == -1) answerX = 1; // Fallback (should not happen)

        cout << "! " << answerX << '\n';
        cout.flush();
    }
    return 0;
}