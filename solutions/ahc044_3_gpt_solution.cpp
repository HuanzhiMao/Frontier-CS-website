#include <bits/stdc++.h>
using namespace std;

static inline uint64_t splitmix64(uint64_t& x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

struct RNG {
    uint64_t seed;
    RNG(uint64_t s=123456789) { seed = s; }
    uint64_t next() { return splitmix64(seed); }
    int nextInt(int l, int r) {
        return l + (int)(next() % (uint64_t)(r - l + 1));
    }
    double nextDouble() {
        return (next() >> 11) * (1.0/9007199254740992.0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    int L;
    if (!(cin >> N >> L)) {
        return 0;
    }
    vector<int> T(N);
    for (int i = 0; i < N; ++i) cin >> T[i];

    // Prepare probabilities
    vector<double> q(N);
    for (int i = 0; i < N; ++i) q[i] = (double)T[i] / (double)L;

    // Initial mapping: set b as ring, and choose a by greedy capacity fill
    vector<int> a(N, 0), b(N, 0);
    for (int i = 0; i < N; ++i) b[i] = (i + 1) % N;

    // rem capacity for a-edges
    vector<double> rem(N, 0.0);
    for (int j = 0; j < N; ++j) {
        int pred = (j - 1 + N) % N;
        rem[j] = 2.0 * q[j] - q[pred];
    }

    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j){
        if (q[i] != q[j]) return q[i] > q[j];
        return i < j;
    });

    auto pick_max_rem = [&](void)->int{
        int idx = 0;
        double best = rem[0];
        for (int j = 1; j < N; ++j) {
            if (rem[j] > best) { best = rem[j]; idx = j; }
        }
        return idx;
    };

    for (int t = 0; t < N; ++t) {
        int i = order[t];
        int j = pick_max_rem();
        a[i] = j;
        rem[j] -= q[i];
    }

    // Balancing pass for a-edges to reduce overfill
    const double EPS = 1e-12;
    bool improved = true;
    int balance_iters = 0;
    while (improved && balance_iters < 3) {
        improved = false;
        ++balance_iters;
        // Collect current over and under
        vector<int> over, under;
        for (int j = 0; j < N; ++j) {
            if (rem[j] < -EPS) over.push_back(j);
            else if (rem[j] > EPS) under.push_back(j);
        }
        sort(over.begin(), over.end(), [&](int x, int y){ return rem[x] < rem[y]; });
        sort(under.begin(), under.end(), [&](int x, int y){ return rem[x] > rem[y]; });

        if (over.empty() || under.empty()) break;

        // Map from column to list of rows pointing there via a-edge
        vector<vector<int>> rowsToCol(N);
        for (int i = 0; i < N; ++i) rowsToCol[a[i]].push_back(i);

        size_t ui = 0;
        for (int ov_idx = 0; ov_idx < (int)over.size(); ++ov_idx) {
            int jOver = over[ov_idx];
            if (rem[jOver] >= -EPS) continue;
            // Try to move some rows from jOver to underfilled columns
            for (int ri = 0; ri < (int)rowsToCol[jOver].size() && rem[jOver] < -EPS; ++ri) {
                if (ui >= under.size()) break;
                int row = rowsToCol[jOver][ri];
                // Find an underfilled column with enough positive rem (or just the best one)
                while (ui < under.size() && rem[under[ui]] <= EPS) ++ui;
                if (ui >= under.size()) break;
                int jUnder = under[ui];
                // Reassign
                a[row] = jUnder;
                rem[jOver] += q[row];
                rem[jUnder] -= q[row];
                improved = true;
                // If jUnder is no longer underfilled, advance ui
                if (rem[jUnder] <= EPS) ++ui;
            }
        }
    }

    auto simulate_counts = [&](const vector<int>& A, const vector<int>& B)->vector<int>{
        vector<int> cnt(N, 0);
        int cur = 0;
        cnt[cur] = 1;
        for (int step = 2; step <= L; ++step) {
            int next = (cnt[cur] & 1) ? A[cur] : B[cur];
            cur = next;
            ++cnt[cur];
        }
        return cnt;
    };

    auto compute_error = [&](const vector<int>& cnt)->long long{
        long long E = 0;
        for (int i = 0; i < N; ++i) {
            E += llabs((long long)cnt[i] - (long long)T[i]);
        }
        return E;
    };

    vector<int> bestA = a, bestB = b;
    vector<int> cnt = simulate_counts(bestA, bestB);
    long long bestE = compute_error(cnt);

    // Hill climbing refinement
    RNG rng(123456789ULL);
    auto start_time = chrono::steady_clock::now();
    const double TIME_LIMIT_MS = 1800.0;

    int attempts = 0;
    while (true) {
        auto now = chrono::steady_clock::now();
        double ms = chrono::duration<double, milli>(now - start_time).count();
        if (ms > TIME_LIMIT_MS) break;
        ++attempts;

        // Build deficits
        vector<long long> diff(N);
        vector<int> under, over;
        under.reserve(N); over.reserve(N);
        for (int i = 0; i < N; ++i) {
            diff[i] = (long long)T[i] - (long long)cnt[i];
            if (diff[i] > 0) under.push_back(i);
            else if (diff[i] < 0) over.push_back(i);
        }
        if (under.empty() || over.empty()) break;

        sort(under.begin(), under.end(), [&](int i, int j){ return diff[i] > diff[j]; });
        sort(over.begin(), over.end(), [&](int i, int j){ return diff[i] < diff[j]; });

        // Weighted pick of source node u by its count
        vector<int> prefix(N);
        prefix[0] = cnt[0];
        for (int i = 1; i < N; ++i) prefix[i] = prefix[i-1] + cnt[i];
        int total = prefix[N-1];
        int r = rng.nextInt(1, max(1, total));
        int u = int(lower_bound(prefix.begin(), prefix.end(), r) - prefix.begin());

        // Choose which edge to modify: prefer one pointing to overrepresented
        int oldDestA = bestA[u];
        int oldDestB = bestB[u];
        bool changeA = false;
        if (!over.empty()) {
            int ov = over[0];
            if (oldDestA == ov && oldDestB != ov) changeA = true;
            else if (oldDestB == ov && oldDestA != ov) changeA = false;
            else if (oldDestA == ov && oldDestB == ov) changeA = (rng.nextInt(0,1)==0);
            else {
                // neither points to the top-over node: pick one that is more over
                long long va = (oldDestA >= 0 ? -diff[oldDestA] : LLONG_MIN);
                long long vb = (oldDestB >= 0 ? -diff[oldDestB] : LLONG_MIN);
                if (va < vb) changeA = false; else changeA = true;
            }
        } else {
            changeA = (rng.nextInt(0,1)==0);
        }

        int edgeToChange = changeA ? 0 : 1;

        // Choose new destination: one of top-K underrepresented
        int K = min(10, (int)under.size());
        int w = under[rng.nextInt(0, K-1)];

        // Apply tentative change
        int oldVal;
        if (edgeToChange == 0) {
            if (bestA[u] == w) continue;
            oldVal = bestA[u];
            bestA[u] = w;
        } else {
            if (bestB[u] == w) continue;
            oldVal = bestB[u];
            bestB[u] = w;
        }

        // Evaluate
        vector<int> newCnt = simulate_counts(bestA, bestB);
        long long newE = compute_error(newCnt);
        if (newE < bestE) {
            bestE = newE;
            cnt.swap(newCnt);
        } else {
            // revert
            if (edgeToChange == 0) bestA[u] = oldVal;
            else bestB[u] = oldVal;
        }

        // Check time
        now = chrono::steady_clock::now();
        ms = chrono::duration<double, milli>(now - start_time).count();
        if (ms > TIME_LIMIT_MS) break;
    }

    // Output
    for (int i = 0; i < N; ++i) {
        cout << bestA[i] << ' ' << bestB[i] << '\n';
    }
    return 0;
}