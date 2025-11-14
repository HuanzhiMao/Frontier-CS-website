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

    auto overlap4 = [](const string &suf4, const string &pre4) -> int {
        for (int len = 4; len >= 0; --len) {
            bool ok = true;
            for (int k = 0; k < len; ++k) {
                if (suf4[4 - len + k] != pre4[k]) { ok = false; break; }
            }
            if (ok) return len;
        }
        return 0;
    };

    // Build order of t using greedy overlap
    vector<int> used(M, 0);
    vector<int> ord;
    ord.reserve(M);

    // Choose the first word: prefer one starting with the starting cell's char
    int first = 0;
    char startChar = A[si][sj];
    for (int i = 0; i < M; ++i) {
        if (t[i][0] == startChar) { first = i; break; }
    }
    used[first] = 1;
    ord.push_back(first);
    string last4 = t[first].substr(1, 4);

    for (int cnt = 1; cnt < M; ++cnt) {
        int best_j = -1, best_l = -1;
        for (int j = 0; j < M; ++j) if (!used[j]) {
            int l = overlap4(last4, t[j].substr(0, 4));
            if (l > best_l || (l == best_l && (best_j == -1 || t[j] < t[best_j]))) {
                best_l = l;
                best_j = j;
            }
        }
        if (best_j == -1) {
            for (int j = 0; j < M; ++j) if (!used[j]) { best_j = j; break; }
        }
        used[best_j] = 1;
        ord.push_back(best_j);
        last4 = t[best_j].substr(1, 4);
    }

    // Build string S
    string S = t[ord[0]];
    last4 = S.substr(S.size() - 4);
    for (int idx = 1; idx < (int)ord.size(); ++idx) {
        const string &w = t[ord[idx]];
        int l = overlap4(last4, w.substr(0, 4));
        S += w.substr(l);
        last4 = S.substr(S.size() - 4);
    }
    if ((int)S.size() > 5000) S.resize(5000);

    int L = (int)S.size();
    if (L == 0) return 0;

    const int INF = 1e9;
    vector<vector<int>> dpSteps;
    dpSteps.reserve(L);

    vector<int> dpPrev(N * N, INF);
    dpPrev[si * N + sj] = 0;

    // Helper lambda to compute Manhattan transform via two 1D naive transforms
    auto manhattanTransform = [&](const vector<int> &f, vector<int> &h) {
        vector<int> m1(N * N, INF);
        // Row-wise
        for (int i = 0; i < N; ++i) {
            int base = i * N;
            for (int j = 0; j < N; ++j) {
                int best = INF;
                for (int c = 0; c < N; ++c) {
                    int val = f[base + c];
                    if (val >= INF / 2) continue;
                    int cand = val + abs(j - c);
                    if (cand < best) best = cand;
                }
                m1[base + j] = best;
            }
        }
        // Column-wise
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                int best = INF;
                for (int r = 0; r < N; ++r) {
                    int val = m1[r * N + j];
                    if (val >= INF / 2) continue;
                    int cand = val + abs(i - r);
                    if (cand < best) best = cand;
                }
                h[i * N + j] = best;
            }
        }
    };

    for (int step = 0; step < L; ++step) {
        vector<int> h(N * N, INF);
        manhattanTransform(dpPrev, h);
        vector<int> dpCurr(N * N, INF);
        char need = S[step];
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (A[i][j] == need) {
                    int v = h[i * N + j];
                    if (v < INF / 2) {
                        int cand = v + 1;
                        dpCurr[i * N + j] = cand;
                    }
                }
            }
        }
        dpSteps.push_back(dpCurr);
        dpPrev.swap(dpCurr);
    }

    // Choose end position with minimal cost
    int end_idx = -1, best_val = INF;
    for (int idx = 0; idx < N * N; ++idx) {
        if (dpSteps.back()[idx] < best_val) {
            best_val = dpSteps.back()[idx];
            end_idx = idx;
        }
    }

    // Backtrack path
    vector<pair<int,int>> path(L);
    int curr = end_idx;
    for (int step = L - 1; step >= 0; --step) {
        int ci = curr / N, cj = curr % N;
        path[step] = {ci, cj};
        if (step == 0) break;
        int target = dpSteps[step][curr] - 1; // equals dpPrev + |dist|
        int prev_idx = -1;
        const vector<int> &dpPrevStep = dpSteps[step - 1];
        for (int p = 0; p < N * N; ++p) {
            int val = dpPrevStep[p];
            if (val >= INF / 2) continue;
            int pi = p / N, pj = p % N;
            if (val + abs(pi - ci) + abs(pj - cj) == target) {
                prev_idx = p; break;
            }
        }
        if (prev_idx == -1) {
            int bestCand = INF, bestP = -1;
            for (int p = 0; p < N * N; ++p) {
                int val = dpPrevStep[p];
                if (val >= INF / 2) continue;
                int pi = p / N, pj = p % N;
                int cand = val + abs(pi - ci) + abs(pj - cj);
                if (cand < bestCand) { bestCand = cand; bestP = p; }
            }
            prev_idx = bestP;
        }
        curr = prev_idx;
    }

    // Output
    for (auto &pr : path) {
        cout << pr.first << ' ' << pr.second << '\n';
    }
    return 0;
}