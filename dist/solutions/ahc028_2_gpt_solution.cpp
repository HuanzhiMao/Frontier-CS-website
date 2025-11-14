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
    for (int i = 0; i < N; i++) cin >> A[i];
    vector<string> t(M);
    for (int i = 0; i < M; i++) cin >> t[i];

    // Precompute positions for each letter
    vector<vector<pair<int,int>>> pos(26);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pos[A[i][j] - 'A'].push_back({i, j});
        }
    }

    auto mdist = [](const pair<int,int>& a, const pair<int,int>& b) -> int {
        return abs(a.first - b.first) + abs(a.second - b.second);
    };

    // Choose starting string to be the one whose first letter is closest to the starting position
    vector<int> distStart(26, INT_MAX);
    pair<int,int> startPos = {si, sj};
    for (int c = 0; c < 26; c++) {
        for (auto &p : pos[c]) {
            distStart[c] = min(distStart[c], mdist(startPos, p));
        }
    }
    int startIdx = 0;
    int bestDist = INT_MAX;
    for (int k = 0; k < M; k++) {
        int c = t[k][0] - 'A';
        int d = distStart[c];
        if (d < bestDist) {
            bestDist = d;
            startIdx = k;
        }
    }

    // Build a superstring S by greedy overlaps
    vector<char> used(M, false);
    string S = t[startIdx];
    used[startIdx] = true;
    int usedCount = 1;

    auto overlap = [&](const string& s, const string& u) -> int {
        int maxr = min(5, (int)s.size());
        for (int r = maxr; r >= 1; --r) {
            bool ok = true;
            for (int i = 0; i < r; i++) {
                if (s[s.size() - r + i] != u[i]) {
                    ok = false; break;
                }
            }
            if (ok) return r;
        }
        return 0;
    };

    while (usedCount < M) {
        int bestk = -1;
        int bestov = -1;
        for (int k = 0; k < M; k++) {
            if (used[k]) continue;
            int ov = overlap(S, t[k]);
            if (ov > bestov) {
                bestov = ov;
                bestk = k;
            }
        }
        if (bestk == -1) {
            for (int k = 0; k < M; k++) if (!used[k]) { bestk = k; bestov = 0; break; }
        }
        for (int idx = bestov; idx < 5; ++idx) S.push_back(t[bestk][idx]);
        used[bestk] = true;
        usedCount++;
    }

    int L = (int)S.size();
    vector<int> letters(L);
    for (int i = 0; i < L; i++) letters[i] = S[i] - 'A';

    // DP to minimize total movement distance (sum of manhattan distances)
    vector<vector<int>> parent(L);
    vector<int> dpPrev, dpCur;
    int size0 = (int)pos[letters[0]].size();
    dpCur.assign(size0, INT_MAX / 4);
    parent[0].assign(size0, -1);
    for (int j = 0; j < size0; j++) {
        dpCur[j] = mdist(startPos, pos[letters[0]][j]);
    }

    for (int i = 1; i < L; i++) {
        dpPrev = dpCur;
        int prevSize = (int)dpPrev.size();
        int curSize = (int)pos[letters[i]].size();
        dpCur.assign(curSize, INT_MAX / 4);
        parent[i].assign(curSize, -1);
        for (int j = 0; j < curSize; j++) {
            const auto &pc = pos[letters[i]][j];
            int bestCost = INT_MAX / 4;
            int bestk = -1;
            for (int k = 0; k < prevSize; k++) {
                const auto &pp = pos[letters[i - 1]][k];
                int cost = dpPrev[k] + abs(pc.first - pp.first) + abs(pc.second - pp.second);
                if (cost < bestCost) {
                    bestCost = cost;
                    bestk = k;
                }
            }
            dpCur[j] = bestCost;
            parent[i][j] = bestk;
        }
    }

    // Reconstruct path
    int lastSize = (int)dpCur.size();
    int bestj = 0;
    int bestcost = dpCur[0];
    for (int j = 1; j < lastSize; j++) {
        if (dpCur[j] < bestcost) {
            bestcost = dpCur[j];
            bestj = j;
        }
    }

    vector<pair<int,int>> ans(L);
    int idx = bestj;
    for (int i = L - 1; i >= 0; --i) {
        ans[i] = pos[letters[i]][idx];
        idx = parent[i][idx];
        if (i == 0) break;
    }

    for (int i = 0; i < L; i++) {
        cout << ans[i].first << " " << ans[i].second << "\n";
    }

    return 0;
}