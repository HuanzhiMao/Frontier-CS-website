#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static const ll QUERY_LIMIT = 50000;

int n;
long long k;
ll query_count = 0;

// Cache for queried cells
unordered_map<long long, ll> cache;

inline long long key(int i, int j) {
    return ( ( (long long)i ) << 21 ) | (long long)j;
}

ll get_value(int i, int j) {
    long long K = key(i, j);
    auto it = cache.find(K);
    if (it != cache.end()) return it->second;
    cout << "QUERY " << i << " " << j << endl;
    cout.flush();
    ll v;
    if (!(cin >> v)) {
        // If interactor fails, terminate gracefully
        exit(0);
    }
    cache[K] = v;
    query_count++;
    return v;
}

// Count elements <= x using top-right staircase, also fill boundary j array (number of cols <= x per row)
long long count_and_boundary(ll x, vector<int>& J) {
    J.assign(n + 1, 0);
    long long cnt = 0;
    int r = 1;
    int c = n;
    while (r <= n && c >= 1) {
        ll v = get_value(r, c);
        if (v <= x) {
            cnt += c;
            J[r] = c;
            r++;
        } else {
            c--;
        }
    }
    // rows not explicitly set already have 0 in J
    return cnt;
}

// Choose pivot cell roughly at median of the unknown region defined by [JL, JH]
// Returns pair (row, col). Assumes region size > 0.
pair<int,int> choose_pivot_cell(const vector<int>& JL, const vector<int>& JH) {
    long long total = 0;
    for (int i = 1; i <= n; ++i) total += (JH[i] - JL[i]);
    long long target = total / 2; // 0-based index within region
    long long acc = 0;
    for (int i = 1; i <= n; ++i) {
        long long d = (JH[i] - JL[i]);
        if (d <= 0) continue;
        if (acc + d > target) {
            long long offset = target - acc; // 0-based within this row's segment
            int col = JL[i] + 1 + (int)offset;
            return {i, col};
        }
        acc += d;
    }
    // Fallback (should not reach)
    for (int i = 1; i <= n; ++i) {
        if (JH[i] - JL[i] > 0) {
            return {i, JL[i] + 1};
        }
    }
    return {1, 1};
}

// Finalization using multi-merge in the region between JL (exclusive) and JH (inclusive)
ll finalize_in_region(const vector<int>& JL, const vector<int>& JH, long long rank_in_region) {
    struct Entry {
        ll val;
        int i, j;
        bool operator>(const Entry& other) const {
            if (val != other.val) return val > other.val;
            if (i != other.i) return i > other.i;
            return j > other.j;
        }
    };
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    // Initialize heap with first element from each row segment
    for (int i = 1; i <= n; ++i) {
        int jstart = JL[i] + 1;
        if (jstart <= JH[i]) {
            ll v = get_value(i, jstart);
            pq.push({v, i, jstart});
        }
    }

    // Pop rank_in_region-th element (1-based)
    long long popped = 0;
    while (!pq.empty()) {
        Entry cur = pq.top(); pq.pop();
        popped++;
        if (popped == rank_in_region) {
            return cur.val;
        }
        int ni = cur.i;
        int nj = cur.j + 1;
        if (nj <= JH[ni]) {
            ll nv = get_value(ni, nj);
            pq.push({nv, ni, nj});
        }
    }
    // Should not reach if rank_in_region is valid
    return -1;
}

// K-th smallest using min-heap from top-left
ll kth_from_top_left(long long target) {
    struct Node {
        ll val;
        int i, j;
        bool operator>(const Node& other) const {
            if (val != other.val) return val > other.val;
            if (i != other.i) return i > other.i;
            return j > other.j;
        }
    };
    vector<char> visited((n + 2) * (n + 2), 0);
    auto idx = [&](int i, int j)->int { return i * (n + 2) + j; };

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    ll v = get_value(1, 1);
    pq.push({v, 1, 1});
    visited[idx(1,1)] = 1;

    long long popped = 0;
    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        popped++;
        if (popped == target) {
            return cur.val;
        }
        int ni = cur.i + 1, nj = cur.j;
        if (ni <= n && !visited[idx(ni, nj)]) {
            visited[idx(ni, nj)] = 1;
            ll nv = get_value(ni, nj);
            pq.push({nv, ni, nj});
        }
        ni = cur.i; nj = cur.j + 1;
        if (nj <= n && !visited[idx(ni, nj)]) {
            visited[idx(ni, nj)] = 1;
            ll nv = get_value(ni, nj);
            pq.push({nv, ni, nj});
        }
    }
    return -1;
}

