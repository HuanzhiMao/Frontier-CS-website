#include <bits/stdc++.h>
using namespace std;

struct Rect { int a,b,c,d; };

int N;
vector<int> X, Y;
vector<long long> R;
vector<Rect> ans;

static inline long long ceil_div_ll(long long a, long long b) {
    return (a + b - 1) / b;
}

struct BestSplit {
    bool feasible = false;
    long long cost = (1LL<<62);
    char axis = 'X';
    int L = -1;
};

BestSplit computeBestSplitX(const vector<int>& S, int x0, int y0, int x1, int y1) {
    BestSplit cand;
    int m = (int)S.size();
    if (m <= 1) return cand;
    vector<pair<int,int>> ord;
    ord.reserve(m);
    for (int id : S) ord.emplace_back(X[id], id);
    sort(ord.begin(), ord.end());
    if (ord.front().first == ord.back().first) return cand; // cannot split by X

    vector<long long> pref(m+1, 0);
    for (int i = 0; i < m; ++i) pref[i+1] = pref[i] + R[ord[i].second];

    int H = y1 - y0;
    int W = x1 - x0;
    for (int t = 1; t <= m-1; ++t) {
        int vLeft = ord[t-1].first;
        int vRight = ord[t].first;
        if (vLeft == vRight) continue;

        int mL = t, mR = m - t;
        long long minWLeft = ceil_div_ll(mL, (long long)H);
        long long minWRight = ceil_div_ll(mR, (long long)H);

        long long Lmin = max((long long)x0 + minWLeft, (long long)vLeft + 1);
        long long Lmax = min((long long)x1 - minWRight, (long long)vRight);
        if (Lmin > Lmax) continue;

        long long rLeft = pref[t];

        long long wTarget = (long long) llround((long double)rLeft / (long double)H);
        long long Lnearest = (long long)x0 + wTarget;

        long long Lchoose = Lmin;
        long long bestCostHere = (Lmin - x0) * (long long)H - rLeft;
        if (bestCostHere < 0) bestCostHere = -bestCostHere;

        auto evalCost = [&](long long Lc)->long long{
            long long area = (Lc - x0) * (long long)H;
            long long c = area - rLeft;
            return c >= 0 ? c : -c;
        };

        if (Lnearest >= Lmin && Lnearest <= Lmax) {
            long long c = evalCost(Lnearest);
            if (c < bestCostHere) { bestCostHere = c; Lchoose = Lnearest; }
        } else {
            long long c1 = evalCost(Lmin);
            long long c2 = evalCost(Lmax);
            if (c2 < bestCostHere || (c2 == bestCostHere && Lmax != Lchoose)) { bestCostHere = c2; Lchoose = Lmax; }
        }

        if (!cand.feasible || bestCostHere < cand.cost) {
            cand.feasible = true;
            cand.cost = bestCostHere;
            cand.axis = 'X';
            cand.L = (int)Lchoose;
        }
    }
    return cand;
}

BestSplit computeBestSplitY(const vector<int>& S, int x0, int y0, int x1, int y1) {
    BestSplit cand;
    int m = (int)S.size();
    if (m <= 1) return cand;
    vector<pair<int,int>> ord;
    ord.reserve(m);
    for (int id : S) ord.emplace_back(Y[id], id);
    sort(ord.begin(), ord.end());
    if (ord.front().first == ord.back().first) return cand; // cannot split by Y

    vector<long long> pref(m+1, 0);
    for (int i = 0; i < m; ++i) pref[i+1] = pref[i] + R[ord[i].second];

    int W = x1 - x0;
    int H = y1 - y0;
    for (int t = 1; t <= m-1; ++t) {
        int vLeft = ord[t-1].first;
        int vRight = ord[t].first;
        if (vLeft == vRight) continue;

        int mL = t, mR = m - t;
        long long minHLeft = ceil_div_ll(mL, (long long)W);
        long long minHRight = ceil_div_ll(mR, (long long)W);

        long long Lmin = max((long long)y0 + minHLeft, (long long)vLeft + 1);
        long long Lmax = min((long long)y1 - minHRight, (long long)vRight);
        if (Lmin > Lmax) continue;

        long long rLeft = pref[t];

        long long hTarget = (long long) llround((long double)rLeft / (long double)W);
        long long Lnearest = (long long)y0 + hTarget;

        long long Lchoose = Lmin;
        long long bestCostHere = (Lmin - y0) * (long long)W - rLeft;
        if (bestCostHere < 0) bestCostHere = -bestCostHere;

        auto evalCost = [&](long long Lc)->long long{
            long long area = (Lc - y0) * (long long)W;
            long long c = area - rLeft;
            return c >= 0 ? c : -c;
        };

        if (Lnearest >= Lmin && Lnearest <= Lmax) {
            long long c = evalCost(Lnearest);
            if (c < bestCostHere) { bestCostHere = c; Lchoose = Lnearest; }
        } else {
            long long c1 = evalCost(Lmin);
            long long c2 = evalCost(Lmax);
            if (c2 < bestCostHere || (c2 == bestCostHere && Lmax != Lchoose)) { bestCostHere = c2; Lchoose = Lmax; }
        }

        if (!cand.feasible || bestCostHere < cand.cost) {
            cand.feasible = true;
            cand.cost = bestCostHere;
            cand.axis = 'Y';
            cand.L = (int)Lchoose;
        }
    }
    return cand;
}

