#include <bits/stdc++.h>
using namespace std;

struct Pt { int x, y, w; };
struct Rect { int x1, y1, x2, y2; };

static inline int countDiff(const vector<Pt>& pts, const Rect& r) {
    int diff = 0;
    for (const auto& p : pts) {
        if (r.x1 <= p.x && p.x <= r.x2 && r.y1 <= p.y && p.y <= r.y2) diff += p.w;
    }
    return diff;
}

static inline int colWeight(const vector<vector<pair<int,int>>>& xBuckets, int xi, int y1, int y2) {
    if (xi < 0 || xi > 100000) return INT_MIN/4;
    const auto& vec = xBuckets[xi];
    int s = 0;
    for (auto &pr : vec) {
        int y = pr.first, w = pr.second;
        if (y1 <= y && y <= y2) s += w;
    }
    return s;
}

static inline int rowWeight(const vector<vector<pair<int,int>>>& yBuckets, int yi, int x1, int x2) {
    if (yi < 0 || yi > 100000) return INT_MIN/4;
    const auto& vec = yBuckets[yi];
    int s = 0;
    for (auto &pr : vec) {
        int x = pr.first, w = pr.second;
        if (x1 <= x && x <= x2) s += w;
    }
    return s;
}

static Rect bestRectGrid(const vector<Pt>& pts, int G) {
    Rect res{0,0,1,1};
    if (G <= 0) return res;
    if (100000 % G != 0) return res;
    int S = 100000 / G;
    vector<int> grid(G * G, 0);
    for (const auto& p : pts) {
        int ix = min(p.x / S, G - 1);
        int iy = min(p.y / S, G - 1);
        grid[ix * G + iy] += p.w;
    }
    int bestSum = INT_MIN;
    int bestL = 0, bestR = 0, bestT = 0, bestB = 0;

    vector<int> sums(G, 0);
    for (int L = 0; L < G; ++L) {
        fill(sums.begin(), sums.end(), 0);
        for (int R = L; R < G; ++R) {
            for (int y = 0; y < G; ++y) {
                sums[y] += grid[R * G + y];
            }
            int currSum = 0, currStart = 0;
            for (int y = 0; y < G; ++y) {
                if (currSum <= 0) {
                    currSum = sums[y];
                    currStart = y;
                } else {
                    currSum += sums[y];
                }
                if (currSum > bestSum) {
                    bestSum = currSum;
                    bestL = L;
                    bestR = R;
                    bestT = currStart;
                    bestB = y;
                }
            }
        }
    }
    int x1 = bestL * S;
    int x2 = (bestR == G - 1 ? 100000 : (bestR + 1) * S - 1);
    int y1 = bestT * S;
    int y2 = (bestB == G - 1 ? 100000 : (bestB + 1) * S - 1);
    if (x2 <= x1) x2 = min(100000, x1 + 1);
    if (y2 <= y1) y2 = min(100000, y1 + 1);
    res = {x1, y1, x2, y2};
    return res;
}

