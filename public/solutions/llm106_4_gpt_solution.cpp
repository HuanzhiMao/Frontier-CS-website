#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int n;
vector<vector<int>> tree_adj;
vector<int> colorv;

ll query(const vector<int>& s) {
    cout << "? " << s.size() << '\n';
    if (!s.empty()) {
        for (size_t i = 0; i < s.size(); ++i) {
            if (i) cout << ' ';
            cout << s[i];
        }
    }
    cout << '\n';
    cout.flush();
    ll ans;
    if (!(cin >> ans)) exit(0);
    if (ans == -1) exit(0);
    return ans;
}

int findNeighbor(int v, const vector<int>& pool) {
    vector<int> cur = pool;
    while (cur.size() > 1) {
        size_t mid = cur.size() / 2;
        vector<int> left(cur.begin(), cur.begin() + mid);
        vector<int> right(cur.begin() + mid, cur.end());
        bool inLeft = false;
        if (!left.empty()) {
            vector<int> t = left; t.push_back(v);
            ll a = query(t);
            ll b = query(left);
            if (a - b > 0) inLeft = true;
        }
        cur = inLeft ? left : right;
    }
    return cur[0];
}

pair<int, ll> findVertexConnectedToA(const vector<int>& A, ll EA, const vector<int>& rest) {
    vector<int> cand = rest;
    while (cand.size() > 1) {
        size_t mid = cand.size() / 2;
        vector<int> left(cand.begin(), cand.begin() + mid);
        vector<int> right(cand.begin() + mid, cand.end());
        if (!left.empty()) {
            vector<int> U = A;
            U.insert(U.end(), left.begin(), left.end());
            ll AU = query(U);
            ll EL = query(left);
            ll cross = AU - EA - EL;
            if (cross > 0) {
                cand = left;
            } else {
                cand = right;
            }
        } else {
            cand = right;
        }
    }
    int v = cand[0];
    vector<int> U = A;
    U.push_back(v);
    ll EAplus = query(U);
    return {v, EAplus};
}

pair<int,int> findInternalEdge(const vector<int>& S) {
    vector<int> P;
    ll e = 0;
    for (int v : S) {
        vector<int> Pplus = P;
        Pplus.push_back(v);
        ll newE = query(Pplus);
        if (newE > e) {
            if (!P.empty()) {
                int u = findNeighbor(v, P);
                return {u, v};
            }
        }
        e = newE;
        P.push_back(v);
    }
    return {0, 0};
}

vector<int> getPath(int u, int v) {
    vector<int> prev(n + 1, -1);
    queue<int> q;
    q.push(u);
    prev[u] = -2;
    while (!q.empty()) {
        int x = q.front(); q.pop();
        if (x == v) break;
        for (int y : tree_adj[x]) {
            if (prev[y] == -1) {
                prev[y] = x;
                q.push(y);
            }
        }
    }
    vector<int> path;
    int cur = v;
    while (cur != -2) {
        path.push_back(cur);
        if (cur == u) break;
        cur = prev[cur];
        if (cur == -1) break;
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    tree_adj.assign(n + 1, {});
    colorv.assign(n + 1, -1);

    vector<int> A;
    vector<int> rest;
    for (int i = 2; i <= n; ++i) rest.push_back(i);
    A.push_back(1);
    colorv[1] = 0;

    ll EA = query(A); // should be 0

    while (!rest.empty()) {
        auto [v, EAplus] = findVertexConnectedToA(A, EA, rest);
        int u = findNeighbor(v, A);
        tree_adj[u].push_back(v);
        tree_adj[v].push_back(u);
        colorv[v] = 1 - colorv[u];

        A.push_back(v);
        EA = EAplus;

        // remove v from rest
        for (size_t i = 0; i < rest.size(); ++i) {
            if (rest[i] == v) {
                rest.erase(rest.begin() + i);
                break;
            }
        }
    }

    vector<int> part0, part1;
    for (int i = 1; i <= n; ++i) {
        if (colorv[i] == 0) part0.push_back(i);
        else part1.push_back(i);
    }

    ll e0 = 0;
    if (!part0.empty()) e0 = query(part0);
    if (e0 > 0) {
        auto pr = findInternalEdge(part0);
        int x = pr.first, y = pr.second;
        vector<int> cyc = getPath(x, y);
        cout << "N " << cyc.size() << '\n';
        for (size_t i = 0; i < cyc.size(); ++i) {
            if (i) cout << ' ';
            cout << cyc[i];
        }
        cout << '\n';
        cout.flush();
        return 0;
    }
    ll e1 = 0;
    if (!part1.empty()) e1 = query(part1);
    if (e1 > 0) {
        auto pr = findInternalEdge(part1);
        int x = pr.first, y = pr.second;
        vector<int> cyc = getPath(x, y);
        cout << "N " << cyc.size() << '\n';
        for (size_t i = 0; i < cyc.size(); ++i) {
            if (i) cout << ' ';
            cout << cyc[i];
        }
        cout << '\n';
        cout.flush();
        return 0;
    }

    cout << "Y " << part0.size() << '\n';
    for (size_t i = 0; i < part0.size(); ++i) {
        if (i) cout << ' ';
        cout << part0[i];
    }
    cout << '\n';
    cout.flush();

    return 0;
}