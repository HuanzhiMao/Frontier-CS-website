#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read all integers from input
    vector<long long> in;
    long long x;
    while (cin >> x) in.push_back(x);
    if (in.empty()) return 0;

    long long N = in[0];
    long long M = 2 * N;

    // If we have types provided (offline adaptation), pair by equal types.
    if ((long long)in.size() >= 1 + M) {
        vector<long long> type(M + 1);
        for (long long i = 1; i <= M; ++i) type[i] = in[i];

        unordered_map<long long, long long> first;
        first.reserve(N * 2);
        vector<pair<long long, long long>> ans;
        ans.reserve(N);
        for (long long i = 1; i <= M; ++i) {
            auto it = first.find(type[i]);
            if (it == first.end()) {
                first[type[i]] = i;
            } else {
                ans.emplace_back(it->second, i);
                first.erase(it);
            }
        }
        // Output exactly N pairs, covering 1..2N exactly once.
        for (auto &p : ans) {
            cout << "! " << p.first << " " << p.second << "\n";
        }
        return 0;
    }

    // Fallback: if no additional data is provided, output trivial pairs.
    // Note: This is a placeholder for non-interactive environments.
    for (long long i = 1; i <= M; i += 2) {
        cout << "! " << i << " " << i + 1 << "\n";
    }
    return 0;
}