static Rect improveRect(const vector<Pt>& pts, Rect r, const vector<vector<pair<int,int>>>& xBuckets, const vector<vector<pair<int,int>>>& yBuckets, int maxSteps = 5000) {
    int diff = countDiff(pts, r);
    for (int step = 0; step < maxSteps; ++step) {
        int bestDelta = 0;
        int moveType = -1;
        // 0: expand left, 1: shrink left, 2: expand right, 3: shrink right, 4: expand down, 5: shrink down, 6: expand up, 7: shrink up
        // Left
        if (r.x1 > 0) {
            int d = colWeight(xBuckets, r.x1 - 1, r.y1, r.y2);
            if (d > bestDelta) { bestDelta = d; moveType = 0; }
        }
        if (r.x1 + 1 <= r.x2) {
            int d = -colWeight(xBuckets, r.x1, r.y1, r.y2);
            if (d > bestDelta) { bestDelta = d; moveType = 1; }
        }
        // Right
        if (r.x2 < 100000) {
            int d = colWeight(xBuckets, r.x2 + 1, r.y1, r.y2);
            if (d > bestDelta) { bestDelta = d; moveType = 2; }
        }
        if (r.x2 - 1 >= r.x1) {
            int d = -colWeight(xBuckets, r.x2, r.y1, r.y2);
            if (d > bestDelta) { bestDelta = d; moveType = 3; }
        }
        // Down
        if (r.y1 > 0) {
            int d = rowWeight(yBuckets, r.y1 - 1, r.x1, r.x2);
            if (d > bestDelta) { bestDelta = d; moveType = 4; }
        }
        if (r.y1 + 1 <= r.y2) {
            int d = -rowWeight(yBuckets, r.y1, r.x1, r.x2);
            if (d > bestDelta) { bestDelta = d; moveType = 5; }
        }
        // Up
        if (r.y2 < 100000) {
            int d = rowWeight(yBuckets, r.y2 + 1, r.x1, r.x2);
            if (d > bestDelta) { bestDelta = d; moveType = 6; }
        }
        if (r.y2 - 1 >= r.y1) {
            int d = -rowWeight(yBuckets, r.y2, r.x1, r.x2);
            if (d > bestDelta) { bestDelta = d; moveType = 7; }
        }

        if (bestDelta <= 0 || moveType == -1) break;
        switch (moveType) {
            case 0: r.x1--; break;
            case 1: r.x1++; break;
            case 2: r.x2++; break;
            case 3: r.x2--; break;
            case 4: r.y1--; break;
            case 5: r.y1++; break;
            case 6: r.y2++; break;
            case 7: r.y2--; break;
        }
        diff += bestDelta;
        if (r.x2 <= r.x1) r.x2 = r.x1 + 1;
        if (r.y2 <= r.y1) r.y2 = r.y1 + 1;
        if (r.x2 > 100000) r.x2 = 100000;
        if (r.y2 > 100000) r.y2 = 100000;
        if (r.x1 < 0) r.x1 = 0;
        if (r.y1 < 0) r.y1 = 0;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) {
        return 0;
    }
    vector<Pt> pts;
    pts.reserve(2 * N);
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
        pts.push_back({x, y, +1});
    }
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
        pts.push_back({x, y, -1});
    }

    // Prepare buckets by exact coordinates for quick per-column/row contributions
    vector<vector<pair<int,int>>> xBuckets(100001);
    vector<vector<pair<int,int>>> yBuckets(100001);
    xBuckets.shrink_to_fit();
    yBuckets.shrink_to_fit();
    for (const auto& p : pts) {
        xBuckets[p.x].push_back({p.y, p.w});
        yBuckets[p.y].push_back({p.x, p.w});
    }

    // Candidate G values that divide 100000
    vector<int> Gs = {50, 80, 100, 125, 160, 200, 250, 40, 32, 20};
    Rect best = {0, 0, 1, 1};
    int bestDiff = countDiff(pts, best);

    for (int G : Gs) {
        if (G <= 0) continue;
        if (100000 % G != 0) continue;
        Rect r = bestRectGrid(pts, G);
        int d = countDiff(pts, r);
        if (d > bestDiff) {
            bestDiff = d;
            best = r;
        }
    }

    // Improve with local adjustments
    Rect improved = improveRect(pts, best, xBuckets, yBuckets, 5000);
    int improvedDiff = countDiff(pts, improved);
    if (improvedDiff > bestDiff) {
        bestDiff = improvedDiff;
        best = improved;
    }

    // Fallback small rectangles near corners to avoid fish if necessary
    vector<Rect> fallbacks = {
        {0, 0, 1, 1},
        {0, 99999, 1, 100000},
        {99999, 0, 100000, 1},
        {99999, 99999, 100000, 100000},
        {50000, 50000, 50001, 50001}
    };
    for (auto &fb : fallbacks) {
        int d = countDiff(pts, fb);
        if (d > bestDiff) {
            bestDiff = d;
            best = fb;
        }
    }

    // Ensure valid rectangle
    if (best.x2 <= best.x1) best.x2 = min(100000, best.x1 + 1);
    if (best.y2 <= best.y1) best.y2 = min(100000, best.y1 + 1);

    // Output polygon (rectangle)
    cout << 4 << "\n";
    cout << best.x1 << " " << best.y1 << "\n";
    cout << best.x2 << " " << best.y1 << "\n";
    cout << best.x2 << " " << best.y2 << "\n";
    cout << best.x1 << " " << best.y2 << "\n";
    return 0;
}