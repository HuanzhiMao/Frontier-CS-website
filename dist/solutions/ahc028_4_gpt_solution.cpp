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
    for (int k = 0; k < M; ++k) cin >> t[k];

    // Build S by greedy overlaps
    vector<bool> used(M, false);
    int startIdx = -1;
    for (int k = 0; k < M; ++k) {
        if (t[k][0] == A[si][sj]) { startIdx = k; break; }
    }
    if (startIdx == -1) startIdx = 0;
    used[startIdx] = true;
    string S = t[startIdx];
    int usedCnt = 1;

    while (usedCnt < M) {
        int bestIdx = -1;
        int bestK = -1;
        int Ls = (int)S.size();
        for (int k = 0; k < M; ++k) {
            if (used[k]) continue;
            int ov = 0;
            for (int len = 4; len >= 1; --len) {
                if (Ls >= len) {
                    bool ok = true;
                    for (int x = 0; x < len; ++x) {
                        if (S[Ls - len + x] != t[k][x]) { ok = false; break; }
                    }
                    if (ok) { ov = len; break; }
                }
            }
            if (ov > bestK || bestIdx == -1) {
                bestK = ov;
                bestIdx = k;
            }
        }
        if (bestIdx == -1) break;
        S += t[bestIdx].substr(bestK);
        used[bestIdx] = true;
        usedCnt++;
    }

    // Precompute positions for each letter
    array<vector<pair<int,int>>, 26> pos;
    for (int i = 0; i < 26; ++i) pos[i].clear();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char c = A[i][j];
            pos[c - 'A'].push_back({i, j});
        }
    }

    int L = (int)S.size();
    if (L == 0) return 0;

    vector<vector<int>> parent(L);
    vector<int> dpPrev, dpCur;
    vector<pair<int,int>> prevPos, curPos;

    curPos = pos[S[0] - 'A'];
    dpCur.assign(curPos.size(), INT_MAX / 4);
    parent[0].assign(curPos.size(), -1);
    for (size_t q = 0; q < curPos.size(); ++q) {
        int dist = abs(curPos[q].first - si) + abs(curPos[q].second - sj);
        dpCur[q] = dist + 1;
    }
    dpPrev = dpCur;
    prevPos = curPos;

    for (int idx = 1; idx < L; ++idx) {
        curPos = pos[S[idx] - 'A'];
        parent[idx].assign(curPos.size(), -1);
        dpCur.assign(curPos.size(), INT_MAX / 4);
        for (size_t p = 0; p < prevPos.size(); ++p) {
            auto [pi, pj] = prevPos[p];
            int base = dpPrev[p];
            for (size_t q = 0; q < curPos.size(); ++q) {
                auto [qi, qj] = curPos[q];
                int cost = base + abs(qi - pi) + abs(qj - pj) + 1;
                if (cost < dpCur[q]) {
                    dpCur[q] = cost;
                    parent[idx][q] = (int)p;
                }
            }
        }
        dpPrev.swap(dpCur);
        prevPos.swap(curPos);
    }

    vector<int> chosenIndex(L);
    int bestq = 0;
    for (size_t q = 1; q < prevPos.size(); ++q) {
        if (dpPrev[q] < dpPrev[bestq]) bestq = (int)q;
    }
    chosenIndex[L - 1] = bestq;
    for (int idx = L - 1; idx >= 1; --idx) {
        int p = parent[idx][chosenIndex[idx]];
        if (p < 0) p = 0;
        chosenIndex[idx - 1] = p;
    }

    for (int idx = 0; idx < L; ++idx) {
        auto &v = pos[S[idx] - 'A'];
        auto [i, j] = v[chosenIndex[idx]];
        cout << i << ' ' << j << '\n';
    }

    return 0;
}