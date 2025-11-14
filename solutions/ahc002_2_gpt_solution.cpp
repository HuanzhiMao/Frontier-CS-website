#include <bits/stdc++.h>
using namespace std;

static const int N = 50;
static const int H = N, W = N;
static const int DX[4] = {-1, 1, 0, 0};
static const int DY[4] = {0, 0, -1, 1};
static const char DIRC[4] = {'U', 'D', 'L', 'R'};

struct RNG {
    uint64_t x;
    RNG(uint64_t seed=88172645463393265ull) : x(seed) {}
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int randint(int a, int b) { // [a,b)
        return (int)(next() % (uint64_t)(b - a)) + a;
    }
    inline double drand() {
        return (next() & ((1ull<<53)-1)) * (1.0 / (double)(1ull<<53));
    }
};

struct Param {
    double wDeg1;
    double w2;
    double wDeg2;
    double trapPenalty;
    double noise;
};

struct Result {
    string path;
    long long score;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int si, sj;
    if (!(cin >> si >> sj)) return 0;
    vector<int> tflat(H*W);
    vector<int> pflat(H*W);
    int M = 0;

    for (int i=0;i<H;i++) {
        for (int j=0;j<W;j++) {
            int v; cin >> v;
            tflat[i*W+j] = v;
            if (v+1 > M) M = v+1;
        }
    }
    for (int i=0;i<H;i++) {
        for (int j=0;j<W;j++) {
            int v; cin >> v;
            pflat[i*W+j] = v;
        }
    }

    // Precompute neighbors
    vector<array<int,4>> nb(H*W);
    for (int i=0;i<H;i++) {
        for (int j=0;j<W;j++) {
            int id = i*W + j;
            for (int d=0; d<4; d++) {
                int ni = i + DX[d], nj = j + DY[d];
                if (0 <= ni && ni < H && 0 <= nj && nj < W) nb[id][d] = ni*W + nj;
                else nb[id][d] = -1;
            }
        }
    }

    // Visited versioning for tiles
    vector<int> visVer(M, 0);
    int tag = 1;

    auto countDeg = [&](int cell, int curTag, int exclude1, int exclude2)->int{
        int tid_here = tflat[cell];
        int deg = 0;
        auto &arr = nb[cell];
        for (int d=0; d<4; d++) {
            int to = arr[d];
            if (to < 0) continue;
            int tid_to = tflat[to];
            if (tid_to == tid_here) continue; // cannot move within same tile
            if (visVer[tid_to] == curTag) continue;
            if (tid_to == exclude1 || tid_to == exclude2) continue;
            deg++;
        }
        return deg;
    };

    auto best2Val = [&](int cell, int curTag, double wDeg2, int exclude1)->double{
        // among legal next moves from 'cell' (after having visited exclude1 tile),
        // choose max of p[next] + wDeg2 * deg3 (where deg3 excludes both exclude1 and tid2)
        int tid_here = tflat[cell];
        double bestv = 0.0;
        auto &arr = nb[cell];
        for (int d=0; d<4; d++) {
            int to = arr[d];
            if (to < 0) continue;
            int tid_to = tflat[to];
            if (tid_to == tid_here) continue;
            if (visVer[tid_to] == curTag) continue;
            if (tid_to == exclude1) continue;
            int deg3 = countDeg(to, curTag, exclude1, tid_to);
            double v = (double)pflat[to] + wDeg2 * (double)deg3;
            if (v > bestv) bestv = v;
        }
        return bestv;
    };

    // Timer
    auto startTime = chrono::high_resolution_clock::now();
    auto elapsed_ms = [&](){
        return (int)chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime).count();
    };

    // Parameter candidates (a small set; randomized selection among them)
    vector<Param> params;
    vector<double> wDeg1s = {0.10, 0.20, 0.30, 0.40};
    vector<double> w2s = {0.0, 0.25, 0.5, 0.75};
    vector<double> wDeg2s = {0.05, 0.10, 0.15};
    vector<double> noises = {0.0, 0.4, 0.8, 1.5, 3.0};
    for (double a: wDeg1s) for (double b: w2s) for (double c: wDeg2s) for (double n: noises) {
        Param pm;
        pm.wDeg1 = a;
        pm.w2 = b;
        pm.wDeg2 = c;
        pm.trapPenalty = -3.0; // small penalty to avoid dead-ends when alternatives exist
        pm.noise = n;
        params.push_back(pm);
    }

    RNG rng(chrono::high_resolution_clock::now().time_since_epoch().count());

    // attempt runner
    auto run_attempt = [&](const Param &pm)->Result{
        int curTag = ++tag;
        int pos = si*W + sj;
        long long score = pflat[pos];
        visVer[tflat[pos]] = curTag;
        string path;
        path.reserve(H*W);

        // iterate until stuck
        while (true) {
            struct Cand {
                int dir;
                int to;
                int deg2;
                double baseVal;
                double val;
            };
            vector<Cand> cands;
            cands.reserve(4);
            auto &arr = nb[pos];
            int curTid = tflat[pos];

            for (int d=0; d<4; d++) {
                int to = arr[d];
                if (to < 0) continue;
                int tid_to = tflat[to];
                if (tid_to == curTid) continue; // cannot step within same tile
                if (visVer[tid_to] == curTag) continue; // already visited tile
                // compute features
                int deg2 = countDeg(to, curTag, tid_to, -1); // after stepping onto tid_to, exclude stepping into itself
                double best2 = 0.0;
                if (pm.w2 != 0.0 || pm.wDeg2 != 0.0) {
                    best2 = best2Val(to, curTag, pm.wDeg2, tid_to);
                }
                double val = (double)pflat[to] + pm.wDeg1 * (double)deg2 + pm.w2 * best2;
                double noise = (pm.noise > 0 ? (rng.drand() - 0.5) * pm.noise : 0.0);
                Cand c{d, to, deg2, val, val + noise};
                cands.push_back(c);
            }

            if (cands.empty()) break;

            bool hasAlt = false;
            for (auto &c : cands) if (c.deg2 > 0) { hasAlt = true; break; }
            if (hasAlt) {
                for (auto &c : cands) if (c.deg2 == 0) c.val += pm.trapPenalty;
            }

            // Choose best candidate
            int best_k = 0;
            double best_val = cands[0].val;
            for (int k=1; k<(int)cands.size(); k++) {
                if (cands[k].val > best_val) {
                    best_val = cands[k].val;
                    best_k = k;
                }
            }

            // apply move
            int d = cands[best_k].dir;
            int to = cands[best_k].to;
            path.push_back(DIRC[d]);
            score += pflat[to];
            visVer[tflat[to]] = curTag;
            pos = to;
        }

        return Result{path, score};
    };

    // Initial attempt with a solid parameter
    Param initialParam{0.30, 0.50, 0.10, -3.0, 0.5};
    Result best = run_attempt(initialParam);
    string bestPath = best.path;
    long long bestScore = best.score;

    // Iterative improvement within time limit (~1.9s)
    const int TIME_LIMIT_MS = 1950;
    int iter = 0;
    while (elapsed_ms() < TIME_LIMIT_MS) {
        const Param &pm = params[rng.randint(0, (int)params.size())];
        Result r = run_attempt(pm);
        if (r.score > bestScore) {
            bestScore = r.score;
            bestPath = r.path;
        }
        iter++;
    }

    cout << bestPath << '\n';
    return 0;
}