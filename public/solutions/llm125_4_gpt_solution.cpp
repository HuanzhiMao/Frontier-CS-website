#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<long long> a(2 * N + 1);
    for (int i = 1; i <= 2 * N; ++i) {
        if (!(cin >> a[i])) return 0;
    }
    
    unordered_map<long long, int> first;
    first.reserve(2 * N * 2);
    vector<pair<int,int>> ans;
    ans.reserve(N);
    
    for (int i = 1; i <= 2 * N; ++i) {
        long long v = a[i];
        auto it = first.find(v);
        if (it == first.end()) {
            first.emplace(v, i);
        } else {
            ans.emplace_back(it->second, i);
            first.erase(it);
        }
    }
    
    for (auto &p : ans) {
        cout << p.first << ' ' << p.second << '\n';
    }
    return 0;
}