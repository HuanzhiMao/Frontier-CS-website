#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct SumMask {
    long long sum;
    uint32_t mask;
    bool operator<(const SumMask& other) const {
        return sum < other.sum;
    }
};

static inline long long absl(long long x){ return x >= 0 ? x : -x; }

static void genSums(const vector<long long>& vals, vector<SumMask>& out) {
    int m = (int)vals.size();
    int sz = 1 << m;
    out.resize(sz);
    out[0] = {0LL, 0u};
    for (int s = 1; s < sz; ++s) {
        int b = __builtin_ctz(s);
        int prev = s & (s - 1);
        out[s].sum = out[prev].sum + vals[b];
        out[s].mask = (uint32_t)s;
    }
}

static void applyMaskFlips(const vector<int>& idxL, uint32_t maskL, const vector<int>& idxR, uint32_t maskR,
                           vector<uint8_t>& x, const vector<long long>& a, long long& currSum) {
    for (int j = 0; j < (int)idxL.size(); ++j) {
        if ((maskL >> j) & 1u) {
            int i = idxL[j];
            if (x[i]) currSum -= a[i]; else currSum += a[i];
            x[i] ^= 1;
        }
    }
    for (int j = 0; j < (int)idxR.size(); ++j) {
        if ((maskR >> j) & 1u) {
            int i = idxR[j];
            if (x[i]) currSum -= a[i]; else currSum += a[i];
            x[i] ^= 1;
        }
    }
}

