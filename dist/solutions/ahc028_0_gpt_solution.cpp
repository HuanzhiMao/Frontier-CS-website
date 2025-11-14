#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    int si, sj;
    cin >> si >> sj;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];
    vector<string> words(M);
    for (int i = 0; i < M; ++i) cin >> words[i];

    int totalCells = N * N;
    auto idxOf = [N](int i, int j) { return i * N + j; };
    vector<int> rr(totalCells), cc(totalCells);
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) {
        int idx = idxOf(i, j);
        rr[idx] = i; cc[idx] = j;
    }

    // Precompute distances between all cells
    vector<int> distMat(totalCells * totalCells);
    for (int a = 0; a < totalCells; ++a) {
        for (int b = 0; b < totalCells; ++b) {
            distMat[a * totalCells + b] = abs(rr[a] - rr[b]) + abs(cc[a] - cc[b]);
        }
    }
    auto dist = [&](int a, int b) -> int { return distMat[a * totalCells + b]; };

    // Positions for each letter
    vector<vector<int>> letterPos(26);
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) {
        char ch = grid[i][j];
        letterPos[ch - 'A'].push_back(idxOf(i, j));
    }

    // Map word to index
    unordered_map<string, int> strToIdx;
    strToIdx.reserve(M * 2);
    for (int i = 0; i < M; ++i) strToIdx[words[i]] = i;

    vector<pair<int,int>> ops;
    string S;
    vector<bool> used(M, false);
    int usedCount = 0;
    int curPos = idxOf(si, sj);

    auto updateUsedNew = [&](int oldLen) {
        int newLen = (int)S.size();
        for (int end = oldLen; end < newLen; ++end) {
            int st = end - 4;
            if (st >= 0) {
                string sub = S.substr(st, 5);
                auto it = strToIdx.find(sub);
                if (it != strToIdx.end()) {
                    int id = it->second;
                    if (!used[id]) {
                        used[id] = true;
                        usedCount++;
                    }
                }
            }
        }
    };

    auto computeCost = [&](int startIdx, const string& suf) -> long long {
        if (suf.empty()) return 0LL;
        const long long INF = (1LL<<60);
        int L = (int)suf.size();
        const vector<int>& pos0 = letterPos[suf[0]-'A'];
        int n0 = (int)pos0.size();
        if (n0 == 0) return INF;
        vector<long long> dpPrev(n0, INF);
        for (int i = 0; i < n0; ++i) dpPrev[i] = (long long)dist(startIdx, pos0[i]) + 1;
        for (int step = 1; step < L; ++step) {
            const vector<int>& prevList = letterPos[suf[step-1]-'A'];
            const vector<int>& posList = letterPos[suf[step]-'A'];
            int pn = (int)posList.size();
            int qn = (int)prevList.size();
            vector<long long> dpCur(pn, INF);
            for (int i = 0; i < pn; ++i) {
                long long best = INF;
                int pi = posList[i];
                for (int j = 0; j < qn; ++j) {
                    long long cand = dpPrev[j] + (long long)dist(prevList[j], pi) + 1;
                    if (cand < best) best = cand;
                }
                dpCur[i] = best;
            }
            dpPrev.swap(dpCur);
        }
        long long best = *min_element(dpPrev.begin(), dpPrev.end());
        return best;
    };

    auto computePath = [&](int startIdx, const string& suf, vector<int>& path) -> long long {
        path.clear();
        if (suf.empty()) return 0LL;
        const long long INF = (1LL<<60);
        int L = (int)suf.size();
        vector<vector<int>> posList(L);
        for (int i = 0; i < L; ++i) posList[i] = letterPos[suf[i]-'A'];

        if (posList[0].empty()) return INF;

        vector<vector<long long>> DP(L);
        vector<vector<int>> PRE(L);
        DP[0].assign(posList[0].size(), INF);
        PRE[0].assign(posList[0].size(), -1);
        for (int i = 0; i < (int)posList[0].size(); ++i) {
            DP[0][i] = (long long)dist(startIdx, posList[0][i]) + 1;
        }
        for (int s = 1; s < L; ++s) {
            int pn = (int)posList[s].size();
            int qn = (int)posList[s-1].size();
            DP[s].assign(pn, INF);
            PRE[s].assign(pn, -1);
            for (int i = 0; i < pn; ++i) {
                long long best = INF;
                int bestj = -1;
                int pi = posList[s][i];
                for (int j = 0; j < qn; ++j) {
                    long long cand = DP[s-1][j] + (long long)dist(posList[s-1][j], pi) + 1;
                    if (cand < best) {
                        best = cand;
                        bestj = j;
                    }
                }
                DP[s][i] = best;
                PRE[s][i] = bestj;
            }
        }
        long long best = INF;
        int lastIdx = -1;
        int ln = (int)posList[L-1].size();
        for (int i = 0; i < ln; ++i) {
            if (DP[L-1][i] < best) {
                best = DP[L-1][i];
                lastIdx = i;
            }
        }
        if (lastIdx == -1) return INF;
        vector<int> chosen(L);
        int cur = lastIdx;
        for (int s = L-1; s >= 0; --s) {
            chosen[s] = posList[s][cur];
            cur = PRE[s][cur];
        }
        path = chosen;
        return best;
    };

    // Initialize with any substrings already present (none since S empty)
    updateUsedNew(0);

    // Main loop: greedily pick the next word minimizing added cost with overlap consideration
    while (usedCount < M) {
        long long globalBestCost = (1LL<<60);
        int bestWord = -1;
        int bestOverlap = 0;
        int sLen = (int)S.size();
        int maxOverlapPossible = min(4, sLen);

        for (int i = 0; i < M; ++i) if (!used[i]) {
            const string& w = words[i];
            long long bestCostThis = (1LL<<60);
            int bestLThis = 0;
            for (int L = maxOverlapPossible; L >= 0; --L) {
                bool ok = true;
                for (int k = 0; k < L; ++k) {
                    if (S[sLen - L + k] != w[k]) { ok = false; break; }
                }
                if (!ok) continue;
                string suf = w.substr(L);
                long long cst = computeCost(curPos, suf);
                if (cst < bestCostThis || (cst == bestCostThis && L > bestLThis)) {
                    bestCostThis = cst;
                    bestLThis = L;
                }
            }
            if (bestCostThis < globalBestCost || (bestCostThis == globalBestCost && bestLThis > bestOverlap)) {
                globalBestCost = bestCostThis;
                bestWord = i;
                bestOverlap = bestLThis;
            }
        }

        if (bestWord == -1) {
            // Fallback: in case something went wrong, type a random nearby letter to progress
            // Pick the current cell letter (cost 1)
            int ci = rr[curPos], cj = cc[curPos];
            ops.emplace_back(ci, cj);
            S.push_back(grid[ci][cj]);
            int oldLen = (int)S.size() - 1;
            updateUsedNew(oldLen);
            if ((int)ops.size() >= 5000) break;
            continue;
        }

        string suffix = words[bestWord].substr(bestOverlap);
        vector<int> path;
        computePath(curPos, suffix, path);
        for (int idx : path) {
            ops.emplace_back(rr[idx], cc[idx]);
        }
        int oldLen = (int)S.size();
        S += suffix;
        updateUsedNew(oldLen);
        if (!path.empty()) curPos = path.back();
        if ((int)ops.size() >= 5000) break;
    }

    // Output operations
    for (auto &p : ops) {
        cout << p.first << ' ' << p.second << '\n';
    }
    return 0;
}