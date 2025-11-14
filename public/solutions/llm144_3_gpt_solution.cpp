#include <bits/stdc++.h>
using namespace std;

bool is_perm(const vector<long long>& arr, long long n) {
    if ((long long)arr.size() != n) return false;
    vector<int> seen(n + 1, 0);
    for (long long x : arr) {
        if (x < 1 || x > n) return false;
        if (++seen[(size_t)x] > 1) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> all;
    long long x;
    while (cin >> x) all.push_back(x);
    if (all.empty()) return 0;

    auto solve_one = [](long long n, const vector<long long>& p) -> pair<int,int> {
        long long a = n/2, b = n/2 + 1;
        int i1 = -1, i2 = -1;
        for (int i = 0; i < (int)p.size(); ++i) {
            if (p[i] == a) i1 = i + 1;
            if (p[i] == b) i2 = i + 1;
        }
        return {i1, i2};
    };

    // Try single test interpretation: n followed by permutation of size n
    if (all.size() >= 2) {
        long long n = all[0];
        if (n >= 1 && (long long)all.size() >= 1 + n) {
            vector<long long> p(all.begin() + 1, all.begin() + 1 + n);
            if (is_perm(p, n)) {
                auto ans = solve_one(n, p);
                cout << ans.first << " " << ans.second << "\n";
                return 0;
            }
        }
    }

    // Try multiple test cases: T, then for each test: n and permutation of size n
    {
        size_t pos = 0;
        long long T = all[pos++];
        bool ok = (T >= 1);
        vector<pair<int,int>> answers;
        for (long long t = 0; ok && t < T; ++t) {
            if (pos >= all.size()) { ok = false; break; }
            long long n = all[pos++];
            if (n < 1 || pos + (size_t)n > all.size()) { ok = false; break; }
            vector<long long> p(all.begin() + pos, all.begin() + pos + n);
            pos += n;
            if (!is_perm(p, n)) { ok = false; break; }
            answers.push_back(solve_one(n, p));
        }
        if (ok && (int)answers.size() == T) {
            for (auto &ans : answers) {
                cout << ans.first << " " << ans.second << "\n";
            }
            return 0;
        }
    }

    // Fallback: cannot interpret input format (likely interactive), do nothing.
    return 0;
}