static void hillClimb1(vector<uint8_t>& x, const vector<long long>& a, long long T, long long& sum) {
    bool improved = true;
    while (improved) {
        improved = false;
        long long bestErr = absl(sum - T);
        for (int i = 0; i < (int)a.size(); ++i) {
            long long ns = sum + (x[i] ? -a[i] : a[i]);
            long long e = absl(ns - T);
            if (e < bestErr) {
                x[i] ^= 1;
                sum = ns;
                bestErr = e;
                improved = true;
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long T;
    if(!(cin>>n>>T)) return 0;
    vector<long long> a(n);
    for(int i=0;i<n;i++) cin>>a[i];

    auto start = chrono::high_resolution_clock::now();
    auto elapsed = [&](){
        return chrono::duration<double>(chrono::high_resolution_clock::now() - start).count();
    };
    const double TIME_LIMIT = 1.90;

    // Initial solutions
    vector<int> ordDesc(n), ordAsc(n);
    iota(ordDesc.begin(), ordDesc.end(), 0);
    sort(ordDesc.begin(), ordDesc.end(), [&](int i, int j){ return a[i] > a[j]; });
    ordAsc = ordDesc;
    reverse(ordAsc.begin(), ordAsc.end());

    auto greedyWithOrder = [&](const vector<int>& order)->pair<vector<uint8_t>, long long>{
        vector<uint8_t> x(n, 0);
        long long sum = 0;
        for (int idx : order) {
            long long d0 = absl(sum - T);
            long long d1 = absl(sum + a[idx] - T);
            if (d1 <= d0) {
                x[idx] = 1;
                sum += a[idx];
            }
        }
        return {x, sum};
    };

    vector<uint8_t> bestX(n, 0), currX(n,0);
    long long bestSum = 0, currSum = 0;
    long long bestErr = absl(T - 0);

    // Greedy descending
    {
        auto [x, s] = greedyWithOrder(ordDesc);
        long long e = absl(s - T);
        if (e < bestErr) { bestX = x; bestSum = s; bestErr = e; }
        if (elapsed() > TIME_LIMIT) {
            for (int i=0;i<n;i++) cout << (int)bestX[i];
            cout << '\n';
            return 0;
        }
    }
    // Greedy ascending
    {
        auto [x, s] = greedyWithOrder(ordAsc);
        long long e = absl(s - T);
        if (e < bestErr) { bestX = x; bestSum = s; bestErr = e; }
        if (elapsed() > TIME_LIMIT) {
            for (int i=0;i<n;i++) cout << (int)bestX[i];
            cout << '\n';
            return 0;
        }
    }

    // Meet-in-the-middle on top k1 values
    int k1 = min(n, 36);
    if (k1 > 0 && elapsed() < TIME_LIMIT * 0.6) {
        vector<int> heavyIdx(ordDesc.begin(), ordDesc.begin() + k1);
        int lsz = k1 / 2;
        int rsz = k1 - lsz;
        vector<int> leftIdx(heavyIdx.begin(), heavyIdx.begin() + lsz);
        vector<int> rightIdx(heavyIdx.begin() + lsz, heavyIdx.begin() + k1);

        vector<long long> leftVals(lsz), rightVals(rsz);
        for (int i = 0; i < lsz; ++i) leftVals[i] = a[leftIdx[i]];
        for (int i = 0; i < rsz; ++i) rightVals[i] = a[rightIdx[i]];
        vector<SumMask> L, R;
        genSums(leftVals, L);
        genSums(rightVals, R);
        sort(R.begin(), R.end());

        long long localBestErr = (1LL<<62);
        uint32_t bestLm = 0, bestRm = 0;
        for (const auto& lm : L) {
            long long want = T - lm.sum;
            auto it = lower_bound(R.begin(), R.end(), SumMask{want, 0u});
            if (it != R.end()) {
                long long s = lm.sum + it->sum;
                long long e = absl(s - T);
                if (e < localBestErr) { localBestErr = e; bestLm = lm.mask; bestRm = it->mask; }
            }
            if (it != R.begin()) {
                --it;
                long long s = lm.sum + it->sum;
                long long e = absl(s - T);
                if (e < localBestErr) { localBestErr = e; bestLm = lm.mask; bestRm = it->mask; }
            }
        }
        vector<uint8_t> x(n, 0);
        long long ssum = 0;
        for (int i = 0; i < lsz; ++i) if ((bestLm>>i)&1u) { x[leftIdx[i]] = 1; ssum += a[leftIdx[i]]; }
        for (int i = 0; i < rsz; ++i) if ((bestRm>>i)&1u) { x[rightIdx[i]] = 1; ssum += a[rightIdx[i]]; }
        // Greedy include remaining
        for (int idx : ordDesc) {
            if (find(heavyIdx.begin(), heavyIdx.end(), idx) != heavyIdx.end()) continue;
            long long d0 = absl(ssum - T);
            long long d1 = absl(ssum + a[idx] - T);
            if (d1 < d0) { x[idx] = 1; ssum += a[idx]; }
        }
        long long e = absl(ssum - T);
        if (e < bestErr) { bestX = x; bestSum = ssum; bestErr = e; }
    }

    // Start from best known
    currX = bestX; currSum = bestSum;
    hillClimb1(currX, a, T, currSum);
    if (absl(currSum - T) < bestErr) { bestErr = absl(currSum - T); bestX = currX; bestSum = currSum; }

    // Large Neighborhood Search with MiM on random/closest K indices
    std::mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    vector<int> allIdx(n); iota(allIdx.begin(), allIdx.end(), 0);

    int iter = 0;
    while (elapsed() < TIME_LIMIT) {
        ++iter;
        long long residual = T - currSum;
        if (residual == 0) break;

        int K = min(n, (iter % 5 == 0 ? 24 : 28)); // occasionally smaller to vary
        // Build candidates by closeness
        vector<pair<long long,int>> cand;
        cand.reserve(n);
        for (int i = 0; i < n; ++i) {
            long long delta = currX[i] ? -a[i] : a[i];
            long long score = absl(residual - delta);
            cand.emplace_back(score, i);
        }
        int M = min(n, max(K*2, K)); // top 2K candidates if possible
        nth_element(cand.begin(), cand.begin()+M-1, cand.end(), [](auto& x, auto& y){ return x.first < y.first; });
        cand.resize(M);
        sort(cand.begin(), cand.end(), [](auto& x, auto& y){ return x.first < y.first; });

        vector<int> idx;
        idx.reserve(K);
        for (int i = 0; i < min(K, M); ++i) idx.push_back(cand[i].second);
        // if not enough, fill with random
        while ((int)idx.size() < K) {
            int v = rng() % n;
            bool used=false; for(int u:idx){ if(u==v){ used=true; break; } }
            if(!used) idx.push_back(v);
        }

        int h = K / 2;
        vector<int> idxL(idx.begin(), idx.begin() + h);
        vector<int> idxR(idx.begin() + h, idx.end());

        vector<long long> valsL(h), valsR((int)idxR.size());
        for (int i = 0; i < h; ++i) valsL[i] = currX[idxL[i]] ? -a[idxL[i]] : a[idxL[i]];
        for (int i = 0; i < (int)idxR.size(); ++i) valsR[i] = currX[idxR[i]] ? -a[idxR[i]] : a[idxR[i]];

        vector<SumMask> L, R;
        genSums(valsL, L);
        genSums(valsR, R);
        sort(R.begin(), R.end());

        long long baseErr = absl(currSum - T);
        long long bestImpErr = baseErr;
        uint32_t bestLm = 0, bestRm = 0;

        long long want = residual;
        for (const auto& lm : L) {
            long long need = want - lm.sum;
            auto it = lower_bound(R.begin(), R.end(), SumMask{need, 0u});
            if (it != R.end()) {
                long long newSum = currSum + lm.sum + it->sum;
                long long e = absl(newSum - T);
                if (e < bestImpErr) { bestImpErr = e; bestLm = lm.mask; bestRm = it->mask; }
            }
            if (it != R.begin()) {
                --it;
                long long newSum = currSum + lm.sum + it->sum;
                long long e = absl(newSum - T);
                if (e < bestImpErr) { bestImpErr = e; bestLm = lm.mask; bestRm = it->mask; }
            }
        }

        if (bestImpErr < baseErr) {
            applyMaskFlips(idxL, bestLm, idxR, bestRm, currX, a, currSum);
            // Further 1-bit improvements
            hillClimb1(currX, a, T, currSum);
            long long e = absl(currSum - T);
            if (e < bestErr) { bestErr = e; bestX = currX; bestSum = currSum; }
            if (bestErr == 0) break;
        } else {
            // small random 1-2 flips to escape local min occasionally
            if ((iter % 7 == 0) && n > 0) {
                int flips = min(2, n);
                for (int c = 0; c < flips; ++c) {
                    int i = rng() % n;
                    long long ns = currSum + (currX[i] ? -a[i] : a[i]);
                    if (absl(ns - T) <= absl(currSum - T) || (rng() & 1)) {
                        currX[i] ^= 1;
                        currSum = ns;
                    }
                }
                hillClimb1(currX, a, T, currSum);
                long long e = absl(currSum - T);
                if (e < bestErr) { bestErr = e; bestX = currX; bestSum = currSum; }
            }
        }

        if (elapsed() > TIME_LIMIT) break;
    }

    // Output best solution
    for (int i = 0; i < n; ++i) cout << (int)bestX[i];
    cout << '\n';
    return 0;
}