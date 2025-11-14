#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> tokens;
    long long x;
    while (cin >> x) tokens.push_back(x);
    if (tokens.empty()) return 0;

    long long N = tokens[0];
    long long total = 2 * N;

    vector<long long> rest;
    if (tokens.size() > 1) {
        size_t available = tokens.size() - 1;
        size_t use = min<long long>(available, total);
        rest.assign(tokens.begin() + 1, tokens.begin() + 1 + use);
    }

    vector<pair<int,int>> out;
    out.reserve(N);

    if ((long long)rest.size() == total) {
        // Try to detect if rest is a list of 2N distinct indices (pairs given consecutively)
        bool isConsecutivePairsList = true;
        vector<int> cnt(total + 1, 0);
        for (long long v : rest) {
            if (v < 1 || v > total) { isConsecutivePairsList = false; break; }
            cnt[(int)v]++;
        }
        if (isConsecutivePairsList) {
            for (int i = 1; i <= total; ++i) {
                if (cnt[i] != 1) { isConsecutivePairsList = false; break; }
            }
        }
        if (isConsecutivePairsList) {
            for (int i = 0; i < total; i += 2) {
                out.emplace_back((int)rest[i], (int)rest[i + 1]);
            }
        } else {
            // Try to detect if rest is a partner mapping arr[i] = partner(i), with involution property
            vector<int> arr(total + 1, 0);
            bool validMap = true;
            for (int i = 1; i <= total; ++i) {
                long long val = rest[i - 1];
                if (val < 1 || val > total) { validMap = false; break; }
                arr[i] = (int)val;
            }
            if (validMap) {
                for (int i = 1; i <= total; ++i) {
                    if (arr[i] < 1 || arr[i] > total || arr[i] == i || arr[arr[i]] != i) { validMap = false; break; }
                }
            }
            if (validMap) {
                vector<char> used(total + 1, false);
                for (int i = 1; i <= total; ++i) {
                    if (!used[i]) {
                        int j = arr[i];
                        if (j >= 1 && j <= total && !used[j]) {
                            out.emplace_back(i, j);
                            used[i] = used[j] = true;
                        }
                    }
                }
            } else {
                // Treat as types: each type occurs twice
                unordered_map<long long,int> first;
                first.reserve((size_t)(2 * N));
                for (int i = 1; i <= total; ++i) {
                    long long t = rest[i - 1];
                    auto it = first.find(t);
                    if (it == first.end()) {
                        first.emplace(t, i);
                    } else {
                        out.emplace_back(it->second, i);
                        first.erase(it);
                    }
                }
                // If for some reason not all paired, fill arbitrarily
                if ((int)out.size() < (int)N) {
                    vector<int> deg(total + 1, 0);
                    for (auto &p : out) { deg[p.first]++; deg[p.second]++; }
                    vector<int> unused;
                    for (int i = 1; i <= total; ++i) if (deg[i] == 0) unused.push_back(i);
                    for (size_t i = 0; i + 1 < unused.size() && out.size() < (size_t)N; i += 2)
                        out.emplace_back(unused[i], unused[i + 1]);
                }
            }
        }
    } else if ((long long)rest.size() == 0) {
        // No extra info; fallback trivial pairing
        for (int i = 1; i <= total; i += 2) out.emplace_back(i, i + 1);
    } else {
        // If rest size is even, assume it's a list of pairs; otherwise fallback.
        if ((rest.size() % 2) == 0) {
            for (size_t i = 0; i + 1 < rest.size() && out.size() < (size_t)N; i += 2) {
                out.emplace_back((int)rest[i], (int)rest[i + 1]);
            }
            if (out.size() < (size_t)N) {
                vector<int> deg(total + 1, 0);
                for (auto &p : out) { 
                    if (p.first >= 1 && p.first <= total) deg[p.first]++;
                    if (p.second >= 1 && p.second <= total) deg[p.second]++; 
                }
                vector<int> unused;
                for (int i = 1; i <= total; ++i) if (deg[i] == 0) unused.push_back(i);
                for (size_t i = 0; i + 1 < unused.size() && out.size() < (size_t)N; i += 2)
                    out.emplace_back(unused[i], unused[i + 1]);
            }
        } else {
            for (int i = 1; i <= total; i += 2) out.emplace_back(i, i + 1);
        }
    }

    if (out.size() > (size_t)N) out.resize(N);
    else if (out.size() < (size_t)N) {
        vector<int> deg(total + 1, 0);
        for (auto &p : out) {
            if (p.first >= 1 && p.first <= total) deg[p.first]++;
            if (p.second >= 1 && p.second <= total) deg[p.second]++;
        }
        vector<int> unused;
        for (int i = 1; i <= total; ++i) if (deg[i] == 0) unused.push_back(i);
        for (size_t i = 0; i + 1 < unused.size() && out.size() < (size_t)N; i += 2)
            out.emplace_back(unused[i], unused[i + 1]);
    }

    for (auto &p : out) {
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}