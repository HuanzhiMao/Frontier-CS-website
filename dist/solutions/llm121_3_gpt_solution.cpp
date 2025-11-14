#include <bits/stdc++.h>
using namespace std;

static inline int charToCode(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'G') return 2;
    if (c == 'T') return 3;
    return -1; // '?'
}

static inline string encodeKey(const vector<pair<int,uint8_t>>& v) {
    string key;
    key.resize(v.size() * 5);
    char* p = key.data();
    for (auto &pr : v) {
        uint32_t pos = (uint32_t)pr.first;
        p[0] = (char)(pos & 0xFF);
        p[1] = (char)((pos >> 8) & 0xFF);
        p[2] = (char)((pos >> 16) & 0xFF);
        p[3] = (char)((pos >> 24) & 0xFF);
        p[4] = (char)pr.second;
        p += 5;
    }
    return key;
}

static inline bool isSubset(const vector<pair<int,uint8_t>>& a, const vector<pair<int,uint8_t>>& b) {
    // return true if a ⊆ b (same letters on positions)
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i].first == b[j].first) {
            if (a[i].second != b[j].second) return false;
            ++i; ++j;
        } else if (a[i].first > b[j].first) {
            ++j;
        } else { // a[i].first < b[j].first
            return false;
        }
    }
    return i == a.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<string> s(m);
    for (int i = 0; i < m; ++i) cin >> s[i];

    // Collect patterns: for each string, list positions with fixed letters
    vector<vector<pair<int,uint8_t>>> patterns_raw;
    patterns_raw.reserve(m);
    vector<char> used(n, 0);
    bool hasEmpty = false;
    for (int i = 0; i < m; ++i) {
        vector<pair<int,uint8_t>> v;
        v.reserve(n);
        for (int j = 0; j < n; ++j) {
            int code = charToCode(s[i][j]);
            if (code != -1) {
                v.emplace_back(j, (uint8_t)code);
                used[j] = 1;
            }
        }
        if (v.empty()) {
            // A pattern with all '?' covers all strings
            cout.setf(std::ios::fixed); cout<<setprecision(15)<<1.0<<"\n";
            return 0;
        }
        sort(v.begin(), v.end());
        patterns_raw.push_back(move(v));
    }

    // Compress positions to only those used at least once
    vector<int> mapPos(n, -1);
    int k = 0;
    for (int j = 0; j < n; ++j) if (used[j]) mapPos[j] = k++;
    int nEff = k;
    // If somehow none used (shouldn't happen due to early return), probability is 0 (but covered above).
    if (nEff == 0) {
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<0.0<<"\n";
        return 0;
    }

    // Remap positions
    vector<vector<pair<int,uint8_t>>> patterns;
    patterns.reserve(patterns_raw.size());
    for (auto &v : patterns_raw) {
        vector<pair<int,uint8_t>> w;
        w.reserve(v.size());
        for (auto &pr : v) {
            int np = mapPos[pr.first];
            if (np >= 0) w.emplace_back(np, pr.second);
        }
        sort(w.begin(), w.end());
        patterns.push_back(move(w));
    }
    patterns_raw.clear();

    // Deduplicate identical patterns (same set of fixed positions with same letters)
    unordered_set<string> seen;
    seen.reserve(patterns.size() * 2 + 1);
    vector<vector<pair<int,uint8_t>>> uniq;
    uniq.reserve(patterns.size());
    for (auto &v : patterns) {
        string key = encodeKey(v);
        if (seen.insert(key).second) {
            uniq.push_back(move(v));
        }
    }
    patterns.clear();

    // Remove dominated patterns: if there exists a pattern a such that a ⊆ b, then b is redundant
    sort(uniq.begin(), uniq.end(), [](const auto& a, const auto& b){
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    });
    int M = (int)uniq.size();
    vector<char> removed(M, 0);
    for (int j = 0; j < M; ++j) {
        if (removed[j]) continue;
        for (int i = 0; i < j; ++i) {
            if (removed[i]) continue;
            if (isSubset(uniq[i], uniq[j])) {
                removed[j] = 1;
                break;
            }
        }
    }
    vector<vector<pair<int,uint8_t>>> pats;
    pats.reserve(M);
    for (int i = 0; i < M; ++i) if (!removed[i]) pats.push_back(move(uniq[i]));
    uniq.clear();
    M = (int)pats.size();

    if (M == 0) {
        // No patterns left => no string covered
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<0.0<<"\n";
        return 0;
    }

    // Precompute 4^{-r} for r=0..nEff
    vector<long double> pow4(nEff + 1);
    pow4[0] = 1.0L;
    for (int i = 1; i <= nEff; ++i) pow4[i] = pow4[i-1] * 0.25L;

    // DFS with backtracking
    vector<int8_t> forced(nEff, -1); // -1 unforced, else 0..3 letter
    vector<int> changeStack;
    changeStack.reserve(nEff);
    int r = 0;
    long double ans = 0.0L;

    function<void(int,int)> dfs = [&](int idx, int chosenCnt) {
        if (idx == M) return;

        // Exclude current pattern
        dfs(idx + 1, chosenCnt);

        // Include current pattern
        int checkpoint = (int)changeStack.size();
        bool conflict = false;
        auto &vec = pats[idx];
        for (auto &pr : vec) {
            int p = pr.first;
            int l = pr.second;
            int8_t cur = forced[p];
            if (cur == -1) {
                forced[p] = (int8_t)l;
                changeStack.push_back(p);
                ++r;
            } else if (cur != l) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            int szNext = chosenCnt + 1;
            ans += ((szNext & 1) ? +1.0L : -1.0L) * pow4[r];
            dfs(idx + 1, szNext);
        }
        // Revert changes
        while ((int)changeStack.size() > checkpoint) {
            int p = changeStack.back(); changeStack.pop_back();
            forced[p] = -1;
            --r;
        }
    };

    dfs(0, 0);

    long double result = ans;
    if (result < 0) result = 0;
    if (result > 1) result = 1;
    cout.setf(std::ios::fixed);
    cout << setprecision(15) << (double)result << "\n";
    return 0;
}