void fallback_assign(const vector<int>& S) {
    for (int id : S) {
        int a = X[id], b = Y[id];
        int c = a + 1, d = b + 1;
        if (c > 10000) c = 10000;
        if (d > 10000) d = 10000;
        if (a < 0) a = 0;
        if (b < 0) b = 0;
        if (c <= a) c = min(10000, a + 1);
        if (d <= b) d = min(10000, b + 1);
        ans[id] = {a, b, c, d};
    }
}

void solveRec(int x0, int y0, int x1, int y1, const vector<int>& S) {
    int m = (int)S.size();
    if (m == 0) return;
    if (m == 1) {
        int id = S[0];
        ans[id] = {x0, y0, x1, y1};
        return;
    }
    BestSplit sx = computeBestSplitX(S, x0, y0, x1, y1);
    BestSplit sy = computeBestSplitY(S, x0, y0, x1, y1);

    bool hasX = sx.feasible;
    bool hasY = sy.feasible;

    if (!hasX && !hasY) {
        // Fallback to 1x1 around each point
        fallback_assign(S);
        return;
    }

    BestSplit chosen;
    if (hasX && hasY) {
        if (sx.cost < sy.cost) chosen = sx;
        else if (sy.cost < sx.cost) chosen = sy;
        else {
            // tie-break by larger dimension
            int W = x1 - x0, H = y1 - y0;
            chosen = (W >= H) ? sx : sy;
        }
    } else {
        chosen = hasX ? sx : sy;
    }

    vector<int> Lset, Rset;
    Lset.reserve(m);
    Rset.reserve(m);
    if (chosen.axis == 'X') {
        int L = chosen.L;
        for (int id : S) {
            if (X[id] < L) Lset.push_back(id);
            else Rset.push_back(id);
        }
        // Safety: ensure both sides non-empty, otherwise fallback
        if (Lset.empty() || Rset.empty()) {
            fallback_assign(S);
            return;
        }
        solveRec(x0, y0, L, y1, Lset);
        solveRec(L, y0, x1, y1, Rset);
    } else {
        int L = chosen.L;
        for (int id : S) {
            if (Y[id] < L) Lset.push_back(id);
            else Rset.push_back(id);
        }
        if (Lset.empty() || Rset.empty()) {
            fallback_assign(S);
            return;
        }
        solveRec(x0, y0, x1, L, Lset);
        solveRec(x0, L, x1, y1, Rset);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> N)) {
        return 0;
    }
    X.resize(N);
    Y.resize(N);
    R.resize(N);
    ans.assign(N, {0,0,1,1});
    for (int i = 0; i < N; ++i) {
        int xi, yi;
        long long ri;
        cin >> xi >> yi >> ri;
        X[i] = xi; Y[i] = yi; R[i] = ri;
    }
    vector<int> idx(N);
    iota(idx.begin(), idx.end(), 0);
    solveRec(0, 0, 10000, 10000, idx);

    for (int i = 0; i < N; ++i) {
        // Ensure valid positive area
        int a = ans[i].a, b = ans[i].b, c = ans[i].c, d = ans[i].d;
        if (a < 0) a = 0;
        if (b < 0) b = 0;
        if (c > 10000) c = 10000;
        if (d > 10000) d = 10000;
        if (c <= a) c = min(10000, a + 1);
        if (d <= b) d = min(10000, b + 1);
        cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    }
    return 0;
}