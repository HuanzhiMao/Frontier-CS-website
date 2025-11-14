#include <bits/stdc++.h>
using namespace std;

static inline bool is01string(const string &s, int n) {
    if ((int)s.size() != n) return false;
    for (char c : s) if (c != '0' && c != '1') return false;
    return true;
}

vector<int> build_perm(const vector<vector<char>>& C, int n, int leftColor, bool rev) {
    deque<int> dq;
    dq.push_back(1);
    for (int v = 2; v <= n; ++v) {
        if (C[v][dq.front()] == leftColor) dq.push_front(v);
        else dq.push_back(v);
    }
    vector<int> p;
    p.reserve(n);
    if (!rev) {
        for (int x : dq) p.push_back(x);
    } else {
        for (auto it = dq.rbegin(); it != dq.rend(); ++it) p.push_back(*it);
    }
    return p;
}

bool lexLess(const vector<int>& a, const vector<int>& b) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    while ( (cin >> n) ) {
        vector<vector<char>> C(n + 1, vector<char>(n + 1, 0));
        for (int i = 1; i <= n; ++i) {
            string s;
            cin >> s;
            if (is01string(s, n)) {
                for (int j = 1; j <= n; ++j) C[i][j] = s[j - 1] - '0';
            } else {
                // s is the first token (likely "0" or "1"), then read the rest tokens
                C[i][1] = s[0] - '0';
                for (int j = 2; j <= n; ++j) {
                    string t; cin >> t;
                    C[i][j] = t[0] - '0';
                }
            }
        }

        // Build candidates using two variants and their reverses
        vector<int> best = build_perm(C, n, 0, false);
        vector<int> cand;

        cand = build_perm(C, n, 0, true);
        if (lexLess(cand, best)) best = move(cand);

        cand = build_perm(C, n, 1, false);
        if (lexLess(cand, best)) best = move(cand);

        cand = build_perm(C, n, 1, true);
        if (lexLess(cand, best)) best = move(cand);

        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << best[i];
        }
        cout << '\n';
    }
    return 0;
}