#include <bits/stdc++.h>
using namespace std;

struct SimResult {
    vector<int> cnt;
    vector<int> aUsed;
    vector<int> bUsed;
    long long error;
};

static inline SimResult simulate(const vector<int>& a, const vector<int>& b, const vector<int>& T, int N, int L) {
    vector<int> cnt(N, 0), aUsed(N, 0), bUsed(N, 0);
    int x = 0;
    cnt[x]++;
    for (int step = 1; step < L; ++step) {
        bool odd = (cnt[x] % 2 == 1);
        int y = odd ? a[x] : b[x];
        if (odd) aUsed[x]++; else bUsed[x]++;
        x = y;
        cnt[x]++;
    }
    long long E = 0;
    for (int i = 0; i < N; ++i) E += llabs((long long)cnt[i] - (long long)T[i]);
    return {cnt, aUsed, bUsed, E};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, L;
    if (!(cin >> N >> L)) return 0;
    vector<int> T(N);
    for (int i = 0; i < N; ++i) cin >> T[i];

    vector<int> a(N, -1), b(N, -1);
    vector<int> v(N, 0);      // counts built during greedy
    vector<int> deficit = T;  // deficit[i] = T[i] - v[i], maintained incrementally

    auto pickDest = [&](int avoid) -> int {
        int idxPN = -1, valPN = INT_MIN; // best among positive deficit and not visited
        int idxP  = -1, valP  = INT_MIN; // best among positive deficit
        int idxN  = -1, valN  = INT_MIN; // best among not visited
        int idxA  = -1, valA  = INT_MIN; // best among all
        for (int j = 0; j < N; ++j) {
            if (j == avoid) continue;
            int val = deficit[j] - v[j]; // maintain as T[j] - v[j] = deficit[j] - v[j], since deficit is T and v is counts
            // But we kept deficit as T and we decrement deficit[y] when we visit y, so we should compare deficit[j].
            // Let's fix: we already update deficit directly as remaining T - visited, so use deficit[j].
        }
        // Re-implement correctly: deficit[j] is maintained as T[j] - v[j], so no need to compute T[j]-v[j] again.
        idxPN = -1; valPN = INT_MIN;
        idxP  = -1; valP  = INT_MIN;
        idxN  = -1; valN  = INT_MIN;
        idxA  = -1; valA  = INT_MIN;
        for (int j = 0; j < N; ++j) {
            if (j == avoid) continue;
            int val = deficit[j];
            if (val > valA) { valA = val; idxA = j; }
            if (v[j] == 0) {
                if (val > valN) { valN = val; idxN = j; }
                if (val > 0 && val > valPN) { valPN = val; idxPN = j; }
            }
            if (val > 0 && val > valP) { valP = val; idxP = j; }
        }
        if (idxPN != -1) return idxPN;
        if (idxP  != -1) return idxP;
        if (idxN  != -1) return idxN;
        if (idxA  != -1) return idxA;
        // Fallback (should not happen with N >= 1)
        for (int j = 0; j < N; ++j) if (j != avoid) return j;
        return 0;
    };

    // Greedy build of mapping and schedule
    int x = 0;
    v[x]++; deficit[x]--;
    vector<int> aUsed_build(N, 0), bUsed_build(N, 0);
    for (int step = 1; step < L; ++step) {
        bool odd = (v[x] % 2 == 1);
        int y;
        if (odd) {
            if (a[x] >= 0) y = a[x];
            else {
                y = pickDest(-1);
                a[x] = y;
            }
            aUsed_build[x]++;
        } else {
            if (b[x] >= 0) y = b[x];
            else {
                y = pickDest(a[x]);
                b[x] = y;
            }
            bUsed_build[x]++;
        }
        x = y;
        v[x]++; deficit[x]--;
    }

    // Fill any unset pointers (not used in first L weeks, but must output valid values)
    for (int i = 0; i < N; ++i) {
        if (a[i] < 0) a[i] = 0;
        if (b[i] < 0) b[i] = a[i];
    }

    // Optional small improvement loop
    // Prepare initial counts and usage from the greedy build
    vector<int> cnt = v;
    vector<int> aUsed = aUsed_build, bUsed = bUsed_build;
    auto currentError = [&]() -> long long {
        long long E = 0;
        for (int i = 0; i < N; ++i) E += llabs((long long)cnt[i] - (long long)T[i]);
        return E;
    };
    long long bestE = currentError();

    int improveIters = 10; // keep small to stay within time
    for (int iter = 0; iter < improveIters; ++iter) {
        // Compute diffs
        vector<int> diff(N);
        for (int i = 0; i < N; ++i) diff[i] = T[i] - cnt[i];
        int idxDef = -1, idxSurp = -1;
        int bestDefVal = INT_MIN, bestSurpVal = INT_MAX;
        for (int i = 0; i < N; ++i) {
            if (diff[i] > bestDefVal) { bestDefVal = diff[i]; idxDef = i; }
            if (diff[i] < bestSurpVal) { bestSurpVal = diff[i]; idxSurp = i; }
        }
        if (idxDef == -1 || bestDefVal <= 0) break; // no deficits to fix
        if (idxSurp == -1 || bestSurpVal >= 0) break; // no surpluses

        // Collect candidate pointers pointing to the most surplus node
        struct Cand { int use; int x; int t; }; // t=0->a, t=1->b
        vector<Cand> cand;
        for (int i = 0; i < N; ++i) {
            if (a[i] == idxSurp && aUsed[i] > 0) cand.push_back({aUsed[i], i, 0});
            if (b[i] == idxSurp && bUsed[i] > 0) cand.push_back({bUsed[i], i, 1});
        }
        if (cand.empty()) break;
        sort(cand.begin(), cand.end(), [&](const Cand& A, const Cand& B){ return A.use > B.use; });

        bool improved = false;
        int tries = min<int>(3, cand.size());
        for (int k = 0; k < tries; ++k) {
            int px = cand[k].x;
            int pt = cand[k].t;
            int oldDest = (pt == 0 ? a[px] : b[px]);
            if (oldDest == idxDef) continue; // already pointing to deficit node
            int oldVal = (pt == 0 ? a[px] : b[px]);
            if (pt == 0) a[px] = idxDef; else b[px] = idxDef;

            SimResult sr = simulate(a, b, T, N, L);
            if (sr.error < bestE) {
                // accept
                bestE = sr.error;
                cnt.swap(sr.cnt);
                aUsed.swap(sr.aUsed);
                bUsed.swap(sr.bUsed);
                improved = true;
                break;
            } else {
                // revert
                if (pt == 0) a[px] = oldVal; else b[px] = oldVal;
            }
        }
        if (!improved) break;
    }

    for (int i = 0; i < N; ++i) {
        cout << a[i] << ' ' << b[i] << '\n';
    }
    return 0;
}