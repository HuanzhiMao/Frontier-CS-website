#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long val;
    int i, j;
};

struct MinCmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.val > b.val;
    }
};

struct MaxCmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.val < b.val;
    }
};

long long do_query(int x, int y) {
    cout << "QUERY " << x << " " << y << endl;
    cout.flush();
    long long v;
    if (!(cin >> v)) {
        exit(0);
    }
    return v;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    if (!(cin >> n >> k)) return 0;

    long long N2 = n * n;
    bool forward = true;
    long long t = k;
    if (N2 - k + 1 < k) {
        forward = false;
        t = N2 - k + 1;
    }

    long long ans = 0;

    if (forward) {
        priority_queue<Node, vector<Node>, MinCmp> pq;
        long long initialRows = min<long long>(n, t);
        for (int i = 1; i <= initialRows; ++i) {
            long long v = do_query(i, 1);
            pq.push({v, i, 1});
        }
        long long cnt = 0;
        while (true) {
            Node cur = pq.top(); pq.pop();
            ++cnt;
            if (cnt == t) {
                ans = cur.val;
                break;
            }
            if (cur.j + 1 <= n) {
                long long v2 = do_query(cur.i, cur.j + 1);
                pq.push({v2, cur.i, cur.j + 1});
            }
        }
    } else {
        priority_queue<Node, vector<Node>, MaxCmp> pq;
        long long initialRows = min<long long>(n, t);
        int startRow = (int)(n - initialRows + 1);
        for (int i = startRow; i <= n; ++i) {
            long long v = do_query(i, (int)n);
            pq.push({v, i, (int)n});
        }
        long long cnt = 0;
        while (true) {
            Node cur = pq.top(); pq.pop();
            ++cnt;
            if (cnt == t) {
                ans = cur.val;
                break;
            }
            if (cur.j - 1 >= 1) {
                long long v2 = do_query(cur.i, cur.j - 1);
                pq.push({v2, cur.i, cur.j - 1});
            }
        }
    }

    cout << "DONE " << ans << endl;
    cout.flush();
    return 0;
}