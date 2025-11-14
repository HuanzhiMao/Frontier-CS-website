#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    int si, sj;
    cin >> si >> sj;
    vector<string> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    vector<string> t(M);
    for (int i = 0; i < M; ++i) cin >> t[i];

    // Precompute positions for each letter
    vector<vector<pair<int,int>>> pos(26);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            pos[A[i][j]-'A'].push_back({i, j});
        }
    }

    // Build the sequence to type with simple overlaps (up to 4 chars)
    string seq;
    seq.reserve(M * 5);
    string cur = "";
    cur.reserve(M * 5);
    for (int k = 0; k < M; ++k) {
        const string& w = t[k];
        int maxL = min(4, (int)cur.size());
        int overlap = 0;
        for (int L = maxL; L >= 0; --L) {
            bool ok = true;
            for (int x = 0; x < L; ++x) {
                if (cur[cur.size() - L + x] != w[x]) { ok = false; break; }
            }
            if (ok) { overlap = L; break; }
        }
        for (int i = overlap; i < 5; ++i) {
            seq.push_back(w[i]);
            cur.push_back(w[i]);
        }
    }

    // Type the sequence with simple 1-step lookahead to next character
    vector<pair<int,int>> moves;
    moves.reserve(seq.size());
    int ci = si, cj = sj;

    for (size_t p = 0; p < seq.size(); ++p) {
        int c1 = seq[p] - 'A';
        int c2 = -1;
        if (p + 1 < seq.size()) c2 = seq[p + 1] - 'A';

        const auto& v1 = pos[c1];
        pair<int,int> best = v1[0];
        int bestCost = INT_MAX;
        int bestD0 = INT_MAX;

        for (auto &cand : v1) {
            int d0 = abs(cand.first - ci) + abs(cand.second - cj);
            int cost = d0;
            if (c2 != -1) {
                int d2 = INT_MAX;
                const auto& v2 = pos[c2];
                for (auto &q : v2) {
                    int dd = abs(q.first - cand.first) + abs(q.second - cand.second);
                    if (dd < d2) d2 = dd;
                }
                cost += d2;
            }
            if (cost < bestCost || (cost == bestCost && d0 < bestD0)) {
                bestCost = cost;
                bestD0 = d0;
                best = cand;
            }
        }

        moves.push_back(best);
        ci = best.first;
        cj = best.second;
        if ((int)moves.size() >= 5000) break;
    }

    for (auto &mv : moves) {
        cout << mv.first << ' ' << mv.second << '\n';
    }
    return 0;
}