// K-th largest using max-heap from bottom-right (equivalently (n*n - k + 1)-th largest -> find value directly)
ll kth_from_bottom_right(long long target) {
    struct Node {
        ll val;
        int i, j;
        bool operator<(const Node& other) const {
            if (val != other.val) return val < other.val; // max-heap
            if (i != other.i) return i < other.i;
            return j < other.j;
        }
    };
    vector<char> visited((n + 2) * (n + 2), 0);
    auto idx = [&](int i, int j)->int { return i * (n + 2) + j; };

    priority_queue<Node> pq;
    ll v = get_value(n, n);
    pq.push({v, n, n});
    visited[idx(n,n)] = 1;

    long long popped = 0;
    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        popped++;
        if (popped == target) {
            return cur.val;
        }
        int ni = cur.i - 1, nj = cur.j;
        if (ni >= 1 && !visited[idx(ni, nj)]) {
            visited[idx(ni, nj)] = 1;
            ll nv = get_value(ni, nj);
            pq.push({nv, ni, nj});
        }
        ni = cur.i; nj = cur.j - 1;
        if (nj >= 1 && !visited[idx(ni, nj)]) {
            visited[idx(ni, nj)] = 1;
            ll nv = get_value(ni, nj);
            pq.push({nv, ni, nj});
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> k)) {
        return 0;
    }

    // Query corners
    ll minV = get_value(1, 1);
    ll maxV = get_value(n, n);

    long long totalCells = 1LL * n * n;

    // Try small-k or small-(n^2 - k + 1) using heap approach within budget
    const long long BFS_LIMIT = 15000; // conservative cap to avoid exceeding 50k queries
    if (k <= BFS_LIMIT) {
        ll ans = kth_from_top_left(k);
        cout << "DONE " << ans << endl;
        cout.flush();
        return 0;
    }
    if ((totalCells - k + 1) <= BFS_LIMIT) {
        ll ans = kth_from_bottom_right(totalCells - k + 1);
        cout << "DONE " << ans << endl;
        cout.flush();
        return 0;
    }

    // Region-shrinking approach
    vector<int> JL(n + 1, 0), JH(n + 1, n);
    long long cntLow = 0;                 // count <= lowVal
    long long cntHigh = totalCells;       // count <= highVal
    ll lowVal = minV - 1;                 // conceptual value with count 0
    ll highVal = maxV;                    // count totalCells

    auto region_size = [&]()->long long {
        long long s = 0;
        for (int i = 1; i <= n; ++i) s += (JH[i] - JL[i]);
        return s;
    };

    // Iteratively shrink the region
    while (true) {
        long long c = region_size();
        if (c == 0 || lowVal == highVal) break;
        long long remainBudget = QUERY_LIMIT - query_count - 10; // keep some margin
        if (c <= remainBudget) break;

        // Choose pivot cell around the median of region
        auto [pi, pj] = choose_pivot_cell(JL, JH);
        ll pv = get_value(pi, pj);

        vector<int> JM(n + 1, 0);
        long long cntM = count_and_boundary(pv, JM);

        if (cntM >= k) {
            highVal = pv;
            JH.swap(JM);
            cntHigh = cntM;
        } else {
            lowVal = pv;
            JL.swap(JM);
            cntLow = cntM;
        }
    }

    // Finalization
    long long c = 0;
    for (int i = 1; i <= n; ++i) c += (JH[i] - JL[i]);

    if (c == 0) {
        // Region empty; answer is highVal
        cout << "DONE " << highVal << endl;
        cout.flush();
        return 0;
    }

    // Ensure enough budget to finish within region; if not, continue shrinking until it is
    while (true) {
        long long remainBudget = QUERY_LIMIT - query_count - 10; // margin
        // Need at most (#rows with non-empty) + (k - cntLow) queries (worst-case)
        long long rowsNonEmpty = 0;
        for (int i = 1; i <= n; ++i) if (JH[i] - JL[i] > 0) rowsNonEmpty++;
        long long g = k - cntLow;
        long long need = rowsNonEmpty + g;
        if (need <= remainBudget) break;

        if (lowVal == highVal) break;
        if (c == 0) break;

        // Further shrink region by another pivot
        auto [pi, pj] = choose_pivot_cell(JL, JH);
        ll pv = get_value(pi, pj);
        vector<int> JM(n + 1, 0);
        long long cntM = count_and_boundary(pv, JM);
        if (cntM >= k) {
            highVal = pv;
            JH.swap(JM);
            cntHigh = cntM;
        } else {
            lowVal = pv;
            JL.swap(JM);
            cntLow = cntM;
        }

        c = 0;
        for (int i = 1; i <= n; ++i) c += (JH[i] - JL[i]);
        if (c == 0) break;
    }

    // If region exhausted or bounds converged, answer is highVal
    if (highVal == lowVal || c == 0) {
        cout << "DONE " << highVal << endl;
        cout.flush();
        return 0;
    }

    // Otherwise, finish inside region
    long long rank_in_region = k - cntLow; // 1-based
    ll ans = finalize_in_region(JL, JH, rank_in_region);
    cout << "DONE " << ans << endl;
    cout.flush();
    return 0;
}