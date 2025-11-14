#include <bits/stdc++.h>
using namespace std;

struct Step {
    bool hor; // true: horizontal, false: vertical
    int a, b; // for hor: (row=a, col=b) meaning edge h[a][b]; for ver: (row=a, col=b) meaning edge v[a][b]
};

static const int R = 30;
static const int C = 30;
static const int N = R * C;

inline int nodeid(int i, int j) { return i * C + j; }
inline void nodepos(int id, int &i, int &j) { i = id / C; j = id % C; }

double HR[R], VC[C];
double dH[R][C-1], dV[R-1][C];

inline double clampd(double x, double lo, double hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}
inline double getH(int i, int j) {
    // horizontal edge from (i,j) to (i,j+1)
    double w = HR[i] + dH[i][j];
    return clampd(w, 1000.0, 9000.0);
}
inline double getV(int i, int j) {
    // vertical edge from (i,j) to (i+1,j)
    double w = VC[j] + dV[i][j];
    return clampd(w, 1000.0, 9000.0);
}

struct DijkstraResult {
    string path;
    vector<Step> steps;
    double predSum;
};

DijkstraResult planPath(int si, int sj, int ti, int tj) {
    vector<double> dist(N, 1e100);
    vector<int> parent(N, -1);
    vector<char> pmove(N, 0);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;

    int s = nodeid(si, sj);
    int t = nodeid(ti, tj);
    dist[s] = 0.0;
    pq.push({0.0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == t) break;
        int ui, uj;
        nodepos(u, ui, uj);

        // neighbors: U, D, L, R
        // Up
        if (ui > 0) {
            double w = getV(ui-1, uj);
            int v = nodeid(ui-1, uj);
            double nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pmove[v] = 'U'; // move from parent to v is U
                pq.push({nd, v});
            }
        }
        // Down
        if (ui + 1 < R) {
            double w = getV(ui, uj);
            int v = nodeid(ui+1, uj);
            double nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pmove[v] = 'D';
                pq.push({nd, v});
            }
        }
        // Left
        if (uj > 0) {
            double w = getH(ui, uj-1);
            int v = nodeid(ui, uj-1);
            double nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pmove[v] = 'L';
                pq.push({nd, v});
            }
        }
        // Right
        if (uj + 1 < C) {
            double w = getH(ui, uj);
            int v = nodeid(ui, uj+1);
            double nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pmove[v] = 'R';
                pq.push({nd, v});
            }
        }
    }

    // Reconstruct path
    string path_rev;
    vector<Step> steps_rev;
    double predSum = 0.0;

    int cur = t;
    while (cur != s) {
        int p = parent[cur];
        if (p == -1) {
            // Fallback: if something goes wrong, use simple Manhattan path
            // This should not happen, but just in case.
            string fallback;
            vector<Step> fsteps;
            int ci = si, cj = sj;
            // Horizontal moves
            while (cj < tj) { fallback.push_back('R'); fsteps.push_back({true, ci, cj}); predSum += getH(ci, cj); cj++; }
            while (cj > tj) { fallback.push_back('L'); fsteps.push_back({true, ci, cj-1}); predSum += getH(ci, cj-1); cj--; }
            // Vertical moves
            while (ci < ti) { fallback.push_back('D'); fsteps.push_back({false, ci, cj}); predSum += getV(ci, cj); ci++; }
            while (ci > ti) { fallback.push_back('U'); fsteps.push_back({false, ci-1, cj}); predSum += getV(ci-1, cj); ci--; }
            return {fallback, fsteps, predSum};
        }
        int pi, pj, ci_, cj_;
        nodepos(p, pi, pj);
        nodepos(cur, ci_, cj_);
        char mv = pmove[cur];
        path_rev.push_back(mv);
        if (mv == 'U') {
            // from (pi,pj) to (ci_,cj_) where ci_ = pi-1
            steps_rev.push_back({false, ci_, cj_}); // v[ci_][cj_]
            predSum += getV(ci_, cj_);
        } else if (mv == 'D') {
            steps_rev.push_back({false, pi, pj}); // v[pi][pj]
            predSum += getV(pi, pj);
        } else if (mv == 'L') {
            steps_rev.push_back({true, ci_, cj_}); // h[ci_][cj_]
            predSum += getH(ci_, cj_);
        } else if (mv == 'R') {
            steps_rev.push_back({true, pi, pj}); // h[pi][pj]
            predSum += getH(pi, pj);
        }
        cur = p;
    }
    reverse(path_rev.begin(), path_rev.end());
    reverse(steps_rev.begin(), steps_rev.end());
    return {path_rev, steps_rev, predSum};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initialize estimates
    for (int i = 0; i < R; i++) HR[i] = 6000.0;
    for (int j = 0; j < C; j++) VC[j] = 6000.0;
    for (int i = 0; i < R; i++) for (int j = 0; j < C-1; j++) dH[i][j] = 0.0;
    for (int i = 0; i < R-1; i++) for (int j = 0; j < C; j++) dV[i][j] = 0.0;

    for (int k = 0; k < 1000; k++) {
        int si, sj, ti, tj;
        if (!(cin >> si >> sj >> ti >> tj)) break;

        // Plan path using current estimates
        DijkstraResult res = planPath(si, sj, ti, tj);
        string path = res.path;
        vector<Step> steps = res.steps;
        double pred = res.predSum;

        cout << path << '\n' << flush;

        long long obsLL;
        if (!(cin >> obsLL)) break;
        double obs = (double)obsLL;

        // Update model based on observation
        // Count features
        int L = (int)steps.size();
        if (L == 0) continue;

        vector<int> countRowH(R, 0);
        vector<int> countColV(C, 0);
        int cntH = 0, cntV = 0;
        for (auto &st : steps) {
            if (st.hor) {
                countRowH[st.a]++;
                cntH++;
            } else {
                countColV[st.b]++;
                cntV++;
            }
        }

        double diff = obs - pred;

        // Feature weighting schedule
        double progress = (double)k / 999.0;
        if (progress > 1.0) progress = 1.0;
        double K = 0.75 - 0.35 * progress; // correction fraction
        double rowAlpha = 1.0;
        double colAlpha = 1.0;
        double edgeAlpha = 0.05 + 0.25 * progress; // gradually emphasize residuals

        // Compute denom = sum alpha * ||x||^2
        double sRow2 = 0.0, sCol2 = 0.0;
        for (int i = 0; i < R; i++) sRow2 += (double)countRowH[i] * (double)countRowH[i];
        for (int j = 0; j < C; j++) sCol2 += (double)countColV[j] * (double)countColV[j];
        double denom = rowAlpha * sRow2 + colAlpha * sCol2 + edgeAlpha * (cntH + cntV);
        if (denom < 1e-9) denom = 1e-9;
        double baseRate = K / denom;

        // Apply updates
        for (int i = 0; i < R; i++) {
            if (countRowH[i] != 0) {
                HR[i] += baseRate * rowAlpha * diff * (double)countRowH[i];
                HR[i] = clampd(HR[i], 1000.0, 9000.0);
            }
        }
        for (int j = 0; j < C; j++) {
            if (countColV[j] != 0) {
                VC[j] += baseRate * colAlpha * diff * (double)countColV[j];
                VC[j] = clampd(VC[j], 1000.0, 9000.0);
            }
        }
        for (auto &st : steps) {
            if (st.hor) {
                dH[st.a][st.b] += baseRate * edgeAlpha * diff;
                dH[st.a][st.b] = clampd(dH[st.a][st.b], -4000.0, 4000.0);
            } else {
                dV[st.a][st.b] += baseRate * edgeAlpha * diff;
                dV[st.a][st.b] = clampd(dV[st.a][st.b], -4000.0, 4000.0);
            }
        }

        // Mild decay for residuals to avoid drift
        double decay = 0.9999;
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C-1; j++) dH[i][j] *= decay;
        }
        for (int i = 0; i < R-1; i++) {
            for (int j = 0; j < C; j++) dV[i][j] *= decay;
        }
    }
    return 0;
}