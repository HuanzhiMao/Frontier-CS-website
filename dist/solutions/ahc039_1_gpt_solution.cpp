#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y, w;
};

struct MaxSeg {
    long long sum;
    int l, r;
    MaxSeg(long long s=-9e18, int L=0, int R=-1): sum(s), l(L), r(R) {}
};

MaxSeg bestSegmentWithWidthConstraints(const vector<int>& arr, const vector<int>& X, int widthMin, int widthMax) {
    int K = (int)arr.size();
    if (K == 0 || widthMax < widthMin) return MaxSeg();
    vector<long long> S(K+1, 0);
    for (int i = 0; i < K; ++i) S[i+1] = S[i] + arr[i];

    vector<int> lMin(K), lMax(K);
    int pMin = 0;
    int pMax = 0;
    for (int r = 0; r < K; ++r) {
        int xmin = X[r] - widthMax;
        while (pMin < K && X[pMin] < xmin) ++pMin;
        int xmax = X[r] - widthMin;
        while (pMax < K && X[pMax] <= xmax) ++pMax;
        lMin[r] = pMin;
        lMax[r] = min(r, pMax - 1);
    }

    deque<pair<int,long long>> dq;
    int ptr = 0;
    MaxSeg res;
    for (int r = 0; r < K; ++r) {
        int upto = lMax[r];
        while (ptr <= upto) {
            long long val = S[ptr];
            while (!dq.empty() && dq.back().second >= val) dq.pop_back();
            dq.emplace_back(ptr, val);
            ++ptr;
        }
        while (!dq.empty() && dq.front().first < lMin[r]) dq.pop_front();
        if (!dq.empty()) {
            long long cur = S[r+1] - dq.front().second;
            if (cur > res.sum) {
                res.sum = cur;
                res.l = dq.front().first;
                res.r = r;
            }
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) {
        return 0;
    }
    vector<Point> pts;
    pts.reserve(2*N);
    vector<int> xs, ys;
    xs.reserve(2*N);
    ys.reserve(2*N);
    for (int i = 0; i < 2*N; ++i) {
        int x, y;
        cin >> x >> y;
        int w = (i < N) ? 1 : -1;
        pts.push_back({x, y, w});
        xs.push_back(x);
        ys.push_back(y);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int Kx = (int)xs.size();
    int Ky = (int)ys.size();

    // Group Y into L bands
    int L = min(60, Ky); // adjustable
    if (L <= 0) L = 1;
    vector<int> yStart(L), yEnd(L);
    for (int g = 0; g < L; ++g) {
        int s = (long long)g * Ky / L;
        int e = (long long)(g+1) * Ky / L - 1;
        if (e < s) e = s;
        yStart[g] = s;
        yEnd[g] = e;
    }

    // Map each point to x-index and y-group
    vector<vector<pair<int,int>>> groups(L); // (x-index, weight)
    groups.shrink_to_fit();
    for (const auto& p : pts) {
        int xi = (int)(lower_bound(xs.begin(), xs.end(), p.x) - xs.begin());
        int yi = (int)(lower_bound(ys.begin(), ys.end(), p.y) - ys.begin());
        int g = min(L-1, (int)((long long)yi * L / Ky));
        groups[g].push_back({xi, p.w});
    }

    // Fallback rectangle
    int best_xL = 0, best_xR = 100000, best_yB = 0, best_yT = 100000;
    long long bestScore = 0;

    // Try to find a better rectangle
    vector<int> arr(Kx, 0);
    for (int B = 0; B < L; ++B) {
        fill(arr.begin(), arr.end(), 0);
        for (int T = B; T < L; ++T) {
            // add group T contributions
            for (auto &pr : groups[T]) {
                arr[pr.first] += pr.second;
            }
            int yBottom = ys[yStart[B]];
            int yTop = ys[yEnd[T]];
            int height = yTop - yBottom;
            if (height <= 0) continue; // ensure positive height to avoid degenerate polygon
            int widthMax = 200000 - height;
            if (widthMax < 1) continue;
            MaxSeg seg = bestSegmentWithWidthConstraints(arr, xs, 1, widthMax);
            if (seg.r >= seg.l && seg.sum > bestScore) {
                int xLeft = xs[seg.l];
                int xRight = xs[seg.r];
                if (xRight - xLeft <= 0) continue; // ensure positive width
                bestScore = seg.sum;
                best_xL = xLeft;
                best_xR = xRight;
                best_yB = yBottom;
                best_yT = yTop;
            }
        }
    }

    // Ensure vertices are distinct and within bounds; adjust minimally if needed
    if (best_xL == best_xR) {
        if (best_xL > 0) --best_xL;
        else if (best_xR < 100000) ++best_xR;
        else { best_xL = 0; best_xR = 100000; }
    }
    if (best_yB == best_yT) {
        if (best_yB > 0) --best_yB;
        else if (best_yT < 100000) ++best_yT;
        else { best_yB = 0; best_yT = 100000; }
    }
    // Ensure perimeter constraint
    int width = best_xR - best_xL;
    int height = best_yT - best_yB;
    if (width < 1 || height < 1 || 2LL*(width + height) > 400000) {
        // fallback
        best_xL = 0; best_yB = 0; best_xR = 100000; best_yT = 100000;
    }

    cout << 4 << '\n';
    cout << best_xL << ' ' << best_yB << '\n';
    cout << best_xR << ' ' << best_yB << '\n';
    cout << best_xR << ' ' << best_yT << '\n';
    cout << best_xL << ' ' << best_yT << '\n';

    return 0;
}