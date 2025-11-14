#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<pair<int,int>> p(M);
    for (int i = 0; i < M; ++i) cin >> p[i].first >> p[i].second;

    int ci = p[0].first, cj = p[0].second;
    vector<pair<char,char>> out;
    out.reserve(2 * N * M);

    for (int k = 1; k < M; ++k) {
        int ti = p[k].first, tj = p[k].second;
        while (ci < ti) { out.emplace_back('M', 'D'); ++ci; }
        while (ci > ti) { out.emplace_back('M', 'U'); --ci; }
        while (cj < tj) { out.emplace_back('M', 'R'); ++cj; }
        while (cj > tj) { out.emplace_back('M', 'L'); --cj; }
    }

    for (auto &op : out) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}