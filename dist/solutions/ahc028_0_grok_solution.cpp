#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    int si, sj;
    cin >> si >> sj;
    vector<string> grid(N);
    for (int i = 0; i < N; i++) cin >> grid[i];
    vector<string> t(M);
    for (int i = 0; i < M; i++) cin >> t[i];
    const int GS = N * N;
    vector<int> positions[26];
    int rrr[GS], ccc[GS];
    int idx = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char ch = grid[i][j];
            int let = ch - 'A';
            positions[let].push_back(idx);
            rrr[idx] = i;
            ccc[idx] = j;
            idx++;
        }
    }
    auto manh = [&](int a, int b) {
        return abs(rrr[a] - rrr[b]) + abs(ccc[a] - ccc[b]);
    };
    vector<string> curr = t;
    while (curr.size() > 1) {
        int best_i = -1, best_j = -1, best_o = -1;
        for (int i = 0; i < curr.size(); i++) {
            for (int j = 0; j < curr.size(); j++) {
                if (i == j) continue;
                const string& a = curr[i];
                const string& b = curr[j];
                int minoo = min(a.size(), b.size());
                int maxoo = 0;
                for (int o = min(10, minoo - 1); o >= 0; o--) {
                    bool eq = true;
                    for (int k = 0; k < o; k++) {
                        if (a[a.size() - o + k] != b[k]) {
                            eq = false;
                            break;
                        }
                    }
                    if (eq) {
                        maxoo = o;
                        break;
                    }
                }
                if (maxoo > best_o) {
                    best_o = maxoo;
                    best_i = i;
                    best_j = j;
                }
            }
        }
        string a = curr[best_i];
        string b = curr[best_j];
        string newstr = a + b.substr(best_o);
        vector<string> newc;
        for (int k = 0; k < curr.size(); k++) {
            if (k != best_i && k != best_j) newc.push_back(curr[k]);
        }
        newc.push_back(newstr);
        curr = newc;
    }
    string S = curr[0];
    int L = S.length();
    int start = si * N + sj;
    const int INF = 1e9 + 5;
    vector<vector<int>> dpc(L + 1, vector<int>(GS, INF));
    vector<vector<int>> prevp(L + 1, vector<int>(GS, -1));
    dpc[0][start] = 0;
    for (int lay = 1; lay <= L; lay++) {
        int ltr = S[lay - 1] - 'A';
        vector<int> candidates = positions[ltr];
        for (int pp : candidates) {
            int minn = INF;
            int best = -1;
            for (int pr = 0; pr < GS; pr++) {
                if (dpc[lay - 1][pr] == INF) continue;
                int dd = manh(pr, pp);
                int tot = dpc[lay - 1][pr] + dd;
                if (tot < minn) {
                    minn = tot;
                    best = pr;
                }
            }
            if (minn < INF) {
                dpc[lay][pp] = minn;
                prevp[lay][pp] = best;
            }
        }
    }
    int min_sum_dist = INF;
    int best_end = -1;
    for (int pp = 0; pp < GS; pp++) {
        if (dpc[L][pp] < min_sum_dist) {
            min_sum_dist = dpc[L][pp];
            best_end = pp;
        }
    }
    vector<pair<int, int>> sequence;
    int current = best_end;
    for (int lay = L; lay >= 1; lay--) {
        sequence.push_back({rrr[current], ccc[current]});
        current = prevp[lay][current];
    }
    reverse(sequence.begin(), sequence.end());
    for (auto [ii, jj] : sequence) {
        cout << ii << " " << jj << endl;
    }
    return 0;
}