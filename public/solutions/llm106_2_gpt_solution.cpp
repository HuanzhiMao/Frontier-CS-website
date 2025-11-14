#include <bits/stdc++.h>
using namespace std;

static const int INF = 1e9;

int n;

int ask(const vector<int>& s) {
    if (s.empty()) return 0; // We never actually query empty set
    cout << "? " << (int)s.size() << '\n';
    for (int i = 0; i < (int)s.size(); ++i) {
        if (i) cout << ' ';
        cout << s[i];
    }
    cout << '\n';
    cout.flush();
    int m;
    if (!(cin >> m)) exit(0);
    if (m == -1) exit(0);
    return m;
}

vector<int> merge_sets(const vector<int>& a, const vector<int>& b) {
    vector<int> c;
    c.reserve(a.size() + b.size());
    for (int x : a) c.push_back(x);
    for (int x : b) c.push_back(x);
    return c;
}

bool hasCrossEdges(const vector<int>& B, const vector<int>& X, int EB) {
    if (X.empty()) return false;
    int eX = ask(X);
    vector<int> U = merge_sets(B, X);
    int eBX = ask(U);
    int cross = eBX - EB - eX;
    return cross > 0;
}

int findVertexConnectedToB(const vector<int>& B, const vector<int>& Uset, int EB) {
    vector<int> cur = Uset;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> left(cur.begin(), cur.begin() + mid);
        vector<int> right(cur.begin() + mid, cur.end());
        if (hasCrossEdges(B, left, EB)) cur = left;
        else cur = right;
    }
    return cur[0];
}

int findNeighborInB(const vector<int>& B, int v) {
    vector<int> cur = B;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> left(cur.begin(), cur.begin() + mid);
        vector<int> right(cur.begin() + mid, cur.end());
        if (!left.empty()) {
            int eL = ask(left);
            vector<int> Lp = left;
            Lp.push_back(v);
            int eLv = ask(Lp);
            if (eLv - eL > 0) {
                cur = left;
                continue;
            }
        }
        cur = right;
    }
    return cur[0];
}

pair<int,int> findEdgeInsideSubset(const vector<int>& S) {
    vector<int> pref;
    int last = 0;
    for (int i = 0; i < (int)S.size(); ++i) {
        pref.push_back(S[i]);
        int curm = ask(pref);
        if (curm - last > 0) {
            int v = S[i];
            vector<int> P = pref;
            P.pop_back();
            // binary search to find neighbor of v in P
            vector<int> cur = P;
            while (cur.size() > 1) {
                int mid = (int)cur.size() / 2;
                vector<int> left(cur.begin(), cur.begin() + mid);
                vector<int> right(cur.begin() + mid, cur.end());
                if (!left.empty()) {
                    int eL = ask(left);
                    vector<int> Lp = left;
                    Lp.push_back(v);
                    int eLv = ask(Lp);
                    if (eLv - eL > 0) {
                        cur = left;
                        continue;
                    }
                }
                cur = right;
            }
            return {cur[0], v};
        }
        last = curm;
    }
    return {-1, -1}; // should not happen if E(S) > 0
}

vector<int> pathBetween(int a, int b, const vector<int>& parent, const vector<int>& depth) {
    vector<int> pa, pb;
    int u = a, v = b;
    int du = depth[u], dv = depth[v];
    while (du > dv) {
        pa.push_back(u);
        u = parent[u];
        du--;
    }
    while (dv > du) {
        pb.push_back(v);
        v = parent[v];
        dv--;
    }
    while (u != v) {
        pa.push_back(u);
        pb.push_back(v);
        u = parent[u];
        v = parent[v];
    }
    // u == v is LCA
    vector<int> res;
    for (int x : pa) res.push_back(x);
    res.push_back(u);
    reverse(pb.begin(), pb.end());
    for (int x : pb) res.push_back(x);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    vector<int> B, Uset;
    for (int i = 2; i <= n; ++i) Uset.push_back(i);
    B.push_back(1);

    vector<int> color(n + 1, -1), parent(n + 1, -1), depth(n + 1, 0);
    color[1] = 0;
    parent[1] = -1;
    depth[1] = 0;

    int EB = ask(B); // edges inside B

    while (!Uset.empty()) {
        int v = findVertexConnectedToB(B, Uset, EB);
        int u = findNeighborInB(B, v);

        parent[v] = u;
        depth[v] = depth[u] + 1;
        color[v] = 1 - color[u];

        B.push_back(v);
        Uset.erase(find(Uset.begin(), Uset.end(), v));

        EB = ask(B);
    }

    vector<int> part0, part1;
    for (int i = 1; i <= n; ++i) {
        if (color[i] == 0) part0.push_back(i);
        else part1.push_back(i);
    }

    int e0 = part0.empty() ? 0 : ask(part0);
    int e1 = part1.empty() ? 0 : ask(part1);

    if (e0 == 0 && e1 == 0) {
        cout << "Y " << (int)part0.size() << '\n';
        for (int i = 0; i < (int)part0.size(); ++i) {
            if (i) cout << ' ';
            cout << part0[i];
        }
        cout << '\n';
        cout.flush();
        return 0;
    } else {
        vector<int> S = (e0 > 0 ? part0 : part1);
        auto p = findEdgeInsideSubset(S);
        int x = p.first, y = p.second;

        vector<int> cyc = pathBetween(x, y, parent, depth);
        int L = (int)cyc.size() + 1;

        cout << "N " << L << '\n';
        for (int i = 0; i < (int)cyc.size(); ++i) {
            if (i) cout << ' ';
            cout << cyc[i];
        }
        cout << '\n';
        cout.flush();
        return 0;
    }
}