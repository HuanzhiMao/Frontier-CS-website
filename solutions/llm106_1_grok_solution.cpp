#include <bits/stdc++.h>
using namespace std;

int n;

int query(const vector<int>& s) {
    if (s.empty()) return 0;
    cout << "? " << s.size() << endl;
    for (int x : s) cout << x << " ";
    cout << endl;
    cout.flush();
    int m;
    cin >> m;
    if (m == -1) exit(0);
    return m;
}

int main() {
    cin >> n;
    if (n == 1) {
        cout << "Y 1" << endl;
        cout << "1" << endl;
        return 0;
    }
    vector<bool> visited(n + 1, false);
    vector<int> parent(n + 1, -1);
    vector<int> col(n + 1, -1);
    queue<int> q;
    int root = 1;
    visited[root] = true;
    col[root] = 0;
    parent[root] = -1;
    q.push(root);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        vector<int> U;
        for (int i = 1; i <= n; i++) {
            if (!visited[i] && i != v) U.push_back(i);
        }
        if (U.empty()) continue;
        int du = query(U);
        vector<int> vU = U;
        vU.push_back(v);
        sort(vU.begin(), vU.end());
        int dvu = query(vU);
        int d = dvu - du;
        if (d == 0) continue;
        vector<int> remaining = U;
        sort(remaining.begin(), remaining.end());
        for (int child = 0; child < d; child++) {
            int lo = 0, hi = remaining.size() - 1;
            int idx = -1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                vector<int> T;
                for (int j = 0; j <= mid; j++) T.push_back(remaining[j]);
                int et = query(T);
                vector<int> vT = T;
                vT.push_back(v);
                sort(vT.begin(), vT.end());
                int evt = query(vT);
                int num = evt - et;
                if (num > 0) {
                    hi = mid;
                    if (lo == hi) {
                        idx = lo;
                        break;
                    }
                } else {
                    lo = mid + 1;
                }
            }
            assert(idx != -1);
            int u = remaining[idx];
            visited[u] = true;
            parent[u] = v;
            col[u] = col[v] + 1;
            q.push(u);
            remaining.erase(remaining.begin() + idx);
        }
    }
    vector<int> A, B;
    for (int i = 1; i <= n; i++) {
        if (col[i] % 2 == 0) A.push_back(i);
        else B.push_back(i);
    }
    int ea = query(A);
    int eb = B.empty() ? 0 : query(B);
    if (ea == 0 && eb == 0) {
        cout << "Y " << A.size() << endl;
        for (int x : A) cout << x << " ";
        cout << endl;
        return 0;
    }
    vector<int> part, other;
    int epart;
    bool isa = ea > 0;
    if (isa) {
        part = A;
        other = B;
        epart = ea;
    } else {
        part = B;
        other = A;
        epart = eb;
    }
    int psz = part.size();
    vector<int> degp(psz, 0);
    for (int i = 0; i < psz; i++) {
        int vv = part[i];
        vector<int> pwov;
        for (int j = 0; j < psz; j++) if (j != i) pwov.push_back(part[j]);
        int ewov = query(pwov);
        degp[i] = epart - ewov;
    }
    int vi = -1;
    for (int i = 0; i < psz; i++) {
        if (degp[i] > 0) {
            vi = i;
            break;
        }
    }
    assert(vi != -1);
    int vv = part[vi];
    vector<int> remp;
    for (int j = 0; j < psz; j++) if (part[j] != vv) remp.push_back(part[j]);
    sort(remp.begin(), remp.end());
    int lo = 0, hi = remp.size() - 1;
    int idx = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        vector<int> T;
        for (int j = 0; j <= mid; j++) T.push_back(remp[j]);
        int et = query(T);
        vector<int> vT = T;
        vT.push_back(vv);
        sort(vT.begin(), vT.end());
        int evt = query(vT);
        int num = evt - et;
        if (num > 0) {
            hi = mid;
            if (lo == hi) {
                idx = lo;
                break;
            }
        } else {
            lo = mid + 1;
        }
    }
    assert(idx != -1);
    int w = remp[idx];
    vector<int> fromvv;
    int cur = vv;
    while (cur != -1) {
        fromvv.push_back(cur);
        cur = parent[cur];
    }
    vector<int> fromww;
    cur = w;
    while (cur != -1) {
        fromww.push_back(cur);
        cur = parent[cur];
    }
    reverse(fromvv.begin(), fromvv.end());
    reverse(fromww.begin(), fromww.end());
    int minlen = min(fromvv.size(), fromww.size());
    int lca_idx = 0;
    for (int j = 0; j < minlen; j++) {
        if (fromvv[j] == fromww[j]) lca_idx = j;
        else break;
    }
    vector<int> cycle;
    for (int j = 0; j <= lca_idx; j++) cycle.push_back(fromvv[j]);
    for (int j = lca_idx - 1; j >= 0; j--) cycle.push_back(fromww[j]);
    cout << "N " << cycle.size() << endl;
    for (int x : cycle) cout << x << " ";
    cout << endl;
    return 0;
}