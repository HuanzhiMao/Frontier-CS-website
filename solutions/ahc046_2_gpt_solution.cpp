#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<pair<int,int>> p(M);
    for (int k = 0; k < M; k++) cin >> p[k].first >> p[k].second;

    vector<pair<char,char>> out;
    out.reserve(2 * N * M);

    for (int k = 1; k < M; k++) {
        int ci = p[k-1].first, cj = p[k-1].second;
        int ti = p[k].first, tj = p[k].second;

        while (ci < ti) {
            out.emplace_back('M','D');
            ci++;
        }
        while (ci > ti) {
            out.emplace_back('M','U');
            ci--;
        }
        while (cj < tj) {
            out.emplace_back('M','R');
            cj++;
        }
        while (cj > tj) {
            out.emplace_back('M','L');
            cj--;
        }
    }

    for (auto &cmd : out) {
        cout << cmd.first << ' ' << cmd.second << '\n';
    }
    return 0;
}