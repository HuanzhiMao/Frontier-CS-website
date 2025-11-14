#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<long long> tokens;
    string tok;
    while (cin >> tok) {
        bool neg = false;
        int i = 0;
        if (!tok.empty() && (tok[0] == '-' || tok[0] == '+')) {
            neg = (tok[0] == '-');
            i = 1;
        }
        bool numeric = (i < (int)tok.size());
        long long val = 0;
        for (; i < (int)tok.size(); ++i) {
            if (!isdigit((unsigned char)tok[i])) { numeric = false; break; }
            val = val * 10 + (tok[i] - '0');
        }
        if (numeric) {
            if (neg) val = -val;
            tokens.push_back(val);
        }
    }

    int n = N;
    int M = tokens.size();
    int total = 2 * n;
    vector<pair<int,int>> answer;
    auto try_involution = [&](int s)->bool {
        if (s < 0 || s + total > M) return false;
        vector<int> v(total + 1, 0);
        for (int i = 1; i <= total; ++i) {
            long long t = tokens[s + i - 1];
            if (!(1 <= t && t <= total)) return false;
            v[i] = (int)t;
        }
        for (int i = 1; i <= total; ++i) {
            int t = v[i];
            if (t == i) return false;
            if (t < 1 || t > total) return false;
        }
        for (int i = 1; i <= total; ++i) {
            if (v[v[i]] != i) return false;
        }
        vector<char> vis(total + 1, 0);
        vector<pair<int,int>> ans;
        ans.reserve(n);
        for (int i = 1; i <= total; ++i) {
            if (!vis[i]) {
                int j = v[i];
                if (vis[j]) return false;
                vis[i] = vis[j] = 1;
                if (i < j) ans.emplace_back(i, j);
            }
        }
        if ((int)ans.size() != n) return false;
        answer = move(ans);
        return true;
    };
    auto try_colors = [&](int s)->bool {
        if (s < 0 || s + total > M) return false;
        unordered_map<long long, vector<int>> pos;
        pos.reserve(total * 2);
        for (int i = 1; i <= total; ++i) {
            long long t = tokens[s + i - 1];
            pos[t].push_back(i);
        }
        if ((int)pos.size() != n) return false;
        vector<pair<int,int>> ans;
        ans.reserve(n);
        for (auto &kv : pos) {
            auto &vec = kv.second;
            if ((int)vec.size() != 2) return false;
            ans.emplace_back(vec[0], vec[1]);
        }
        if ((int)ans.size() != n) return false;
        answer = move(ans);
        return true;
    };
    auto try_flat_pairs = [&](int s)->bool {
        if (s < 0 || s + total > M) return false;
        vector<int> cnt(total + 1, 0);
        for (int i = 0; i < total; ++i) {
            long long t = tokens[s + i];
            if (!(1 <= t && t <= total)) return false;
            cnt[(int)t]++;
        }
        for (int i = 1; i <= total; ++i) {
            if (cnt[i] != 1) return false;
        }
        vector<pair<int,int>> ans;
        ans.reserve(n);
        for (int i = 0; i < total; i += 2) {
            int a = (int)tokens[s + i];
            int b = (int)tokens[s + i + 1];
            ans.emplace_back(a, b);
        }
        if ((int)ans.size() != n) return false;
        answer = move(ans);
        return true;
    };

    bool found = false;
    if (M == total) {
        if (try_involution(0) || try_colors(0) || try_flat_pairs(0)) {
            found = true;
        }
    } else if (M > total) {
        int maxWindows = 200;
        int limit = min(maxWindows, M - total + 1);
        for (int s = 0; s < limit && !found; ++s) {
            if (try_involution(s) || try_colors(s) || try_flat_pairs(s)) {
                found = true;
                break;
            }
        }
        if (!found) {
            int s = M - total;
            if (try_involution(s) || try_colors(s) || try_flat_pairs(s)) {
                found = true;
            }
        }
    }

    if (!found) {
        answer.clear();
        answer.reserve(n);
        for (int i = 1; i <= total; i += 2) {
            answer.emplace_back(i, i + 1);
        }
    }

    for (auto &p : answer) {
        cout << p.first << ' ' << p.second << '\n';
    }
    return 0;
}