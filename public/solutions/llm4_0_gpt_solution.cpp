#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int n;
long long k;

struct Node {
    ll val;
    int i, j;
};

struct MinCmp {
    bool operator()(const Node& a, const Node& b) const {
        if (a.val != b.val) return a.val > b.val;
        if (a.i != b.i) return a.i > b.i;
        return a.j > b.j;
    }
};

struct MaxCmp {
    bool operator()(const Node& a, const Node& b) const {
        if (a.val != b.val) return a.val < b.val;
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    }
};

ll query(int x, int y) {
    cout << "QUERY " << x << " " << y << endl;
    cout.flush();
    ll v;
    if (!(cin >> v)) {
        exit(0);
    }
    return v;
}

void done(ll ans) {
    cout << "DONE " << ans << endl;
    cout.flush();
    // Do not read anything after DONE; interactor terminates.
    exit(0);
}

// Ascending: dynamic seeding of rows (good when k < n)
ll ascDynamic(long long kk) {
    priority_queue<Node, vector<Node>, MinCmp> pq;
    ll v11 = query(1, 1);
    pq.push({v11, 1, 1});
    int seeded = 1;

    bool haveNext = false;
    ll nextVal = 0;
    if (seeded < n) {
        nextVal = query(seeded + 1, 1);
        haveNext = true;
    }

    for (long long t = 1; t < kk; ++t) {
        while (seeded < n && haveNext && nextVal <= pq.top().val) {
            pq.push({nextVal, seeded + 1, 1});
            ++seeded;
            if (seeded < n) {
                nextVal = query(seeded + 1, 1);
            } else {
                haveNext = false;
            }
        }
        Node cur = pq.top(); pq.pop();
        if (cur.j < n) {
            ll nv = query(cur.i, cur.j + 1);
            pq.push({nv, cur.i, cur.j + 1});
        }
    }
    while (seeded < n && haveNext && nextVal <= pq.top().val) {
        pq.push({nextVal, seeded + 1, 1});
        ++seeded;
        if (seeded < n) {
            nextVal = query(seeded + 1, 1);
        } else {
            haveNext = false;
        }
    }
    return pq.top().val;
}

// Ascending: static seeding of all rows (good when k >= n)
ll ascStatic(long long kk) {
    priority_queue<Node, vector<Node>, MinCmp> pq;
    for (int i = 1; i <= n; ++i) {
        ll v = query(i, 1);
        pq.push({v, i, 1});
    }
    for (long long t = 1; t < kk; ++t) {
        Node cur = pq.top(); pq.pop();
        if (cur.j < n) {
            ll nv = query(cur.i, cur.j + 1);
            pq.push({nv, cur.i, cur.j + 1});
        }
    }
    return pq.top().val;
}

// Descending: dynamic seeding from bottom rows (good when k' < n)
ll descDynamic(long long kk) {
    priority_queue<Node, vector<Node>, MaxCmp> pq;
    ll vnn = query(n, n);
    pq.push({vnn, n, n});
    int seeded = 1; // number of rows seeded from bottom
    bool haveNext = false;
    ll nextVal = 0;
    if (seeded < n) {
        nextVal = query(n - seeded, n);
        haveNext = true;
    }

    for (long long t = 1; t < kk; ++t) {
        while (seeded < n && haveNext && nextVal >= pq.top().val) {
            pq.push({nextVal, n - seeded, n});
            ++seeded;
            if (seeded < n) {
                nextVal = query(n - seeded, n);
            } else {
                haveNext = false;
            }
        }
        Node cur = pq.top(); pq.pop();
        if (cur.j > 1) {
            ll nv = query(cur.i, cur.j - 1);
            pq.push({nv, cur.i, cur.j - 1});
        }
    }
    while (seeded < n && haveNext && nextVal >= pq.top().val) {
        pq.push({nextVal, n - seeded, n});
        ++seeded;
        if (seeded < n) {
            nextVal = query(n - seeded, n);
        } else {
            haveNext = false;
        }
    }
    return pq.top().val;
}

// Descending: static seeding of all rows from last column (good when k' >= n)
ll descStatic(long long kk) {
    priority_queue<Node, vector<Node>, MaxCmp> pq;
    for (int i = 1; i <= n; ++i) {
        ll v = query(i, n);
        pq.push({v, i, n});
    }
    for (long long t = 1; t < kk; ++t) {
        Node cur = pq.top(); pq.pop();
        if (cur.j > 1) {
            ll nv = query(cur.i, cur.j - 1);
            pq.push({nv, cur.i, cur.j - 1});
        }
    }
    return pq.top().val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> k)) {
        return 0;
    }
    long long total = 1LL * n * n;
    long long kprime = total - k + 1;

    bool useAsc = (k <= kprime);
    long long steps = min(k, kprime);

    ll ans = 0;
    if (useAsc) {
        if (steps < n) ans = ascDynamic(k);
        else ans = ascStatic(k);
    } else {
        if (steps < n) ans = descDynamic(kprime);
        else ans = descStatic(kprime);
    }

    done(ans);
    return 0;
}